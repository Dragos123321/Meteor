#pragma once

#include "meteor/graphics/color.h"
#include "meteor/math/rect.h"

#include <string>

namespace mtr
{
	class Font;

	class Text
	{
	public:
		static const float MIN_EDGE;

		Text(std::string_view text, const Font& font, const glm::vec2& position = { 0.f, 0.f });

		void set_text(std::string_view text);
		void set_style(const ColorRgbF& color, float width, float edge);
		void set_border_style(const ColorRgbF& border_color, float border_width, 
			float border_edge, const glm::vec2& border_offset = { 0.f, 0.f });
		void set_position(const glm::vec2& position);
		void set_size(uint32_t size);
		void center_inside(const FloatRect& bounds);

		std::string_view text() const;
		const Font& font() const;
		const glm::vec2& position() const;
		const ColorRgbF& color() const;
		float width() const;
		float edge() const;
		const ColorRgbF& border_color() const;
		float border_width() const;
		float border_edge() const;
		const glm::vec2& border_offset() const;
		glm::vec2 normalized_border_offset() const;
		uint32_t size() const;

		glm::vec2 compute_size() const;
		FloatRect compute_bounds() const;

	private:
		std::string_view m_text;
		const Font* m_font = nullptr;
		glm::vec2 m_position = { 0.f, 0.f };
		ColorRgbF m_color = COLOR_WHITE;
		float m_width = 0.5f;
		float m_edge = 0.08f;
		ColorRgbF m_border_color = COLOR_WHITE;
		float m_border_width = 0.f;
		float m_border_edge = MIN_EDGE;
		glm::vec2 m_border_offset = { 0.f, 0.f };
		uint32_t m_size = 32;
	};
}