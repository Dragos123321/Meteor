#include "meteor/graphics/text.h"
#include "meteor/gui/font.h"

#include <algorithm>

namespace mtr
{
	const float Text::MIN_EDGE = 0.001f;

	Text::Text(std::string_view text, const Font& font, const glm::vec2& position) :
		m_text{ text },
		m_font{ &font },
		m_position{ position }
	{}

	void Text::set_text(std::string_view text)
	{
		m_text = text;
	}

	void Text::set_style(const ColorRgbF& color, float width, float edge)
	{
		m_color = color;
		m_width = std::clamp(width, 0.f, 1.f);
		m_edge = std::clamp(edge, MIN_EDGE, 1.f);
	}

	void Text::set_border_style(const ColorRgbF& border_color, float border_width, float border_edge, const glm::vec2& border_offset)
	{
		m_border_color = border_color;
		m_border_width = std::clamp(border_width, 0.f, 1.f);
		m_border_edge = std::clamp(border_edge, MIN_EDGE, 1.f);
		m_border_offset.x = std::clamp(border_offset.x, 0.f, 1.f);
		m_border_offset.y = std::clamp(border_offset.y, 0.f, 1.f);
	}

	void Text::set_position(const glm::vec2& position)
	{
		m_position = position;
	}

	void Text::set_size(uint32_t size)
	{
		m_size = std::max<uint32_t>(size, 1);
	}

	void Text::center_inside(const FloatRect& bounds)
	{
		glm::vec2 size = compute_size();
		m_position.x = bounds.x + (bounds.w - size.x) / 2.f;
		m_position.y = bounds.y + (bounds.h - size.y) / 2.f;
	}

	std::string_view Text::text() const
	{
		return m_text;
	}

	const Font& Text::font() const
	{
		return *m_font;
	}

	const glm::vec2& Text::position() const
	{
		return m_position;
	}

	const ColorRgbF& Text::color() const
	{
		return m_color;
	}

	float Text::width() const
	{
		return m_width;
	}

	float Text::edge() const
	{
		return m_edge;
	}

	const ColorRgbF& Text::border_color() const
	{
		return m_border_color;
	}

	float Text::border_width() const
	{
		return m_border_width;
	}

	float Text::border_edge() const
	{
		return m_border_edge;
	}

	const glm::vec2& Text::border_offset() const
	{
		return m_border_offset;
	}

	glm::vec2 Text::normalized_border_offset() const
	{
		return {
			m_border_offset.x / static_cast<float>(m_font->texture().width()),
			m_border_offset.y / static_cast<float>(m_font->texture().height())
		};
	}

	uint32_t Text::size() const
	{
		return m_size;
	}

	glm::vec2 Text::compute_size() const
	{
		float max_width = 0.f;
		float width = 0.f;
		float height = m_font->metrics().size;
		uint32_t prev_char = '\0';

		for (uint32_t curr_char : m_text)
		{
			if (curr_char == '\n')
			{
				max_width = std::max(width, max_width);
				height += m_font->metrics().line_height;

				width = 0.f;
			}
			else
			{
				width += m_font->glyphs().at(curr_char).advance;

				auto kerning_it = m_font->kernings().find({ prev_char, curr_char });

				if (kerning_it != m_font->kernings().end())
				{
					width += kerning_it->second;
				}
			}
			
			prev_char = curr_char;
		}

		float scale = m_size / m_font->metrics().size;
		max_width = std::max(width, max_width);

		return { max_width * scale, height * scale };
	}

	FloatRect Text::compute_bounds() const
	{
		glm::vec2 size = compute_size();
		return { m_position.x, m_position.y, size.x, size.y };
	}
}