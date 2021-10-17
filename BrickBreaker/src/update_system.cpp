#include "consts.h"
#include "update_system.h"

#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>

UpdateResult UpdateSystem::update(std::vector<Brick>& bricks, std::vector<Ball>& balls, Paddle& paddle)
{
	UpdateResult result;

	for (size_t i = 0; i != balls.size(); ++i)
	{
		auto& ball = balls[i];
		ball.bounds.x += ball.direction.x * game::BALL_SPEED;
		ball.bounds.y += ball.direction.y * game::BALL_SPEED;

		result = collide_ball_with_bricks(ball, bricks);
		if (result != UpdateResult::None) return result;

		collide_ball_with_paddle(ball, paddle);

		result = collide_ball_with_walls(ball);
		if (result != UpdateResult::None) return result;

		ball.old_bounds = ball.bounds;
	}

	return UpdateResult::None;
}

UpdateResult UpdateSystem::collide_ball_with_bricks(Ball& ball, std::vector<Brick>& bricks)
{
	for (size_t i = 0; i < bricks.size();)
	{
		if (collide_ball_with_brick(ball, bricks[i]))
		{
			bricks[i] = bricks.back();
			bricks.pop_back();
		}
		else
		{
			++i;
		}
	}

	return bricks.empty() ? UpdateResult::Win : UpdateResult::None;
}

void UpdateSystem::collide_ball_with_paddle(Ball& ball, const Paddle& paddle)
{
	if (ball.bounds.intersects(paddle.bounds))
	{
		float ball_center = ball.bounds.x + ball.bounds.w / 2.f;
		float paddle_center = paddle.bounds.x + paddle.bounds.w / 2.f;
		float distance = ball_center - paddle_center;
		float angle_scale = std::clamp(distance, -paddle.bounds.w / 2.f, paddle.bounds.w / 2.f) / (paddle.bounds.w / 2.f);

		ball.bounds.set_bottom(paddle.bounds.y - 0.1f);
		ball.direction = glm::rotate(glm::vec2(ball.direction.x, -std::abs(ball.direction.y)), 
			game::BALL_MAX_REFLECTION_ANGLE * angle_scale);
	}
}

UpdateResult UpdateSystem::collide_ball_with_walls(Ball& ball)
{
	if (ball.bounds.top() > game::ARENA_HEIGHT) return UpdateResult::Lose;

	if (ball.bounds.x < 0.f)
	{
		ball.bounds.x = 0.1f;
		ball.direction.x = std::abs(ball.direction.x);
	}
	else if (ball.bounds.right() > game::ARENA_WIDTH)
	{
		ball.bounds.set_right(game::ARENA_WIDTH - 0.1f);
		ball.direction.x = -std::abs(ball.direction.x);
	}

	if (ball.bounds.y < 0.f)
	{
		ball.bounds.y = 0.1f;
		ball.direction.y = std::abs(ball.direction.y);
	}

	return UpdateResult::None;
}

bool UpdateSystem::collide_ball_with_brick(Ball& ball, const Brick& brick)
{
	if (!ball.bounds.intersects(brick.bounds)) return false;

	if (ball.old_bounds.right() < brick.bounds.x && ball.bounds.right() >= brick.bounds.x)
	{
		ball.bounds.set_right(brick.bounds.x - 0.1f);
		ball.direction.x = -std::abs(ball.direction.x);
	}
	else if (ball.old_bounds.x > brick.bounds.right() && ball.bounds.x <= brick.bounds.right())
	{
		ball.bounds.x = brick.bounds.right() + 0.1f;
		ball.direction.x = std::abs(ball.direction.x);
	}
	else if (ball.old_bounds.bottom() < brick.bounds.y && ball.bounds.bottom() >= brick.bounds.y)
	{
		ball.bounds.set_bottom(brick.bounds.y - 0.1f);
		ball.direction.y = -std::abs(ball.direction.y);
	}
	else if (ball.old_bounds.y > brick.bounds.bottom() && ball.bounds.y <= brick.bounds.bottom())
	{
		ball.bounds.y = brick.bounds.bottom() + 0.1f;
		ball.direction.y = std::abs(ball.direction.y);
	}

	return true;
}
