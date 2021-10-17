#include "meteor/input/key_event.h"

namespace mtr
{
	/*
		KeyPressEvent
	*/
	KeyPressEvent::KeyPressEvent(KeyCode key_code, int scancode, const Modifiers& mods, bool repeated) :
		key_code{ key_code },
		scancode{ scancode },
		mods{ mods },
		repeated{ repeated }
	{}

	/*
		KeyReleaseEvent
	*/
	KeyReleaseEvent::KeyReleaseEvent(KeyCode key_code, int scancode, const Modifiers& mods) :
		key_code{ key_code },
		scancode{ scancode },
		mods{ mods }
	{}
}