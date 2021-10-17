#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/image.h"
#include "meteor/graphics/opengl/texture.h"

#include <vector>

namespace mtr::gl
{
	struct CubemapImages
	{
		Image left_image;
		Image right_image;
		Image top_image;
		Image bottom_image;
		Image back_image;
		Image front_image;
	};

	class CubemapTexture : NonCopyable
	{
	public:
		CubemapTexture() = default;
		CubemapTexture(const CubemapImages& images, const TextureSettings& settings, bool generate_mipmap = true);
		CubemapTexture(CubemapTexture&& other) noexcept;
		~CubemapTexture();

		CubemapTexture& operator=(CubemapTexture&& other) noexcept;

		GLuint id() const;
		bool valid() const;

	private:
		void dispose();

		GLuint m_id = 0;
	};

	class CubemapTextureBinder : NonCopyable, NonMovable
	{
	public:
		explicit CubemapTextureBinder(const CubemapTexture& texture);
		~CubemapTextureBinder();

		void generate_mipmap();

	private:
		const CubemapTexture* m_texture = nullptr;
	};
}