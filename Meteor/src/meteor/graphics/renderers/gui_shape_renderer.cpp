#include "meteor/core/utils.h"
#include "meteor/graphics/camera_2d.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/renderers/gui_shape_renderer.h"

#include <algorithm>

namespace mtr
{
	const float GuiShapeRenderer::DEFAULT_SMOOTHNESS = 0.7f;

	GuiShapeRenderer::GuiShapeRenderer()
	{
		gl::VertexLayout layout(sizeof(Vertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, position));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, top));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, bottom));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, left));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, right));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, radius));
		layout.add_attribute(gl::VertexAttributeType::Float, offsetof(Vertex, smoothness));
		layout.add_attribute(gl::VertexAttributeType::UByteVec4, offsetof(Vertex, color), true);
		m_vao = gl::VertexArray(layout, m_vbo);

		m_program = gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	}

	void GuiShapeRenderer::clear()
	{
		m_vertices.clear();
		m_indexes.clear();
		m_ready_to_render = false;
	}

	void GuiShapeRenderer::draw_rounded_rect(const FloatRect& bounds, const ColorRgba& color, float radius, float smoothness)
	{
		GLuint first_index = static_cast<GLuint>(m_vertices.size());
		radius = std::min(radius, std::min(bounds.w, bounds.h) / 2.f);
		smoothness = std::clamp(smoothness, 0.f, 1.f);

		m_vertices.push_back({ bounds.top_left(), bounds, radius, smoothness, color });
		m_vertices.push_back({ bounds.top_right(), bounds, radius, smoothness, color });
		m_vertices.push_back({ bounds.bottom_right(), bounds, radius, smoothness, color });
		m_vertices.push_back({ bounds.bottom_left(), bounds, radius, smoothness, color });

		m_indexes.push_back(first_index);
		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 2);

		m_ready_to_render = false;
	}

	void GuiShapeRenderer::render(const Camera2D& camera)
	{
		upload_data();

		gl::ProgramBinder program_binder(m_program);
		program_binder["ortho_matrix"] = camera.final_matrix();

		gl::RenderSettings render_settings;
		render_settings.blend = true;
		render_settings.apply();

		gl::draw_elements(m_vao, m_ebo);
	}

	void GuiShapeRenderer::upload_data()
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

	/*
		Vertex
	*/
	GuiShapeRenderer::Vertex::Vertex(const glm::vec2& position, const FloatRect& bounds,
		float radius, float smoothness, const ColorRgba& color) :

		position{ position },
		top{ bounds.top() + radius },
		bottom{ bounds.bottom() - radius },
		left{ bounds.left() + radius },
		right{ bounds.right() - radius },
		radius{ radius },
		smoothness{ smoothness },
		color{ color }
	{}

#pragma region Shaders
	const char* const GuiShapeRenderer::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout(location = 0) in vec2 vert_position;
		layout(location = 1) in float vert_top;
		layout(location = 2) in float vert_bottom;
		layout(location = 3) in float vert_left;
		layout(location = 4) in float vert_right;
		layout(location = 5) in float vert_radius;
		layout(location = 6) in float vert_smoothness;
		layout(location = 7) in vec4 vert_color;

		out vec2 frag_position;
		flat out float frag_top;
		flat out float frag_bottom;
		flat out float frag_left;
		flat out float frag_right;
		flat out float frag_radius;
		flat out float frag_smoothness;
		flat out vec4 frag_color; 

		uniform mat4 ortho_matrix;

		void main()
		{
			gl_Position = ortho_matrix * vec4(vert_position, 0.0, 1.0);
	
			frag_position = vert_position;
			frag_top = vert_top;
			frag_bottom = vert_bottom;
			frag_left = vert_left;
			frag_right = vert_right;
			frag_radius = vert_radius;
			frag_smoothness = vert_smoothness;
			frag_color = vert_color;
		}
	)";

	const char* const GuiShapeRenderer::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		in vec2 frag_position;
		flat in float frag_top;
		flat in float frag_bottom;
		flat in float frag_left;
		flat in float frag_right;
		flat in float frag_radius;
		flat in float frag_smoothness;
		flat in vec4 frag_color; 

		out vec4 output_color;

		bool is_inside_top_left_corner()
		{
			return
				frag_position.x <= frag_left &&
				frag_position.y <= frag_top;
		}

		bool is_inside_top_right_corner()
		{
			return
				frag_position.x >= frag_right &&
				frag_position.y <= frag_top;
		}

		bool is_inside_bottom_right_corner()
		{
			return
				frag_position.x >= frag_right &&
				frag_position.y >= frag_bottom;
		}

		bool is_inside_bottom_left_corner()
		{
			return
				frag_position.x <= frag_left &&
				frag_position.y >= frag_bottom;
		}

		float compute_alpha(vec2 anchor)
		{
			return 1.0 - smoothstep(
				frag_radius - frag_smoothness, 
				frag_radius + frag_smoothness,
				length(frag_position - anchor)
			);
		}

		void main()
		{
			output_color = frag_color;

			if (frag_radius != 0.0)
			{
				if (is_inside_top_left_corner())
				{
					output_color.a *= compute_alpha(vec2(frag_left, frag_top));
				}
				else if (is_inside_top_right_corner())
				{
					output_color.a *= compute_alpha(vec2(frag_right, frag_top));
				}
				else if (is_inside_bottom_right_corner())
				{
					output_color.a *= compute_alpha(vec2(frag_right, frag_bottom));
				}
				else if (is_inside_bottom_left_corner())
				{
					output_color.a *= compute_alpha(vec2(frag_left, frag_bottom));
				}
			}
		}
	)";
#pragma endregion
}