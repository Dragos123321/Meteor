#include "meteor/graphics/camera_2d.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/opengl/shader_sources.h"
#include "meteor/graphics/opengl/texture_2d.h"
#include "meteor/graphics/renderers/texture_renderer_2d.h"

#include <algorithm>

namespace mtr
{
	TextureRenderer2D::TextureRenderer2D()
	{
		gl::VertexLayout layout(sizeof(Vertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, position));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, tex_coords));
		layout.add_attribute(gl::VertexAttributeType::UByteVec4, offsetof(Vertex, color), true);

		m_vao = gl::VertexArray(layout, m_vbo);

		m_program = gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	}

	void TextureRenderer2D::draw(const gl::Texture2D& texture, const glm::vec2& position, const ColorRgba& color)
	{
		FloatRect bounds(position.x, position.y, static_cast<float>(texture.width()), static_cast<float>(texture.height()));
		draw(texture, bounds, color);
	}

	void TextureRenderer2D::draw(const gl::Texture2D& texture, const FloatRect& bounds, const ColorRgba& color)
	{
		draw(texture, bounds, { 0.f, 0.f, 1.f, 1.f }, color);
	}

	void TextureRenderer2D::draw(const gl::Texture2D& texture, const FloatRect& bounds,
		const FloatRect& tex_coords, const ColorRgba& color)
	{
		m_drawable_data.push_back(DrawableData(static_cast<GLuint>(m_vertices.size()), texture.id()));

		m_vertices.push_back(Vertex(bounds.top_left(), tex_coords.top_left(), color));
		m_vertices.push_back(Vertex(bounds.top_right(), tex_coords.top_right(), color));
		m_vertices.push_back(Vertex(bounds.bottom_right(), tex_coords.bottom_right(), color));
		m_vertices.push_back(Vertex(bounds.bottom_left(), tex_coords.bottom_left(), color));

		m_ready_to_render = false;
	}

	void TextureRenderer2D::clear()
	{
		m_drawable_data.clear();
		m_batches.clear();
		m_vertices.clear();
		m_indexes.clear();

		m_ready_to_render = false;
	}

	void TextureRenderer2D::prepare_rendering(bool sort_drawables)
	{
		if (!m_ready_to_render)
		{
			m_batches.clear();
			m_indexes.clear();

			if (sort_drawables)
			{
				std::sort(m_drawable_data.begin(), m_drawable_data.end(), [](const DrawableData& d1, const DrawableData& d2) {
					return d1.texture_id < d2.texture_id;
				});
			}

			GLuint last_texture_id = 0;

			for (const auto& drawable_data : m_drawable_data)
			{
				if (drawable_data.texture_id == last_texture_id)
				{
					m_batches.back().index_count += 6;
				}
				else
				{
					m_batches.push_back(Batch(drawable_data.texture_id, static_cast<GLuint>(m_indexes.size()), 6));
					last_texture_id = drawable_data.texture_id;
				}

				m_indexes.push_back(drawable_data.first_vertex_index);    
				m_indexes.push_back(drawable_data.first_vertex_index + 1);
				m_indexes.push_back(drawable_data.first_vertex_index + 3);
				m_indexes.push_back(drawable_data.first_vertex_index + 3);
				m_indexes.push_back(drawable_data.first_vertex_index + 1);
				m_indexes.push_back(drawable_data.first_vertex_index + 2);
			}

			gl::VertexBufferBinder vbo_binder(m_vbo);
			vbo_binder.upload_data(m_vertices.data(), m_vertices.size(), gl::BufferUsage::DynamicDraw);

			gl::IndexBufferBinder ebo_binder(m_ebo);
			ebo_binder.upload_data(m_indexes.data(), m_indexes.size(), gl::BufferUsage::DynamicDraw);

			m_ready_to_render = true;
		}
	}

	void TextureRenderer2D::render(const Camera2D& camera, bool sort_drawables)
	{
		prepare_rendering();

		gl::ProgramBinder program_binder(m_program);
		program_binder["camera_matrix"] = camera.final_matrix();

		gl::VertexArrayBinder vao_binder(m_vao);
		gl::IndexBufferBinder ebo_binder(m_ebo);

		gl::RenderSettings render_settings;
		render_settings.blend = true;
		render_settings.apply();

		for (const auto& batch : m_batches)
		{
			gl::CONTEXT->bind_texture_2d(batch.texture_id);
			gl::draw_elements_raw(GL_TRIANGLES, batch.index_count, batch.index_offset);
			gl::CONTEXT->unbind_texture_2d();
		}
	}

#pragma region Shaders
	const char* const TextureRenderer2D::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout(location = 0) in vec2 vert_position;
		layout(location = 1) in vec2 vert_tex_coords;
		layout(location = 2) in vec4 vert_color;

		out vec2 frag_tex_coords;
		out vec4 frag_color;

		uniform mat4 camera_matrix;

		void main()
		{
			gl_Position = vec4((camera_matrix * vec4(vert_position, 0.0, 1.0)).xy, 0.0, 1.0);
			frag_tex_coords = vert_tex_coords;
			frag_color = vert_color;
		}
	)";

	const char* const TextureRenderer2D::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		in vec2 frag_tex_coords;
		in vec4 frag_color;

		out vec4 output_color;

		uniform sampler2D sampler;

		void main()
		{
			output_color = frag_color * texture(sampler, vec2(frag_tex_coords.x, 1.0 - frag_tex_coords.y));
		}
	)";
#pragma endregion
}