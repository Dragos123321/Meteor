#pragma once

#include <glm/glm.hpp>

namespace mtr
{
	enum class ScrollDirection
	{
		None,
		Clockwise,
		CounterClockwise
	};

	struct ScrollEvent
	{
		explicit ScrollEvent(const glm::vec2& offset);

		ScrollDirection direction_x() const;
		ScrollDirection direction_y() const;

		glm::vec2 offset;
	};
}
