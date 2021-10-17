#include "meteor/core/utils.h"
#include "meteor/graphics/camera_2d.h"
#include "meteor/graphics/text.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/renderers/gui_text_renderer.h"
#include "meteor/gui/font.h"

#include <algorithm>

namespace mtr
{
	GuiTextRenderer::GuiTextRenderer()
	{
		gl::VertexLayout layout(sizeof(Vertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, position));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, tex_coords));

		m_vao = gl::VertexArray(layout, m_vbo);

		m_program = gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	}

	void GuiTextRenderer::clear()
	{
		m_vertices.clear();
		m_indexes.clear();
		m_text_data.clear();
		m_ready_to_render = false;
	}

	void GuiTextRenderer::draw_text(const Text& text)
	{
		mtr::FontMetrics metrics = text.font().metrics();
		float scale = static_cast<float>(text.size()) / metrics.size;
		glm::vec2 texture_size(text.font().texture().width(), text.font().texture().height());

		glm::vec2 cursor = text.position();
		FloatRect glyph_bounds;
		uint32_t prev_char = '\0';

		m_text_data.push_back({ text, m_indexes.size() });

		for (uint32_t curr_char : text.text())
		{
			if (curr_char == '\n')
			{
				cursor.x = text.position().x;
				cursor.y += metrics.line_height * scale;
				continue;
			}

			const auto& glyph = text.font().glyphs().at(curr_char);
			glyph_bounds.x = cursor.x + glyph.offset.x * scale;
			glyph_bounds.y = cursor.y + glyph.offset.y * scale;
			glyph_bounds.w = glyph.tex_coords.w * scale;
			glyph_bounds.h = glyph.tex_coords.h * scale;

			cursor.x += glyph.advance * scale;

			auto kerning_it = text.font().kernings().find({ prev_char, curr_char });

			if (kerning_it != text.font().kernings().end())
			{
				cursor.x += kerning_it->second * scale;
			}

			prev_char = curr_char;

			GLuint first_index = static_cast<GLuint>(m_vertices.size());
			m_text_data.back().index_count += 6;

			m_vertices.push_back({ glyph_bounds.top_left(), glyph.tex_coords.top_left() / texture_size });
			m_vertices.push_back({ glyph_bounds.top_right(), glyph.tex_coords.top_right() / texture_size, });
			m_vertices.push_back({ glyph_bounds.bottom_right(), glyph.tex_coords.bottom_right() / texture_size });
			m_vertices.push_back({ glyph_bounds.bottom_left(), glyph.tex_coords.bottom_left() / texture_size });	

			m_indexes.push_back(first_index);
			m_indexes.push_back(first_index + 1);
			m_indexes.push_back(first_index + 3);
			m_indexes.push_back(first_index + 3);
			m_indexes.push_back(first_index + 1);
			m_indexes.push_back(first_index + 2);
		}

		m_ready_to_render = false;
	}

	void GuiTextRenderer::upload_data()
	{
		if (!m_ready_to_render)
		{
			gl::VertexBufferBinder vbo_binder(m_vbo);
			vbo_binder.upload_data(m_vertices.data(), m_vertices.size(), gl::BufferUsage::DynamicDraw);

			gl::IndexBufferBinder ebo_binder(m_ebo);
			ebo_binder.upload_data(m_indexes.data(), m_indexes.size(), gl::BufferUsage::DynamicDraw);

			m_ready_to_render = true;
		}
	}

	void GuiTextRenderer::render(const Camera2D& camera)
	{
		upload_data();

		gl::ProgramBinder program_binder(m_program);
		program_binder["ortho_matrix"] = camera.final_matrix();

		gl::VertexArrayBinder vao_binder(m_vao);
		gl::IndexBufferBinder ebo_binder(m_ebo);

		gl::RenderSettings render_settings;
		render_settings.blend = true;
		render_settings.apply();

		for (const auto& text_data : m_text_data)
		{
			program_binder["width"] = text_data.width;
			program_binder["edge"] = text_data.edge;
			program_binder["color"] = text_data.color.as_vec3();
			program_binder["border_width"] = text_data.border_width;
			program_binder["border_edge"] = text_data.border_edge;
			program_binder["border_color"] = text_data.border_color.as_vec3();
			program_binder["border_offset"] = text_data.border_offset;

			gl::CONTEXT->bind_texture_2d(text_data.texture_id);
			gl::draw_elements_raw(GL_TRIANGLES, text_data.index_count, text_data.index_offset);
			gl::CONTEXT->unbind_texture_2d();
		}
	}

	/*
		Vertex
	*/
	GuiTextRenderer::Vertex::Vertex(const glm::vec2& position, const glm::vec2& tex_coords) :
		position{ position },
		tex_coords{ tex_coords }
	{}

	/*
		TextData
	*/
	GuiTextRenderer::TextData::TextData(const Text& text, size_t index_offset, size_t index_count) :
		texture_id{ text.font().texture().id() },
		index_offset{ static_cast<GLuint>(index_offset) },
		index_count{ static_cast<GLuint>(index_count) },
		width{ text.width() },
		edge{ text.edge() },
		border_width{ text.border_width() },
		border_edge{ text.border_edge() },
		border_offset{ text.normalized_border_offset() },
		color{ text.color() },
		border_color{ text.border_color() }
	{}

#pragma region Shaders
	const char* const GuiTextRenderer::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout(location = 0) in vec2 vert_position;
		layout(location = 1) in vec2 vert_tex_coords;

		out vec2 frag_tex_coords;
		flat out vec3 frag_color;

		uniform mat4 ortho_matrix;

		void main()
		{
			gl_Position = ortho_matrix * vec4(vert_position, 0.0, 1.0);
			frag_tex_coords = vec2(vert_tex_coords.x, 1.0 - vert_tex_coords.y);
		}
	)";

	const char* const GuiTextRenderer::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		in vec2 frag_tex_coords;

		out vec4 output_color;

		uniform sampler2D sampler;
		uniform float width;
		uniform float edge;
		uniform vec3 color;
		uniform float border_width;
		uniform float border_edge;
		uniform vec3 border_color;
		uniform vec2 border_offset;

		void main()
		{
			float distance = 1.0 - texture(sampler, frag_tex_coords).a;
			float alpha = (1.0 - smoothstep(width, width + edge, distance));
	
			if (border_width == 0.0)
			{
				output_color = vec4(color, alpha);
			}
			else
			{
				float border_distance = 1.0 - texture(sampler, frag_tex_coords + vec2(-border_offset.x, border_offset.y)).a;
				float border_alpha = (1.0 - smoothstep(width + border_width, width + border_width + border_edge, border_distance));
		
				vec3 final_color;
		
				if (border_alpha != 0.0)
					final_color = mix(border_color, color, alpha / border_alpha);
				else
					final_color = color;
		
				output_color = vec4(final_color, alpha + (1.0 - alpha) * border_alpha);
			}
		}
	)";
#pragma endregion
}