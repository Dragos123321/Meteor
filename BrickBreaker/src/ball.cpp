#include "ball.h"
#include "consts.h"

Ball::Ball(const glm::vec2& position) :
	bounds{ position.x, position.y, game::BALL_SIZE, game::BALL_SIZE },
	old_bounds{ position.x, position.y, game::BALL_SIZE, game::BALL_SIZE },
	direction{ 0.f, -1.f }
{}
