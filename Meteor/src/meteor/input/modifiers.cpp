#include "meteor/input/modifiers.h"

namespace mtr
{
	Modifiers::Modifiers(int value) :
		value{ value }
	{}

	bool Modifiers::shift() const
	{
		return value & Shift;
	}

	bool Modifiers::ctrl() const
	{
		return value & Ctrl;
	}

	bool Modifiers::alt() const
	{
		return value & Alt;
	}

	bool Modifiers::super() const
	{
		return value & Super;
	}

	bool Modifiers::caps_lock() const
	{
		return value & CapsLock;
	}

	bool Modifiers::num_lock() const
	{
		return value & NumLock;
	}
}