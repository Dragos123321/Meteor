#pragma once

#include <glm/trigonometric.hpp>

namespace game
{
	constexpr float ARENA_WIDTH = 448.f;
	constexpr float ARENA_HEIGHT = 640.f;
	constexpr float BRICK_WIDTH = 64.f;
	constexpr float BRICK_HEIGHT = 40.f;
	constexpr float PADDLE_WIDTH = 88.f;
	constexpr float PADDLE_HEIGHT = 20.f;
	constexpr float PADDLE_SPEED = 5.f;
	constexpr float BALL_SIZE = 20.f;
	constexpr float BALL_SPEED = 6.f;
	constexpr float BALL_MAX_REFLECTION_ANGLE = glm::radians(25.f);
}