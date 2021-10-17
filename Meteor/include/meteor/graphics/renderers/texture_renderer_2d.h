#pragma once

#include "meteor/graphics/color.h"
#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"
#include "meteor/math/rect.h"

namespace mtr::gl
{
	class Texture2D;
}

namespace mtr
{
	class Camera2D;

	class TextureRenderer2D
	{
	public:
		TextureRenderer2D();

		void draw(const gl::Texture2D& texture, const glm::vec2& position, const ColorRgba& color = COLOR_WHITE);
		void draw(const gl::Texture2D& texture, const FloatRect& bounds, const ColorRgba& color = COLOR_WHITE);
		void draw(const gl::Texture2D& texture, const FloatRect& bounds, const FloatRect& tex_coords, const ColorRgba& color = COLOR_WHITE);

		void clear();
		void prepare_rendering(bool sort_drawables = true);
		void render(const Camera2D& camera, bool sort_drawables = true);

	private:
		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		struct Vertex
		{
			constexpr Vertex() = default;
			constexpr Vertex(const glm::vec2& position, const glm::vec2& tex_coords, const ColorRgba& color);

			glm::vec2 position = { 0.f, 0.f };
			glm::vec2 tex_coords = { 0.f, 0.f };
			ColorRgba color;
		};

		struct DrawableData
		{
			constexpr DrawableData() = default;
			constexpr DrawableData(GLuint first_vertex_index, GLuint texture_id);

			GLuint first_vertex_index = 0;
			GLuint texture_id = 0;
		};

		struct Batch
		{
			constexpr Batch() = default;
			constexpr Batch(GLuint texture_id, GLuint index_offset, GLuint index_count);

			GLuint texture_id = 0;
			GLuint index_offset = 0;
			GLuint index_count = 0;
		};

		std::vector<DrawableData> m_drawable_data;
		std::vector<Batch> m_batches;
		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indexes;
		bool m_ready_to_render = false;

		gl::VertexArray m_vao;
		gl::VertexBuffer<Vertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;

		float m_exposure_factor = 1.f;
	};

#pragma region Implementation
	/*
		Vertex
	*/
	constexpr TextureRenderer2D::Vertex::Vertex(const glm::vec2& position, const glm::vec2& tex_coords, const ColorRgba& color) :
		position{ position },
		tex_coords{ tex_coords },
		color{ color }
	{}

	/*
		DrawableData
	*/
	constexpr TextureRenderer2D::DrawableData::DrawableData(GLuint first_vertex_index, GLuint texture_id) :
		first_vertex_index{ first_vertex_index },
		texture_id{ texture_id }
	{}

	/*
		Batch
	*/
	constexpr TextureRenderer2D::Batch::Batch(GLuint texture_id, GLuint index_offset, GLuint index_count) :
		texture_id{ texture_id }, 
		index_offset{ index_offset }, 
		index_count{ index_count }
	{}
#pragma endregion
}