#include "meteor/graphics/opengl/drawing.h"
#include "meteor/graphics/opengl/render_settings.h"
#include "meteor/graphics/opengl/vertex_array.h"

namespace mtr::gl
{
	/*
		Draw elements raw
	*/
	void clear_color_and_depth(const ColorRgba& color, float depth)
	{
		gl::RenderSettings render_settings;
		render_settings.depth_test = false;
		render_settings.apply();
		
		glClearColor(
			color.r / 255.f,
			color.g / 255.f,
			color.b / 255.f,
			color.a / 255.f
		);
		glClearDepth(depth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void draw_elements_raw(GLenum primitive, GLuint index_count, GLuint index_offset)
	{
		glDrawElements(primitive, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, 
			reinterpret_cast<void*>(sizeof(GLuint) * index_offset));
	}

	void draw_elements_instanced_raw(GLenum primitive, GLuint index_count, size_t instance_count, GLuint index_offset)
	{
		glDrawElementsInstanced(primitive, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT,
			reinterpret_cast<void*>(sizeof(GLuint) * index_offset), static_cast<GLsizei>(instance_count));
	}

	/*
		Draw arrays
	*/
	void draw_arrays(const VertexArray& vao, size_t vertex_count)
	{
		draw_arrays(VertexArrayBinder(vao), vertex_count);
	}

	void draw_arrays(const VertexArrayBinder& vao_binder, size_t vertex_count)
	{
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count));
	}

	void draw_arrays_instanced(const VertexArray& vao, size_t vertex_count, size_t instance_count)
	{
		draw_arrays_instanced(VertexArrayBinder(vao), vertex_count, instance_count);
	}

	void draw_arrays_instanced(const VertexArrayBinder& vao_binder, size_t vertex_count, size_t instance_count)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count), static_cast<GLsizei>(instance_count));
	}

	/*
		Draw elements
	*/
	void draw_elements(const VertexArray& vao, const IndexBuffer& ebo)
	{
		VertexArrayBinder vao_binder(vao);
		ConstIndexBufferBinder ebo_binder(ebo);

		draw_elements_raw(GL_TRIANGLES, ebo.size());
	}

	void draw_elements(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder)
	{
		draw_elements_raw(GL_TRIANGLES, ebo_binder.buffer().size());
	}

	void draw_elements_instanced(const VertexArray& vao, const IndexBuffer& ebo, size_t instance_count)
	{
		VertexArrayBinder vao_binder(vao);
		ConstIndexBufferBinder ebo_binder(ebo);

		draw_elements_instanced_raw(GL_TRIANGLES, ebo.size(), instance_count);
	}

	void draw_elements_instanced(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder, size_t instance_count)
	{
		draw_elements_instanced_raw(GL_TRIANGLES, ebo_binder.buffer().size(), instance_count);
	}

	/*
		Draw elements triangle strip
	*/
	void draw_elements_triangle_strip(const VertexArray& vao, const IndexBuffer& ebo)
	{
		VertexArrayBinder vao_binder(vao);
		ConstIndexBufferBinder ebo_binder(ebo);

		draw_elements_raw(GL_TRIANGLE_STRIP, ebo.size());
	}

	void draw_elements_triangle_strip(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder)
	{
		glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(ebo_binder.buffer().size()), GL_UNSIGNED_INT, nullptr);
	}

	void draw_elements_triangle_strip_instanced(const VertexArray& vao, const IndexBuffer& ebo, size_t instance_count)
	{
		VertexArrayBinder vao_binder(vao);
		ConstIndexBufferBinder ebo_binder(ebo);

		draw_elements_instanced_raw(GL_TRIANGLE_STRIP, ebo.size(), instance_count);
	}

	void draw_elements_triangle_strip_instanced(const VertexArrayBinder& vao_binder,
		const IndexBufferBinder& ebo_binder, size_t instance_count)
	{
		draw_elements_instanced_raw(GL_TRIANGLE_STRIP, ebo_binder.buffer().size(), instance_count);
	}
}