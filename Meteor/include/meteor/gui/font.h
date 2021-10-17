#pragma once

#include "meteor/graphics/opengl/texture_2d.h"
#include "meteor/math/rect.h"

#include <unordered_map>

namespace mtr
{
	union KerningPair
	{
		constexpr KerningPair() = default;
		constexpr KerningPair(uint32_t first, uint32_t second);

		constexpr bool operator==(const KerningPair& other) const;

		uint64_t value = 0;
		struct
		{
			uint32_t first;
			uint32_t second;
		};
	};

#pragma region Implementation
	constexpr KerningPair::KerningPair(uint32_t first, uint32_t second) :
		first{ first },
		second{ second }
	{}

	constexpr bool KerningPair::operator==(const KerningPair& other) const
	{
		return value == other.value;
	}
#pragma endregion
}

#pragma region Implementation
namespace std
{
	template<>
	class hash<mtr::KerningPair>
	{
	public:
		size_t operator()(const mtr::KerningPair& kerning) const
		{
			return hash<uint64_t>()(kerning.value);
		}
	};
}
#pragma endregion

namespace mtr
{
	struct Glyph
	{
		Glyph() = default;
		Glyph(const FloatRect& tex_coords, const glm::vec2& offset, float advance);

		FloatRect tex_coords;
		glm::vec2 offset = { 0.f, 0.f };
		float advance = 0.f;
	};

	struct FontMetrics
	{
		constexpr FontMetrics() = default;
		constexpr FontMetrics(float size, float base, float line_height);

		constexpr FontMetrics scaled_to(float desired_size) const;

		float size = 0.f;
		float base = 0.f;
		float line_height = 0.f;
	};

	class Font
	{
	public:
		using GlyphMap = std::unordered_map<uint32_t, Glyph>;
		using KerningMap = std::unordered_map<KerningPair, float>;

		Font() = default;
		explicit Font(std::string_view path);

		const gl::Texture2D& texture() const;
		const FontMetrics& metrics() const;
		const GlyphMap& glyphs() const;
		const KerningMap& kernings() const;

	private:
		gl::Texture2D m_texture;
		FontMetrics m_metrics;
		GlyphMap m_glyphs;
		KerningMap m_kernings;
	};

#pragma region Implementation
	constexpr FontMetrics::FontMetrics(float size, float base, float line_height) :
		size{ size },
		base{ base },
		line_height{ line_height }
	{}

	constexpr FontMetrics FontMetrics::scaled_to(float desired_size) const
	{
		float scale = desired_size / size;
		return FontMetrics(size * scale, base * scale, line_height * scale);
	}
#pragma endregion
}