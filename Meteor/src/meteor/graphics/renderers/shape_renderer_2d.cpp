#include "meteor/graphics/camera_2d.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/opengl/shader_sources.h"
#include "meteor/graphics/renderers/shape_renderer_2d.h"

namespace mtr
{
	ShapeRenderer2D::ShapeRenderer2D()
	{
		gl::VertexLayout layout(sizeof(Vertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(Vertex, position));
		layout.add_attribute(gl::VertexAttributeType::UByteVec4, offsetof(Vertex, color), true);

		m_vao = gl::VertexArray(layout, m_vbo);

		m_program = gl::Program(gl::ShaderSources(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE));
	}

	void ShapeRenderer2D::clear()
	{
		m_vertices.clear();
		m_indexes.clear();

		m_ready_to_render = false;
	}

	void ShapeRenderer2D::draw_rect(const FloatRect& bounds, const ColorRgba& color)
	{
		GLuint first_index = static_cast<GLuint>(m_vertices.size());

		m_vertices.emplace_back(bounds.top_left(), color);
		m_vertices.emplace_back(bounds.top_right(), color);
		m_vertices.emplace_back(bounds.bottom_right(), color);
		m_vertices.emplace_back(bounds.bottom_left(), color);

		m_indexes.push_back(first_index);
		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 2);

		m_ready_to_render = false;
	}

	void ShapeRenderer2D::draw_border(FloatRect bounds, float thickness, const ColorRgba& color, BorderType type)
	{
		switch (type)
		{
		case BorderType::Inner:
			bounds.x += thickness;
			bounds.y += thickness;
			bounds.w -= thickness * 2.f;
			bounds.h -= thickness * 2.f;
			break;

		case BorderType::Centered:
			bounds.x += thickness / 2.f;
			bounds.y += thickness / 2.f;
			bounds.w -= thickness;
			bounds.h -= thickness;
			break;
		}

		FloatRect outer_bounds(
			bounds.x - thickness,
			bounds.y - thickness,
			bounds.w + thickness * 2.f,
			bounds.h + thickness * 2.f
		);

		GLuint first_index = static_cast<GLuint>(m_vertices.size());

		m_vertices.emplace_back(bounds.top_left(), color);
		m_vertices.emplace_back(bounds.top_right(), color);
		m_vertices.emplace_back(bounds.bottom_right(), color);
		m_vertices.emplace_back(bounds.bottom_left(), color);

		m_vertices.emplace_back(outer_bounds.top_left(), color);
		m_vertices.emplace_back(outer_bounds.top_right(), color);
		m_vertices.emplace_back(outer_bounds.bottom_right(), color);
		m_vertices.emplace_back(outer_bounds.bottom_left(), color);

		m_indexes.push_back(first_index);
		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 5);

		m_indexes.push_back(first_index);
		m_indexes.push_back(first_index + 5);
		m_indexes.push_back(first_index + 4);

		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 2);
		m_indexes.push_back(first_index + 6);

		m_indexes.push_back(first_index + 1);
		m_indexes.push_back(first_index + 6);
		m_indexes.push_back(first_index + 5);

		m_indexes.push_back(first_index + 2);
		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 7);

		m_indexes.push_back(first_index + 6);
		m_indexes.push_back(first_index + 2);
		m_indexes.push_back(first_index + 7);

		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 0);
		m_indexes.push_back(first_index + 4);

		m_indexes.push_back(first_index + 3);
		m_indexes.push_back(first_index + 4);
		m_indexes.push_back(first_index + 7);
	}

	void ShapeRenderer2D::render(const Camera2D& camera)
	{
		if (!m_ready_to_render)
		{
			gl::VertexBufferBinder<Vertex> vbo_binder(m_vbo);
			vbo_binder.upload_data(m_vertices.data(), m_vertices.size(), gl::BufferUsage::DynamicDraw);

			gl::IndexBufferBinder ebo_binder(m_ebo);
			ebo_binder.upload_data(m_indexes.data(), m_indexes.size(), gl::BufferUsage::DynamicDraw);

			m_ready_to_render = true;
		}

		gl::ProgramBinder program_binder(m_program);
		program_binder["camera_matrix"] = camera.final_matrix();

		gl::RenderSettings render_settings;
		render_settings.blend = true;
		render_settings.apply();

		gl::draw_elements(m_vao, m_ebo);
	}

#pragma region Shaders
	const char* const ShapeRenderer2D::VERTEX_SHADER_SOURCE = R"(
		#version 330 core
	
		layout(location = 0) in vec2 vertex_position;
		layout(location = 1) in vec4 vertex_color;

		out vec4 fragment_color;

		uniform mat4 camera_matrix;

		void main()
		{
			gl_Position = camera_matrix * vec4(vertex_position, 0.0, 1.0);
			fragment_color = vertex_color;
		}
	)";

	const char* const ShapeRenderer2D::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core
		
		in vec4 fragment_color;

		out vec4 output_color;

		void main()
		{
			output_color = fragment_color;
		}
	)";
#pragma endregion
}