#include "meteor/gui/button.h"

namespace mtr
{
	Button::Button(std::string_view text, const Font& font, const WidgetStyle& style) :
		Label{ text, font, style }
	{}
}