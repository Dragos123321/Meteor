#pragma once

#include "meteor/gui/label.h"

namespace mtr
{
	class Button : public Label
	{
	public:
		Button(std::string_view text, const Font& font, const WidgetStyle& style = WIDGET_STYLE_LIGHT);
	};
}
