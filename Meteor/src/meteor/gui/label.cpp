#include "meteor/graphics/text.h"
#include "meteor/graphics/renderers/gui_renderer.h"
#include "meteor/gui/label.h"

namespace mtr
{
	Label::Label(std::string_view text, const Font& font, const WidgetStyle& style) :
		Widget{ style },
		m_text{ text },
		m_font{ &font }
	{}

	void Label::draw(GuiRenderer& gui_renderer, WidgetState state) const
	{
		Text text(m_text, *m_font);
		text.set_size(m_text_style.size);
		text.center_inside(m_bounds);

		switch (state)
		{
		case WidgetState::None:
			text.set_style(m_text_style.color, m_text_style.width, m_text_style.edge);
			text.set_border_style(m_text_style.border_color, m_text_style.border_width, m_text_style.border_edge);

			gui_renderer.draw_rounded_rect(m_bounds, m_style.color, m_style.corner_radius);
			gui_renderer.draw_text(text);
			break;

		case WidgetState::Hovered:
			text.set_style(m_text_style.hover_color, m_text_style.width, m_text_style.edge);
			text.set_border_style(m_text_style.border_hover_color, m_text_style.border_width, m_text_style.border_edge);

			gui_renderer.draw_rounded_rect(m_bounds, m_style.hover_color, m_style.corner_radius);
			gui_renderer.draw_text(text);
			break;
		}
	}

	void Label::set_text(std::string_view text)
	{
		m_text = text;
	}

	void Label::set_text_style(const TextStyle& text_style)
	{
		m_text_style = text_style;
	}

	const std::string& Label::text() const
	{
		return m_text;
	}

	const TextStyle& Label::text_style() const
	{
		return m_text_style;
	}
}