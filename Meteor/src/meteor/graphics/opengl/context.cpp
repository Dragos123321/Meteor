#include "meteor/graphics/opengl/context.h"

#include <cassert>

namespace mtr::gl
{
	Context* CONTEXT = nullptr;

	Context::Context()
	{
		CONTEXT = this;
	}

	Context::~Context()
	{
		CONTEXT = nullptr;
	}

	void Context::bind_buffer(GLenum target, GLuint buffer, GLuint binding)
	{
		switch (target)
		{
		case GL_ARRAY_BUFFER:
			bind_vertex_buffer(buffer);
			break;

		case GL_ELEMENT_ARRAY_BUFFER:
			bind_index_buffer(buffer);
			break;

		default:
			assert(false); // Invalid buffer target
		}
	}

	void Context::bind_vertex_buffer(GLuint vertex_buffer)
	{
		if (m_vertex_buffer_binding.bind(vertex_buffer))
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		}
	}

	void Context::bind_index_buffer(GLuint index_buffer)
	{
		if (m_index_buffer_binding.bind(index_buffer))
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		}
	}

	void Context::bind_program(GLuint program)
	{
		if (m_program_binding.bind(program))
		{
			glUseProgram(program);
		}
	}

	void Context::bind_vertex_array(GLuint vertex_array)
	{
		m_vertex_buffer_binding.reset();
		m_index_buffer_binding.reset();

		if (m_vertex_array_binding.bind(vertex_array))
		{
			glBindVertexArray(vertex_array);
		}
	}

	void Context::bind_texture_unit(GLuint texture_unit)
	{
		if (m_texture_unit_binding.bind(texture_unit + 1)) // +1 because zero is reserved
		{
			glActiveTexture(GL_TEXTURE0 + texture_unit);
		}
	}

	void Context::bind_texture_2d(GLuint texture_id, GLuint texture_unit)
	{
		if (m_texture_2d_binding.bind(texture_id))
		{
			bind_texture_unit(texture_unit);
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}
	}

	void Context::bind_cubemap_texture(GLuint cubemap_texture, GLuint texture_unit)
	{
		if (m_cubemap_texture_binding.bind(cubemap_texture))
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
		}
	}

	void Context::bind_renderbuffer(GLuint renderbuffer)
	{
		if (m_renderbuffer_binding.bind(renderbuffer))
		{
			glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		}
	}

	void Context::bind_framebuffer(GLuint framebuffer)
	{
		m_read_framebuffer_binding.bind(framebuffer);
		m_draw_framebuffer_binding.bind(framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}

	void Context::bind_read_framebuffer(GLuint framebuffer)
	{
		m_read_framebuffer_binding.bind(framebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	}

	void Context::bind_draw_framebuffer(GLuint framebuffer)
	{
		m_draw_framebuffer_binding.bind(framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	}

	void Context::unbind_buffer(GLenum target)
	{
		switch (target)
		{
		case GL_ARRAY_BUFFER:
			unbind_vertex_buffer();
			break;

		case GL_ELEMENT_ARRAY_BUFFER:
			unbind_index_buffer();
			break;

		default:
			assert(false); // Invalid buffer target
		}
	}

	void Context::unbind_vertex_buffer()
	{
		m_vertex_buffer_binding.unbind();
	}

	void Context::unbind_index_buffer()
	{
		m_index_buffer_binding.unbind();
	}

	void Context::unbind_program()
	{
		m_program_binding.unbind();
	}

	void Context::unbind_vertex_array()
	{
		m_vertex_array_binding.unbind();
	}

	void Context::unbind_texture_unit()
	{
		m_texture_unit_binding.unbind();
	}

	void Context::unbind_texture_2d()
	{
		unbind_texture_unit();
		m_texture_2d_binding.unbind(true);
	}

	void Context::unbind_cubemap_texture()
	{
		unbind_texture_unit();
		m_cubemap_texture_binding.unbind(true);
	}

	void Context::unbind_renderbuffer()
	{
		m_renderbuffer_binding.unbind(true);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Context::unbind_framebuffer()
	{
		m_read_framebuffer_binding.unbind(true);
		m_draw_framebuffer_binding.unbind(true);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Context::unbind_read_framebuffer()
	{
		m_read_framebuffer_binding.unbind(true);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void Context::unbind_draw_framebuffer()
	{
		m_draw_framebuffer_binding.unbind(true);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
}