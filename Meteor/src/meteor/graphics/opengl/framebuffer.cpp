#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/framebuffer.h"

namespace mtr::gl
{
	/*
		Framebuffer
	*/
	Framebuffer::Framebuffer(int width, int height, const TextureSettings& texture_settings) :
		m_color_texture{ width, height, texture_settings },
		m_renderbuffer{ width, height }
	{
		glGenFramebuffers(1, &m_id);
		FramebufferBinder framebuffer_binder(*this);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture.id(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer.id());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw IncompleteFramebufferError(m_id);
		}
	}

	Framebuffer::Framebuffer(Framebuffer&& other) noexcept :
		m_id{ other.m_id },
		m_color_texture{ std::move(other.m_color_texture) },
		m_renderbuffer{ std::move(other.m_renderbuffer) }
	{
		other.m_id = 0;
	}

	Framebuffer::~Framebuffer()
	{
		dispose();
	}

	Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		m_color_texture = std::move(other.m_color_texture);
		m_renderbuffer = std::move(other.m_renderbuffer);

		other.m_id = 0;

		return *this;
	}

	GLuint Framebuffer::id() const
	{
		return m_id;
	}

	bool Framebuffer::valid() const
	{
		return m_id;
	}

	const Texture2D& Framebuffer::color_texture() const
	{
		return m_color_texture;
	}

	void Framebuffer::dispose()
	{
		if (m_id) glDeleteFramebuffers(1, &m_id);
	}

	/*
		FramebufferBinder
	*/
	FramebufferBinder::FramebufferBinder(Framebuffer& framebuffer, FramebufferBindingMode binding_mode) :
		m_framebuffer{ &framebuffer },
		m_binding_mode{ binding_mode }
	{
		switch (m_binding_mode)
		{
		case FramebufferBindingMode::Read:
			CONTEXT->bind_read_framebuffer(framebuffer.id());
			break;

		case FramebufferBindingMode::Draw:
			CONTEXT->bind_draw_framebuffer(framebuffer.id());
			break;

		case FramebufferBindingMode::ReadAndDraw:
			CONTEXT->bind_framebuffer(framebuffer.id());
			break;
		}
	}

	FramebufferBinder::~FramebufferBinder()
	{
		switch (m_binding_mode)
		{
		case FramebufferBindingMode::Read:
			CONTEXT->unbind_read_framebuffer();
			break;

		case FramebufferBindingMode::Draw:
			CONTEXT->unbind_draw_framebuffer();
			break;

		case FramebufferBindingMode::ReadAndDraw:
			CONTEXT->unbind_framebuffer();
			break;
		}
	}

	void FramebufferBinder::resize(uint32_t width, uint32_t height)
	{
		Texture2DBinder(m_framebuffer->m_color_texture).resize(width, height);
		RenderbufferBinder(m_framebuffer->m_renderbuffer).resize(width, height);
	}

	/*
		IncompleteFramebufferError
	*/
	IncompleteFramebufferError::IncompleteFramebufferError(GLuint framebuffer_id) :
		std::runtime_error{ "framebuffer is incomplete" },
		m_framebuffer_id{ framebuffer_id }
	{}

	GLuint IncompleteFramebufferError::framebuffer_id() const
	{
		return m_framebuffer_id;
	}
}