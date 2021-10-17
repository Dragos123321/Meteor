#pragma once

#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"

#include <glm/glm.hpp>

namespace mtr::gl 
{
	class Texture2D;
}

class BoardRenderer
{
public:
	BoardRenderer();
	BoardRenderer(BoardRenderer&& other) noexcept = default;

	BoardRenderer& operator=(BoardRenderer&& other) noexcept = default;

	void render(mtr::gl::Texture2D& board_texture, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view);

private:
	static const char* const VERTEX_SHADER_SOURCE;
	static const char* const FRAGMENT_SHADER_SOURCE;

	struct Vertex
	{
		constexpr Vertex() = default;
		constexpr Vertex(const glm::vec3& position, const glm::vec2& tex_coords);

		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec2 tex_coords = { 0.f, 0.f };
	};

	void setup_vertex_array();
	void setup_buffers();
	void setup_program();

	mtr::gl::VertexArray m_vao;
	mtr::gl::VertexBuffer<Vertex> m_vbo;
	mtr::gl::IndexBuffer m_ebo;
	mtr::gl::Program m_program;
};

#pragma region Implementation
constexpr BoardRenderer::Vertex::Vertex(const glm::vec3& position, const glm::vec2& tex_coords) :
	position{ position },
	tex_coords{ tex_coords }
{}
#pragma endregion