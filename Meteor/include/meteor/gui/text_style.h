#pragma once

#include "meteor/graphics/color.h"

#include <glm/vec2.hpp>

namespace mtr
{
	struct TextStyle
	{
		ColorRgb color = ColorRgb(20, 20, 20);
		ColorRgb hover_color = ColorRgb(20, 20, 20);
		ColorRgb border_color = ColorRgb(20, 20, 20);
		ColorRgb border_hover_color = ColorRgb(20, 20, 20);
		float width = 0.425f;
		float edge = 0.175f;
		float border_width = 0.f;
		float border_edge = 0.1f;
		glm::vec2 border_offset = { 0.f, 0.f };
		uint32_t size = 20;
	};
}
