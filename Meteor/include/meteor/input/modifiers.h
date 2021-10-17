#pragma once

#include <glfw/glfw3.h>

namespace mtr
{
	struct Modifiers
	{
		enum Bit
		{
			None = 0,
			Shift = GLFW_MOD_SHIFT,
			Ctrl = GLFW_MOD_CONTROL,
			Alt = GLFW_MOD_ALT,
			Super = GLFW_MOD_SUPER,
			CapsLock = GLFW_MOD_CAPS_LOCK,
			NumLock = GLFW_MOD_NUM_LOCK
		};

		explicit Modifiers(int value = None);

		bool shift() const;
		bool ctrl() const;
		bool alt() const;
		bool super() const;
		bool caps_lock() const;
		bool num_lock() const;

		int value;
	};
}
