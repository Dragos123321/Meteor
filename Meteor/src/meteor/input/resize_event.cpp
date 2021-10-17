#include "meteor/input/resize_event.h"

namespace mtr
{
	/*
		ResizeEvent
	*/
	ResizeEvent::ResizeEvent(const glm::uvec2& size) :
		size{ size }
	{}

	/*
		FramebufferResizeEvent
	*/
	FrameResizeEvent::FrameResizeEvent(const glm::uvec2& size) :
		size{ size }
	{}
}