#pragma once

#include "meteor/gui/widget_style.h"
#include "meteor/math/rect.h"

#include <functional>

namespace mtr
{
	struct ButtonReleaseEvent;
	class GuiRenderer;
	struct MouseMoveEvent;

	enum class WidgetState
	{
		None,
		Hovered
	};

	class Widget
	{
	public:
		using MouseEnterCallback = std::function<void(Widget&)>;
		using MouseExitCallback = std::function<void(Widget&)>;
		using ButtonReleaseCallback = std::function<void(Widget&, const ButtonReleaseEvent&)>;
		using ClickCallback = std::function<void(Widget&)>;

		Widget() = default;
		explicit Widget(const WidgetStyle& style);
		virtual ~Widget() = default;

		virtual void draw(GuiRenderer& gui_renderer, WidgetState state = WidgetState::Hovered) const;

		virtual void set_bounds(const FloatRect& bounds);
		void set_style(const WidgetStyle& style);

		void set_mouse_enter_callback(MouseEnterCallback&& mouse_enter_callback);
		void set_mouse_exit_callback(MouseExitCallback&& mouse_exit_callback);
		void set_button_release_callback(ButtonReleaseCallback&& button_release_callback);
		void set_click_callback(ClickCallback&& click_callback);

		virtual void on_mouse_enter();
		virtual void on_mouse_exit();
		virtual void on_button_release(const ButtonReleaseEvent& button_event);

		const FloatRect& bounds() const;
		const WidgetStyle& style() const;

	protected:
		FloatRect m_bounds;
		WidgetStyle m_style = WIDGET_STYLE_LIGHT;
		MouseEnterCallback m_mouse_enter_callback;
		MouseExitCallback m_mouse_exit_callback;
		ButtonReleaseCallback m_button_release_callback;
		ClickCallback m_click_callback;
	};
}