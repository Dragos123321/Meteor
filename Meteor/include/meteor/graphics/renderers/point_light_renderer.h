#pragma once

#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"
#include "meteor/graphics/point_light.h"

#include <glm/glm.hpp>

namespace mtr
{
	struct PointLightVertex {
		PointLightVertex() = default;
		PointLightVertex(const glm::vec3& pos);

		glm::vec3 position = glm::vec3{0.f, 0.f, 0.f};
	};

	class PointLightRenderer 
	{
	public:
		PointLightRenderer();
		PointLightRenderer(PointLightRenderer&& other) noexcept = default;

		PointLightRenderer& operator=(PointLightRenderer&& other) noexcept = default;

		void draw(const PointLight& light, const glm::mat4& projection, const glm::mat4& view);

	private:
		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		void setup_vertex_array();
		void setup_buffers();
		void setup_program();

		gl::VertexArray m_vao;
		gl::VertexBuffer<PointLightVertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;
	};
}