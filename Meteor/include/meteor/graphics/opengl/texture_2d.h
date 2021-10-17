#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/opengl/texture.h"

#include <iostream>

namespace mtr
{
	class Image;
}

namespace mtr::gl
{
	class Texture2DBinder;
	struct TextureSettings;

	class Texture2D : NonCopyable
	{
		friend class Texture2DBinder;

	public:
		Texture2D() = default;
		Texture2D(uint32_t width, uint32_t height, const TextureSettings& settings);
		Texture2D(const Image& image, const TextureSettings& settings, bool generate_mipmap = true);
		Texture2D(Texture2D&& other) noexcept;
		~Texture2D();

		Texture2D& operator=(Texture2D&& other) noexcept;

		GLuint id() const;
		bool valid() const;
		uint32_t width() const;
		uint32_t height() const;

	private:
		void dispose();

		GLuint m_id = 0;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		TextureSettings m_settings;
	};

	class Texture2DBinder : NonCopyable, NonMovable
	{
	public:
		explicit Texture2DBinder(Texture2D& texture_2d, GLuint texture_unit = 0);
		~Texture2DBinder();

		void resize(uint32_t width, uint32_t height);
		void generate_mipmap();

	private:
		Texture2D* m_texture_2d = nullptr;
	};

	class ConstTexture2DBinder : NonCopyable, NonMovable
	{
	public:
		explicit ConstTexture2DBinder(const Texture2D& texture_2d, GLuint texture_unit = 0);
		~ConstTexture2DBinder();

	private:
		const Texture2D* m_texture_2d = nullptr;
	};
}
