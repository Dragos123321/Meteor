#pragma once

#include "meteor/graphics/color.h"
#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"

namespace mtr
{
	class Camera2D;
	class Font;
	class Text;

	class GuiTextRenderer
	{
	public:
		GuiTextRenderer();

		void clear();
		void draw_text(const Text& text);
		void upload_data();
		void render(const Camera2D& camera);

	private:
		static const char* const VERTEX_SHADER_SOURCE;
		static const char* const FRAGMENT_SHADER_SOURCE;

		struct Vertex
		{
			Vertex(const glm::vec2& position, const glm::vec2& tex_coords);

			glm::vec2 position;
			glm::vec2 tex_coords;
		};

		struct TextData
		{
			TextData(const Text& text, size_t index_offset, size_t index_count = 0);

			GLuint texture_id;
			GLuint index_offset;
			GLuint index_count;
			float width;
			float edge;
			float border_width;
			float border_edge;
			glm::vec2 border_offset;
			ColorRgbF color;
			ColorRgbF border_color;
		};

		gl::VertexArray m_vao;
		gl::VertexBuffer<Vertex> m_vbo;
		gl::IndexBuffer m_ebo;
		gl::Program m_program;

		std::vector<TextData> m_text_data;
		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indexes;
		bool m_ready_to_render = false;
	};
}