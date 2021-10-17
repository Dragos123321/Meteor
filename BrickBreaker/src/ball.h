#pragma once

#include <meteor/math/rect.h>

struct Ball
{
	explicit Ball(const glm::vec2& position);

	mtr::FloatRect bounds;
	mtr::FloatRect old_bounds;
	glm::vec2 direction;
};