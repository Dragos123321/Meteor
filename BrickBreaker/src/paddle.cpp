#include "consts.h"
#include "paddle.h"

Paddle::Paddle()
{
	bounds.x = (game::ARENA_WIDTH - game::PADDLE_WIDTH) / 2.f;
	bounds.y = game::ARENA_HEIGHT - game::PADDLE_HEIGHT;
	bounds.w = game::PADDLE_WIDTH;
	bounds.h = game::PADDLE_HEIGHT;
}
