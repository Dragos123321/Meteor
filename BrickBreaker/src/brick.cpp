#include "consts.h"
#include "brick.h"

Brick::Brick(int grid_x, int grid_y, const mtr::ColorRgba& color) :
	bounds{ grid_x * game::BRICK_WIDTH, grid_y * game::BRICK_HEIGHT, game::BRICK_WIDTH, game::BRICK_HEIGHT },
	color{ color }
{}
