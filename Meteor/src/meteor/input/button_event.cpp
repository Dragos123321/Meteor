#include "meteor/input/button_event.h"

namespace mtr
{
	/*
		ButtonPressEvent
	*/
	ButtonPressEvent::ButtonPressEvent(MouseButton button, const glm::vec2& position, Modifiers mods) :
		button{ button },
		mods{ mods },
		position{ position }
	{}

	/*
		ButtonReleaseEvent
	*/
	ButtonReleaseEvent::ButtonReleaseEvent(MouseButton button, const glm::vec2& position, Modifiers mods) :
		button{ button },
		mods{ mods },
		position{ position }
	{}
}