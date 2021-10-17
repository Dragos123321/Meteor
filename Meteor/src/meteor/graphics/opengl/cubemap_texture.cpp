#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/cubemap_texture.h"

namespace mtr::gl
{
	/*
		CubemapTexture
	*/
	CubemapTexture::CubemapTexture(const CubemapImages& images, const TextureSettings& settings, bool generate_mipmap)
	{
		glGenTextures(1, &m_id);
		CubemapTextureBinder binder(*this);

		auto set_image = [&settings](GLenum target, const Image& image) {
			glTexImage2D(target, 0, static_cast<GLint>(settings.format), 
				image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		};

		set_image(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, images.left_image);
		set_image(GL_TEXTURE_CUBE_MAP_POSITIVE_X, images.right_image);
		set_image(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, images.top_image);
		set_image(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, images.bottom_image);
		set_image(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, images.back_image);
		set_image(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, images.front_image);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLint>(settings.wrap_s));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLint>(settings.wrap_t));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(settings.wrap_r));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(settings.mag_filter));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(settings.min_filter));

		if (generate_mipmap)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	CubemapTexture::CubemapTexture(CubemapTexture&& other) noexcept :
		m_id{ other.m_id }
	{
		other.m_id = 0;
	}

	CubemapTexture::~CubemapTexture()
	{
		dispose();
	}

	CubemapTexture& CubemapTexture::operator=(CubemapTexture&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		other.m_id = 0;

		return *this;
	}

	GLuint CubemapTexture::id() const
	{
		return m_id;
	}

	bool CubemapTexture::valid() const
	{
		return m_id;
	}

	void CubemapTexture::dispose()
	{
		if (m_id) glDeleteTextures(1, &m_id);
	}

	/*
		CubemapTextureBinder
	*/
	CubemapTextureBinder::CubemapTextureBinder(const CubemapTexture& texture) :
		m_texture{ &texture }
	{
		CONTEXT->bind_cubemap_texture(texture.id());
	}

	CubemapTextureBinder::~CubemapTextureBinder()
	{
		CONTEXT->unbind_cubemap_texture();
	}

	void CubemapTextureBinder::generate_mipmap()
	{
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
}