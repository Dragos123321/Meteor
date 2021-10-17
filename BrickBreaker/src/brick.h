#pragma once

#include <meteor/graphics/color.h>
#include <meteor/math/rect.h>

struct Brick
{
	Brick(int grid_x, int grid_y, const mtr::ColorRgba& color);

	mtr::FloatRect bounds;
	mtr::ColorRgba color;
};