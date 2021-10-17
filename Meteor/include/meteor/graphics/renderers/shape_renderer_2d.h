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

	enum class BorderType
	{
		Inner,
		Centered,
		Outer
	};

	class ShapeRenderer2D
	{
	public:
		ShapeRenderer2D();

		void clear();
		void draw_rect(const FloatRect& bounds, const ColorRgba& color);
		void draw_border(FloatRect bounds, float thickness, const ColorRgba& color, BorderType type = BorderType::Inner);
		void render(const Camera2D& camera);

	private:
		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		struct Vertex
		{
			constexpr Vertex() = default;
			constexpr Vertex(const glm::vec2& position, const ColorRgba& color);

			glm::vec2 position = { 0.f, 0.f };
			ColorRgba color = COLOR_WHITE;
		};

		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indexes;
		bool m_ready_to_render = false;

		gl::VertexArray m_vao;
		gl::VertexBuffer<Vertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;
	};

#pragma region Implementation
	constexpr ShapeRenderer2D::Vertex::Vertex(const glm::vec2& position, const ColorRgba& color) :
		position{ position }, color{ color }
	{}
#pragma endregion
}
