#pragma once

#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"

#include <glm/glm.hpp>

namespace mtr::gl
{
	class CubemapTexture;
}

namespace mtr
{
	class CubemapRenderer
	{
	public:
		CubemapRenderer();
		CubemapRenderer(CubemapRenderer&& other) noexcept = default;

		CubemapRenderer& operator=(CubemapRenderer&& other) noexcept = default;

		void render(const gl::CubemapTexture& cubemap_texture, const glm::mat4& projection, const glm::mat4& view);

	private:
		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		struct Vertex
		{
			constexpr Vertex() = default;
			constexpr Vertex(const glm::vec3& position);

			glm::vec3 position = { 0.f, 0.f, 0.f };
		};

		void setup_vertex_array();
		void setup_buffers();
		void setup_program();

		gl::VertexArray m_vao;
		gl::VertexBuffer<Vertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;
	};

#pragma region Implementation
	constexpr CubemapRenderer::Vertex::Vertex(const glm::vec3& position) :
		position{ position }
	{}
#pragma endregion
}
