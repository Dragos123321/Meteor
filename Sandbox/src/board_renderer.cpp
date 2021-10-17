#include "board_renderer.h"
#include "meteor/graphics/opengl/texture_2d.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"

#include <array>

BoardRenderer::BoardRenderer()
{
	setup_vertex_array();
	setup_buffers();
	setup_program();
}

void BoardRenderer::render(mtr::gl::Texture2D& board_texture, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view)
{
	mtr::gl::ProgramBinder program_binder(m_program);
	program_binder["mvp_matrix"] = projection * view * model;

	mtr::gl::RenderSettings render_settings;
	render_settings.depth_test = true;
	render_settings.apply();

	mtr::gl::Texture2DBinder board_texture_binder(board_texture);
	mtr::gl::draw_elements(m_vao, m_ebo);
}

void BoardRenderer::setup_vertex_array()
{
	mtr::gl::VertexLayout layout(sizeof(Vertex));
	layout.add_attribute(mtr::gl::VertexAttributeType::FloatVec3, offsetof(Vertex, position));
	layout.add_attribute(mtr::gl::VertexAttributeType::FloatVec2, offsetof(Vertex, tex_coords));

	m_vao = mtr::gl::VertexArray(layout, m_vbo);
}

void BoardRenderer::setup_buffers()
{
	std::array<Vertex, 4> vertices = {
		Vertex({ 0.5f,  0.5f, 0.0f }, { 1.f, 0.f }),  // Top right
		Vertex({ 0.5f, -0.5f, 0.0f }, { 1.f, 1.f }),  // Bottom right
		Vertex({ -0.5f, -0.5f, 0.0f }, { 0.f, 1.f }), // Bottom left
		Vertex({ -0.5f,  0.5f, 0.0f }, { 0.f, 0.f })  // Top left 
	};

	std::array<GLuint, 6> indexes = {
		0, 1, 3,
		1, 2, 3
	};

	mtr::gl::VertexBufferBinder vbo_binder(m_vbo);
	vbo_binder.upload_data(vertices.data(), vertices.size());

	mtr::gl::IndexBufferBinder ebo_binder(m_ebo);
	ebo_binder.upload_data(indexes.data(), indexes.size());
}

void BoardRenderer::setup_program()
{
	m_program = mtr::gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
}

#pragma region Shaders
const char* const BoardRenderer::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout (location = 0) in vec3 position;
		layout (location = 1) in vec2 aTexCoords;

		out vec2 tex_coords;

		uniform mat4 mvp_matrix;

		void main()
		{
			gl_Position = mvp_matrix * vec4(position, 1.0);
			tex_coords = aTexCoords;
		}
	)";

const char* const BoardRenderer::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		in vec2 tex_coords;

		out vec4 output_color;

		uniform sampler2D sampler;

		void main()
		{
			output_color = texture(sampler, vec2(tex_coords.x, 1.0 - tex_coords.y));
		}
	)";
#pragma endregion