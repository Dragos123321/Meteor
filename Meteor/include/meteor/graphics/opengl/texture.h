#pragma once

#include <glad/glad.h>

#include <string>

namespace mtr::gl
{
	enum class TextureFormat
	{
		Red = GL_RED,
		Rgb = GL_RGB,
		Rgba = GL_RGBA,
		SRgb = GL_SRGB,
		SRgba = GL_SRGB_ALPHA,
		Rgb16F = GL_RGB16F,
		Rgb32F = GL_RGB32F,
		Rgba16F = GL_RGBA16F,
		Rgba32F = GL_RGBA32F
	};

	enum class TextureWrap
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT,
		MirroredClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
	};

	enum class TextureMinFilter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class TextureMagFilter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	struct TextureSettings
	{
		constexpr TextureSettings() = default;
		constexpr TextureSettings(TextureFormat format, TextureWrap wrap_s, TextureWrap wrap_t,
			TextureWrap wrap_r, TextureMinFilter min_filter, TextureMagFilter mag_filter);

		TextureFormat format = TextureFormat::Rgba;
		TextureWrap wrap_s = TextureWrap::ClampToBorder;
		TextureWrap wrap_t = TextureWrap::ClampToBorder;
		TextureWrap wrap_r = TextureWrap::ClampToBorder;
		TextureMinFilter min_filter = TextureMinFilter::LinearMipmapLinear;
		TextureMagFilter mag_filter = TextureMagFilter::Linear;
	};

	enum class TextureType
	{
		None,
		Diffuse,
		Specular,
		NormalMap,
		HeightMap
	};

#pragma region Implementation
	constexpr TextureSettings::TextureSettings(TextureFormat format, TextureWrap wrap_s, TextureWrap wrap_t,
		TextureWrap wrap_r, TextureMinFilter min_filter, TextureMagFilter mag_filter) :

		format{ format },
		wrap_s{ wrap_s },
		wrap_t{ wrap_t },
		wrap_r{ wrap_r },
		min_filter{ min_filter },
		mag_filter{ mag_filter }
	{}
#pragma endregion
}
