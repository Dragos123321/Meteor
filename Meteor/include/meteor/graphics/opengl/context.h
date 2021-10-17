#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/opengl/binding_data.h"

#include <glad/glad.h>

namespace mtr::gl 
{
	class Context;

	extern Context* CONTEXT;

	struct BufferTargetData
	{
		constexpr BufferTargetData() = default;

		GLuint bound_buffer = 0;
		bool binder_active = false;
	};

	class Context : NonCopyable, NonMovable
	{
	public:
		Context();
		~Context();

		void bind_buffer(GLenum target, GLuint buffer, GLuint binding = 0);
		void bind_vertex_buffer(GLuint vertex_buffer);
		void bind_index_buffer(GLuint index_buffer);
		void bind_program(GLuint program);
		void bind_vertex_array(GLuint vertex_array);
		void bind_texture_unit(GLuint texture_unit);
		void bind_texture_2d(GLuint texture_id, GLuint texture_unit = 0);
		void bind_cubemap_texture(GLuint cubemap_texture, GLuint texture_unit = 0);
		void bind_renderbuffer(GLuint renderbuffer);
		void bind_framebuffer(GLuint framebuffer);
		void bind_read_framebuffer(GLuint framebuffer);
		void bind_draw_framebuffer(GLuint framebuffer);

		void unbind_buffer(GLenum target);
		void unbind_vertex_buffer();
		void unbind_index_buffer();
		void unbind_program();
		void unbind_vertex_array();
		void unbind_texture_unit();
		void unbind_texture_2d();
		void unbind_cubemap_texture();
		void unbind_renderbuffer();
		void unbind_framebuffer();
		void unbind_read_framebuffer();
		void unbind_draw_framebuffer();

	private:
		BindingData m_vertex_buffer_binding;
		BindingData m_index_buffer_binding;
		BindingData m_program_binding;
		BindingData m_vertex_array_binding;
		BindingData m_texture_unit_binding;
		BindingData m_texture_2d_binding;
		BindingData m_cubemap_texture_binding;
		BindingData m_renderbuffer_binding;
		BindingData m_read_framebuffer_binding;
		BindingData m_draw_framebuffer_binding;
	};
}