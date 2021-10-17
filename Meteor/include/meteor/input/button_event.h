#pragma once

#include "meteor/input/modifiers.h"
#include "meteor/input/mouse_button.h"

#include <glm/glm.hpp>

namespace mtr
{
	struct ButtonPressEvent
	{
	public:
		explicit ButtonPressEvent(MouseButton button, const glm::vec2& position, Modifiers mods = Modifiers());

		MouseButton button;
		Modifiers mods;
		glm::vec2 position;
	};

	struct ButtonReleaseEvent
	{
	public:
		explicit ButtonReleaseEvent(MouseButton button, const glm::vec2& position, Modifiers mods = Modifiers());

		MouseButton button;
		Modifiers mods;
		glm::vec2 position;
	};
}
