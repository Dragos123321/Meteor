#pragma once

#include "meteor/graphics/color.h"

namespace mtr
{
	struct WidgetStyle
	{
		constexpr WidgetStyle() = default;
		constexpr WidgetStyle(const ColorRgba& color, const ColorRgba& hover_color, float corner_radius = 0.f);

		ColorRgba color;
		ColorRgba hover_color;
		float corner_radius = 0.f;
	};

#pragma region Implementation
	constexpr WidgetStyle::WidgetStyle(const ColorRgba& color, const ColorRgba& hover_color, float corner_radius) :
		color{ color },
		hover_color{ hover_color },
		corner_radius{ corner_radius }
	{}
#pragma endregion

	constexpr WidgetStyle WIDGET_STYLE_PANEL = WidgetStyle(COLOR_TRANSPARENT, COLOR_TRANSPARENT);
	constexpr WidgetStyle WIDGET_STYLE_LIGHT = WidgetStyle(ColorRgb(220, 220, 220), ColorRgb(250, 250, 250), 16.f);
}