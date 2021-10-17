#pragma once

#include "meteor/graphics/color.h"
#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"
#include "meteor/math/rect.h"

namespace mtr
{
	class Camera2D;

	class GuiShapeRenderer
	{
	public:
		static const float DEFAULT_SMOOTHNESS;

		GuiShapeRenderer();

		void clear();
		void draw_rounded_rect(const FloatRect& bounds, const ColorRgba& color, float radius, float smoothness = DEFAULT_SMOOTHNESS);
		void render(const Camera2D& camera);

	private:
		void upload_data();

		struct Vertex
		{
			Vertex() = default;
			Vertex(const glm::vec2& position, const FloatRect& bounds, float radius, float smoothness, const ColorRgba& color);

			glm::vec2 position;
			float top;
			float bottom;
			float left;
			float right;
			float radius;
			float smoothness;
			ColorRgba color;
		};

		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indexes;
		bool m_ready_to_render = false;

		gl::VertexArray m_vao;
		gl::VertexBuffer<Vertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;
	};
}
