#include "meteor/graphics/image.h"
#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/texture.h"
#include "meteor/graphics/opengl/texture_2d.h"

namespace mtr::gl
{
	/*
		Texture2D
	*/
	Texture2D::Texture2D(uint32_t width, uint32_t height, const TextureSettings& settings) :
		m_width{ width },
		m_height{ height },
		m_settings{ settings }
	{
		glGenTextures(1, &m_id);
		Texture2DBinder binder(*this);

		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(settings.format), 
			width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrap_s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrap_t));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag_filter));
	}

	Texture2D::Texture2D(const Image& image, const TextureSettings& settings, bool generate_mipmap) :
		m_width{ image.width() },
		m_height{ image.height() }
	{
		glGenTextures(1, &m_id);
		Texture2DBinder binder(*this);

		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(settings.format),
			m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrap_s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrap_t));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag_filter));

		if (generate_mipmap)
			binder.generate_mipmap();
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept :
		m_id{ other.m_id },
		m_width{ other.m_width },
		m_height{ other.m_height }
	{
		other.m_id = 0;
		other.m_width = 0;
		other.m_height = 0;
	}

	Texture2D::~Texture2D()
	{
		dispose();
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
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

	GLuint Texture2D::id() const
	{
		return m_id;
	}

	bool Texture2D::valid() const
	{
		return m_id;
	}

	uint32_t Texture2D::width() const
	{
		return m_width;
	}

	uint32_t Texture2D::height() const
	{
		return m_height;
	}

	void Texture2D::dispose()
	{
		if (m_id) glDeleteTextures(1, &m_id);
	}

	/*
		Texture2DBinder
	*/
	Texture2DBinder::Texture2DBinder(Texture2D& texture_2d, GLuint texture_unit) :
		m_texture_2d{ &texture_2d }
	{
		CONTEXT->bind_texture_2d(texture_2d.id(), texture_unit);
	}

	Texture2DBinder::~Texture2DBinder()
	{
		CONTEXT->unbind_texture_2d();
	}

	void Texture2DBinder::resize(uint32_t width, uint32_t height)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_texture_2d->m_settings.format),
			static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		m_texture_2d->m_width = width;
		m_texture_2d->m_height = height;
	}

	void Texture2DBinder::generate_mipmap()
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	/*
		ConstTexture2DBinder
	*/
	ConstTexture2DBinder::ConstTexture2DBinder(const Texture2D& texture_2d, GLuint texture_unit) :
		m_texture_2d{ &texture_2d }
	{
		CONTEXT->bind_texture_2d(texture_2d.id(), texture_unit);
	}

	ConstTexture2DBinder::~ConstTexture2DBinder()
	{
		CONTEXT->unbind_texture_2d();
	}
}