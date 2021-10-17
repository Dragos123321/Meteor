#include "meteor/input/scroll_event.h"

namespace mtr
{
	ScrollEvent::ScrollEvent(const glm::vec2& offset) :
		offset{ offset }
	{}

	ScrollDirection ScrollEvent::direction_x() const
	{
		if (offset.x < 0.f) return ScrollDirection::CounterClockwise;
		if (offset.x > 0.f) return ScrollDirection::Clockwise;
		return ScrollDirection::None;
	}

	ScrollDirection ScrollEvent::direction_y() const
	{
		if (offset.y < 0.f) return ScrollDirection::CounterClockwise;
		if (offset.y > 0.f) return ScrollDirection::Clockwise;
		return ScrollDirection::None;
	}
}
