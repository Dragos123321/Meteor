#include "renderer.h"

#include <meteor/graphics/opengl/render_settings.h>
#include <meteor/graphics/opengl/drawing.h>

#include <array>

Renderer::Renderer()
{
	setup_buffers();
	setup_vertex_array();
}

void Renderer::render(mtr::gl::Program& program)
{
	mtr::gl::ProgramBinder program_binder(program);

	mtr::gl::RenderSettings render_settings;
	render_settings.depth_test = true;
	render_settings.apply();

	mtr::gl::draw_elements(m_vao, m_ebo);
}

void Renderer::setup_vertex_array()
{
	mtr::gl::VertexLayout layout(sizeof(Vertex));
	layout.add_attribute(mtr::gl::VertexAttributeType::FloatVec3, offsetof(Vertex, position));

	m_vao = mtr::gl::VertexArray(layout, m_vbo);
}

void Renderer::setup_buffers()
{
	std::array<Vertex, 4> vertices = {
		Vertex(glm::vec3(1.f,  1.f, 0.0f)),  // top right
		Vertex(glm::vec3(1.f, -1.f, 0.0f)),  // bottom right
		Vertex(glm::vec3(-1.f, -1.f, 0.0f)), // bottom left
		Vertex(glm::vec3(-1.f,  1.f, 0.0f))  // top left 
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
