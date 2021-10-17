#pragma once

#include <meteor/graphics/opengl/index_buffer.h>
#include <meteor/graphics/opengl/program.h>
#include <meteor/graphics/opengl/vertex_array.h>
#include <meteor/graphics/opengl/vertex_buffer.h>

#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer();

	void render(mtr::gl::Program& program);

private:
	struct Vertex
	{
		constexpr Vertex() = default;
		constexpr Vertex(const glm::vec3& position);

		glm::vec3 position = { 0.f, 0.f, 0.f };
	};

	void setup_vertex_array();
	void setup_buffers();

	mtr::gl::VertexArray m_vao;
	mtr::gl::VertexBuffer<Vertex> m_vbo;
	mtr::gl::IndexBuffer m_ebo;
};

#pragma region Implementation
constexpr Renderer::Vertex::Vertex(const glm::vec3& position) : 
	position{ position }
{}
#pragma endregion

