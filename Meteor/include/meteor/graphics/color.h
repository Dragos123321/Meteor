#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

namespace mtr
{
	struct ColorRgba;
	struct ColorRgbF;
	struct ColorRgbaF;

	struct ColorRgb
	{
		constexpr ColorRgb() = default;
		constexpr ColorRgb(uint8_t r, uint8_t g, uint8_t b);
		explicit constexpr ColorRgb(const ColorRgba& color);
		constexpr ColorRgb(const ColorRgbF& color);
		explicit constexpr ColorRgb(const ColorRgbaF& color);

		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
	};

	struct ColorRgba
	{
		constexpr ColorRgba() = default;
		constexpr ColorRgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
		constexpr ColorRgba(const ColorRgb& color);
		constexpr ColorRgba(const ColorRgbF& color);
		constexpr ColorRgba(const ColorRgbaF& color);

		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
		uint8_t a = 255;
	};

	struct ColorRgbF
	{
		constexpr ColorRgbF() = default;
		constexpr ColorRgbF(float r, float g, float b);
		constexpr ColorRgbF(const ColorRgb& color);
		explicit constexpr ColorRgbF(const ColorRgba& color);
		explicit constexpr ColorRgbF(const ColorRgbaF& color);

		const glm::vec3& as_vec3() const;

		float r = 1.f;
		float g = 1.f;
		float b = 1.f;
	};

	struct ColorRgbaF
	{
		constexpr ColorRgbaF() = default;
		constexpr ColorRgbaF(float r, float g, float b, float a = 255);
		constexpr ColorRgbaF(const ColorRgb& color);
		constexpr ColorRgbaF(const ColorRgba& color);
		constexpr ColorRgbaF(const ColorRgbF& color);

		const glm::vec4& as_vec4() const;

		float r = 1.f;
		float g = 1.f;
		float b = 1.f;
		float a = 1.f;
	};

#pragma region Implementation
	/*
		ColorRgb
	*/
	constexpr ColorRgb::ColorRgb(uint8_t r, uint8_t g, uint8_t b) :
		r{ r }, g{ g }, b{ b }
	{}

	constexpr ColorRgb::ColorRgb(const ColorRgba& color) :
		r{ color.r },
		g{ color.g },
		b{ color.b }
	{}

	constexpr ColorRgb::ColorRgb(const ColorRgbF& color) :
		r{ static_cast<uint8_t>(color.r * 255.f) },
		g{ static_cast<uint8_t>(color.g * 255.f) },
		b{ static_cast<uint8_t>(color.b * 255.f) }
	{}

	constexpr ColorRgb::ColorRgb(const ColorRgbaF& color) :
		r{ static_cast<uint8_t>(color.r * 255.f) },
		g{ static_cast<uint8_t>(color.g * 255.f) },
		b{ static_cast<uint8_t>(color.b * 255.f) }
	{}

	/*
		ColorRgba
	*/
	constexpr ColorRgba::ColorRgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
		r{ r }, g{ g }, b{ b }, a{ a }
	{}

	constexpr ColorRgba::ColorRgba(const ColorRgb& color) :
		r{ color.r },
		g{ color.g },
		b{ color.b }
	{}

	constexpr ColorRgba::ColorRgba(const ColorRgbF& color) :
		r{ static_cast<uint8_t>(color.r * 255.f) },
		g{ static_cast<uint8_t>(color.g * 255.f) },
		b{ static_cast<uint8_t>(color.b * 255.f) }
	{}

	constexpr ColorRgba::ColorRgba(const ColorRgbaF& color) :
		r{ static_cast<uint8_t>(color.r * 255.f) },
		g{ static_cast<uint8_t>(color.g * 255.f) },
		b{ static_cast<uint8_t>(color.b * 255.f) },
		a{ static_cast<uint8_t>(color.a * 255.f) }
	{}

	/*
		ColorRgbF
	*/
	constexpr ColorRgbF::ColorRgbF(float r, float g, float b) :
		r{ r }, g{ g }, b{ b }
	{}

	constexpr ColorRgbF::ColorRgbF(const ColorRgb& color) :
		r{ static_cast<float>(color.r) / 255.f },
		g{ static_cast<float>(color.g) / 255.f },
		b{ static_cast<float>(color.b) / 255.f }
	{}

	constexpr ColorRgbF::ColorRgbF(const ColorRgba& color) :
		r{ static_cast<float>(color.r) / 255.f },
		g{ static_cast<float>(color.g) / 255.f },
		b{ static_cast<float>(color.b) / 255.f }
	{}

	constexpr ColorRgbF::ColorRgbF(const ColorRgbaF& color) :
		r{ color.r },
		g{ color.g },
		b{ color.b }
	{}

	inline const glm::vec3& ColorRgbF::as_vec3() const
	{
		return reinterpret_cast<const glm::vec3&>(*this);
	}

	/*
		ColorRgbaF
	*/
	constexpr ColorRgbaF::ColorRgbaF(float r, float g, float b, float a) :
		r{ r }, g{ g }, b{ b }, a{ a }
	{}

	constexpr ColorRgbaF::ColorRgbaF(const ColorRgb& color) :
		r{ static_cast<float>(color.r) / 255.f },
		g{ static_cast<float>(color.g) / 255.f },
		b{ static_cast<float>(color.b) / 255.f }
	{}

	constexpr ColorRgbaF::ColorRgbaF(const ColorRgba& color) :
		r{ static_cast<float>(color.r) / 255.f },
		g{ static_cast<float>(color.g) / 255.f },
		b{ static_cast<float>(color.b) / 255.f },
		a{ static_cast<float>(color.a) / 255.f }
	{}

	constexpr ColorRgbaF::ColorRgbaF(const ColorRgbF& color) :
		r{ color.r },
		g{ color.g },
		b{ color.b }
	{}

	inline const glm::vec4& ColorRgbaF::as_vec4() const
	{
		return reinterpret_cast<const glm::vec4&>(*this);
	}
#pragma endregion

	constexpr ColorRgb COLOR_WHITE = ColorRgb(255, 255, 255);
	constexpr ColorRgb COLOR_BLACK = ColorRgb(0, 0, 0);
	constexpr ColorRgb COLOR_RED = ColorRgb(255, 0, 0);
	constexpr ColorRgb COLOR_GREEN = ColorRgb(0, 255, 0);
	constexpr ColorRgb COLOR_BLUE = ColorRgb(0, 0, 255);

	constexpr ColorRgba COLOR_TRANSPARENT = ColorRgba(255, 255, 255, 0);
}
