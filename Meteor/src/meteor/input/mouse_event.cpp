#include "meteor/input/mouse_event.h"

namespace mtr
{
	MouseMoveEvent::MouseMoveEvent(const glm::vec2& position) :
		position{ position }
	{}
}