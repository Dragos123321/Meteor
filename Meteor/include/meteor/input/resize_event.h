#pragma once

#include <glm/glm.hpp>

namespace mtr
{
	struct ResizeEvent
	{
		explicit ResizeEvent(const glm::uvec2& size);

		glm::uvec2 size;
	};

	struct FrameResizeEvent
	{
		explicit FrameResizeEvent(const glm::uvec2& size);

		glm::uvec2 size;
	};
}
