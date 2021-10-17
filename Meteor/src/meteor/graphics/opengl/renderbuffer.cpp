#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/renderbuffer.h"

namespace mtr::gl
{
	/*
		Renderbuffer
	*/
	Renderbuffer::Renderbuffer(uint32_t width, uint32_t height)
	{
		glGenRenderbuffers(1, &m_id);

		RenderbufferBinder renderbuffer_binder(*this);
		renderbuffer_binder.resize(width, height);
	}

	Renderbuffer::Renderbuffer(Renderbuffer&& other) noexcept :
		m_id{ other.m_id },
		m_width{ other.m_width },
		m_height{ other.m_height }
	{
		other.m_id = 0;
		other.m_width = 0;
		other.m_height = 0;
	}

	Renderbuffer::~Renderbuffer()
	{
		dispose();
	}

	Renderbuffer& Renderbuffer::operator=(Renderbuffer&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		m_width = other.m_width;
		m_height = other.m_height;

		other.m_id = 0;
		other.m_width = 0;
		other.m_height = 0;

		return *this;
	}

	GLuint Renderbuffer::id() const
	{
		return m_id;
	}

	bool Renderbuffer::valid() const
	{
		return m_id;
	}

	uint32_t Renderbuffer::width() const
	{
		return m_width;
	}

	uint32_t Renderbuffer::height() const
	{
		return m_height;
	}

	void Renderbuffer::dispose()
	{
		if (m_id) glDeleteRenderbuffers(1, &m_id);
	}

	/*
		RenderbufferBinder
	*/
	RenderbufferBinder::RenderbufferBinder(Renderbuffer& renderbuffer) :
		m_renderbuffer{ &renderbuffer }
	{

		CONTEXT->bind_renderbuffer(renderbuffer.id());
	}

	RenderbufferBinder::~RenderbufferBinder()
	{
		CONTEXT->unbind_renderbuffer();
	}

	void RenderbufferBinder::resize(uint32_t width, uint32_t height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			static_cast<GLsizei>(width), static_cast<GLsizei>(height));

		m_renderbuffer->m_width = width;
		m_renderbuffer->m_height = height;
	}
}