#pragma once

#include "meteor/input/key_code.h"
#include "meteor/input/modifiers.h"

namespace mtr
{
	struct KeyPressEvent
	{
		KeyPressEvent(KeyCode key_code, int scancode, const Modifiers& mods = Modifiers(), bool repeated = false);

		KeyCode key_code;
		int scancode;
		Modifiers mods;
		bool repeated;
	};

	struct KeyReleaseEvent
	{
		KeyReleaseEvent(KeyCode key_code, int scancode, const Modifiers& mods = Modifiers());

		KeyCode key_code;
		int scancode;
		Modifiers mods;
	};
}
