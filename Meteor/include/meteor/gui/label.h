#pragma once

#include "meteor/gui/text_style.h"
#include "meteor/gui/widget.h"

namespace mtr
{
	class Font;

	class Label : public Widget
	{
	public:
		Label(std::string_view text, const Font& font, const WidgetStyle& style = WIDGET_STYLE_PANEL);

		void draw(GuiRenderer& gui_renderer, WidgetState state) const override;

		void set_text(std::string_view text);
		void set_text_style(const TextStyle& text_style);

		const std::string& text() const;
		const TextStyle& text_style() const;

	private:
		std::string m_text;
		const Font* m_font = nullptr;
		TextStyle m_text_style;
	};
}