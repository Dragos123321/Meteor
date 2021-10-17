#pragma once

#include "meteor/input/close_request_event.h"
#include "meteor/input/button_event.h"
#include "meteor/input/key_event.h"
#include "meteor/input/mouse_event.h"
#include "meteor/input/resize_event.h"
#include "meteor/input/scroll_event.h"

#include <variant>

namespace mtr
{
	enum class EventType
	{
		CloseRequest,
		KeyPress,
		KeyRelease,
		ButtonPress,
		ButtonRelease,
		MouseMove,
		Scroll,
		Resize,
		FrameResize
	};

	using EventVariant = std::variant<CloseRequestEvent, KeyPressEvent,
		KeyReleaseEvent, ButtonPressEvent, ButtonReleaseEvent,
		MouseMoveEvent, ScrollEvent, ResizeEvent, FrameResizeEvent>;
}