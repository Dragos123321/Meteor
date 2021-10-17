#pragma once

#include "ball.h"
#include "brick.h"
#include "paddle.h"

#include <vector>

enum class UpdateResult
{
	Win,
	Lose,
	None
};

class UpdateSystem
{
public:
	UpdateResult update(std::vector<Brick>& bricks, std::vector<Ball>& balls, Paddle& paddle);

private:
	UpdateResult collide_ball_with_bricks(Ball& ball, std::vector<Brick>& bricks);
	void collide_ball_with_paddle(Ball& ball, const Paddle& paddle);
	UpdateResult collide_ball_with_walls(Ball& ball);

	bool collide_ball_with_brick(Ball& ball, const Brick& brick);
};