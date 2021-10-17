#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/opengl/shader_sources.h"
#include "meteor/graphics/renderers/point_light_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace mtr 
{
	PointLightVertex::PointLightVertex(const glm::vec3& pos) :
		position{ pos }
	{}

	PointLightRenderer::PointLightRenderer()
	{
		setup_buffers();
		setup_vertex_array();
		setup_program();
	}

	void PointLightRenderer::draw(const PointLight& light, const glm::mat4& projection, const glm::mat4& view)
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, light.position);
		model = glm::scale(model, glm::vec3(0.5f));

		gl::ProgramBinder program_binder(m_program);
		program_binder["color"] = light.color;
		program_binder["model"] = model;
		program_binder["view"] = view;
		program_binder["projection"] = projection;

		gl::RenderSettings render_settings;
		render_settings.depth_test = true;
		render_settings.apply();

		gl::draw_elements(m_vao, m_ebo);
	}

	void PointLightRenderer::setup_vertex_array()
	{
		gl::VertexLayout layout(sizeof(PointLightVertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(PointLightVertex, position));

		m_vao = gl::VertexArray(layout, m_vbo);
	}

	void PointLightRenderer::setup_buffers()
	{
		std::array<PointLightVertex, 8> vertices = {
			// Front
			PointLightVertex(glm::vec3(-1.f, -1.f, 1.f)), // Left bottom -> 0
			PointLightVertex(glm::vec3(1.f, -1.f, 1.f)),  // Right bottom -> 1
			PointLightVertex(glm::vec3(1.f, 1.f, 1.f)),   // Right top -> 2
			PointLightVertex(glm::vec3(-1.f, 1.f, 1.f)),  // Left top -> 3

			// Back
			PointLightVertex(glm::vec3(-1.f, -1.f, -1.f)), // Left bottom -> 4
			PointLightVertex(glm::vec3(1.f, -1.f, -1.f)),  // Right bottom -> 5
			PointLightVertex(glm::vec3(1.f, 1.f, -1.f)),   // Right top -> 6
			PointLightVertex(glm::vec3(-1.f, 1.f, -1.f))   // Left top -> 7
		};

		std::array<GLuint, 36> indexes = {
			// Right
			1, 5, 6,
			6, 2, 1,

			// Left
			0, 4, 7,
			7, 3, 0,

			// Top
			2, 3, 7,
			7, 6, 2,

			// Bottom
			1, 0, 4,
			4, 5, 1,

			// Front
			1, 0, 3,
			3, 2, 1,

			// Back
			5, 4, 7,
			7, 6, 5
		};

		gl::VertexBufferBinder vbo_binder(m_vbo);
		vbo_binder.upload_data(vertices.data(), vertices.size());

		gl::IndexBufferBinder ebo_binder(m_ebo);
		ebo_binder.upload_data(indexes.data(), indexes.size());
	}

	void PointLightRenderer::setup_program()
	{
		m_program = gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	}

	const char* const PointLightRenderer::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
	)";

	const char* const PointLightRenderer::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		out vec4 FragColor;

		uniform vec3 color;

		void main()
		{
			FragColor = vec4(color, 1.0);
		}
	)";
}