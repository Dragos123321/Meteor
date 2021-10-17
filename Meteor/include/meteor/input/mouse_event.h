#pragma once

#include <glm/vec2.hpp>

namespace mtr
{
	struct MouseMoveEvent
	{
	public:
		explicit MouseMoveEvent(const glm::vec2& position);

		glm::vec2 position;
	};
}