#pragma once

#include "meteor/graphics/color.h"
#include "meteor/graphics/opengl/index_buffer.h"

namespace mtr::gl
{
	class VertexArray;
	class VertexArrayBinder;

	extern void clear_color_and_depth(const ColorRgba& color = COLOR_WHITE, float depth = 1.f);

	extern void draw_elements_raw(GLenum primitive, GLuint index_count, GLuint index_offset = 0);
	extern void draw_elements_instanced_raw(GLenum primitive, GLuint index_count, size_t instance_count, GLuint index_offset = 0);

	extern void draw_arrays(const VertexArray& vao, size_t vertex_count);
	extern void draw_arrays(const VertexArrayBinder& vao_binder, size_t vertex_count);
	extern void draw_arrays_instanced(const VertexArray& vao, size_t vertex_count, size_t instance_count);
	extern void draw_arrays_instanced(const VertexArrayBinder& vao_binder, size_t vertex_count, size_t instance_count);

	extern void draw_elements(const VertexArray& vao, const IndexBuffer& ebo);
	extern void draw_elements(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder);
	extern void draw_elements_instanced(const VertexArray& vao, const IndexBuffer& ebo, size_t instance_count);
	extern void draw_elements_instanced(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder, size_t instance_count);

	extern void draw_elements_triangle_strip(const VertexArray& vao, const IndexBuffer& ebo);
	extern void draw_elements_triangle_strip(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder);
	extern void draw_elements_triangle_strip_instanced(const VertexArray& vao, const IndexBuffer& ebo, size_t instance_count);
	extern void draw_elements_triangle_strip_instanced(const VertexArrayBinder& vao_binder, const IndexBufferBinder& ebo_binder, size_t instance_count);
}