#include "meteor/graphics/opengl/cubemap_texture.h"
#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/renderers/cubemap_renderer.h"

#include <array>

namespace mtr
{
	CubemapRenderer::CubemapRenderer()
	{
		setup_buffers();
		setup_vertex_array();
		setup_program();
	}

	void CubemapRenderer::render(const gl::CubemapTexture& cubemap_texture, const glm::mat4& projection, const glm::mat4& view)
	{
		gl::ProgramBinder program_binder(m_program);
		program_binder["view_proj_matrix"] = projection * view;

		gl::RenderSettings render_settings;
		render_settings.depth_test = true;
		render_settings.depth_function = gl::DepthFunction::Less;
		render_settings.apply();

		gl::CubemapTextureBinder cubemap_texture_binder(cubemap_texture);
		gl::draw_elements(m_vao, m_ebo);
	}

	void CubemapRenderer::setup_vertex_array()
	{
		gl::VertexLayout layout(sizeof(Vertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(Vertex, position));

		m_vao = gl::VertexArray(layout, m_vbo);
	}

	void CubemapRenderer::setup_buffers()
	{
		std::array<Vertex, 8> vertices = {
			// Front
			Vertex({ -1.f, 1.f, -1.f }),  // 0 -> Top left
			Vertex({ 1.f, 1.f, -1.f }),	  // 1 -> Top right
			Vertex({ -1.f, -1.f, -1.f }), // 2 -> Bottom left
			Vertex({ 1.f, -1.f, -1.f }),  // 3 -> Bottom right
												   
			// Back					   
			Vertex({ -1.f, 1.f, 1.f }),	 // 4 -> Top left
			Vertex({ 1.f, 1.f, 1.f }),	 // 5 -> Top right
			Vertex({ -1.f, -1.f, 1.f }), // 6 -> Bottom left
			Vertex({ 1.f, -1.f, 1.f }),	 // 7 -> Bottom right
		};

		std::array<GLuint, 36> indexes = {
			// Front
			0, 1, 2,
			2, 1, 3,

			// Back
			4, 5, 6,
			6, 5, 7,

			// Top
			0, 1, 4,
			4, 1, 5,

			// Bottom
			2, 3, 6,
			6, 3, 7,

			// Left
			0, 2, 4,
			4, 2, 6,

			// Right
			1, 3, 5,
			5, 3, 7
		};

		gl::VertexBufferBinder vbo_binder(m_vbo);
		vbo_binder.upload_data(vertices.data(), vertices.size());

		gl::IndexBufferBinder ebo_binder(m_ebo);
		ebo_binder.upload_data(indexes.data(), indexes.size());
	}

	void CubemapRenderer::setup_program()
	{
		m_program = gl::Program(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
	}

#pragma region Shaders
	const char* const CubemapRenderer::VERTEX_SHADER_SOURCE = R"(
		#version 330 core

		layout(location = 0) in vec3 position;

		out vec3 tex_coords;

		uniform mat4 view_proj_matrix;

		void main()
		{
			gl_Position = view_proj_matrix * vec4(position, 1.0);
			tex_coords = vec3(-position.x, -position.y, position.z);
		}
	)";

	const char* const CubemapRenderer::FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core

		in vec3 tex_coords;

		out vec4 output_color;

		uniform samplerCube cube_sampler;

		void main()
		{
			output_color = texture(cube_sampler, tex_coords);
		}
	)";
#pragma endregion
}