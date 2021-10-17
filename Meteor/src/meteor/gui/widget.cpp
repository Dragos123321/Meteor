#include "meteor/graphics/color.h"
#include "meteor/graphics/renderers/gui_renderer.h"
#include "meteor/gui/widget.h"
#include "meteor/input/button_event.h"

namespace mtr
{
	Widget::Widget(const WidgetStyle& style) :
		m_style{ style }
	{}

	void Widget::draw(GuiRenderer& gui_renderer, WidgetState state) const
	{
		switch (state)
		{
		case WidgetState::None:
			gui_renderer.draw_rounded_rect(m_bounds, m_style.color, m_style.corner_radius);
			break;

		case WidgetState::Hovered:
			gui_renderer.draw_rounded_rect(m_bounds, m_style.hover_color, m_style.corner_radius);
			break;
		}
	}

	void Widget::set_bounds(const FloatRect& bounds)
	{
		m_bounds = bounds;
	}

	void Widget::set_style(const WidgetStyle& style)
	{
		m_style = style;
	}

	void Widget::set_mouse_enter_callback(MouseEnterCallback&& mouse_enter_callback)
	{
		m_mouse_enter_callback = std::move(mouse_enter_callback);
	}

	void Widget::set_mouse_exit_callback(MouseExitCallback&& mouse_exit_callback)
	{
		m_mouse_exit_callback = std::move(mouse_exit_callback);
	}

	void Widget::set_button_release_callback(ButtonReleaseCallback&& button_release_callback)
	{
		m_button_release_callback = std::move(button_release_callback);
	}

	void Widget::set_click_callback(ClickCallback&& click_callback)
	{
		m_click_callback = std::move(click_callback);
	}

	void Widget::on_mouse_enter()
	{
		if (m_mouse_enter_callback)
		{
			m_mouse_enter_callback(*this);
		}
	}

	void Widget::on_mouse_exit()
	{
		if (m_mouse_exit_callback)
		{
			m_mouse_exit_callback(*this);
		}
	}

	void Widget::on_button_release(const ButtonReleaseEvent& button_event)
	{
		if (m_button_release_callback)
		{
			m_button_release_callback(*this, button_event);
		}

		if (button_event.button == MouseButton::Left)
		{
			if (m_click_callback)
			{
				m_click_callback(*this);
			}
		}
	}

	const FloatRect& Widget::bounds() const
	{
		return m_bounds;
	}

	const WidgetStyle& Widget::style() const
	{
		return m_style;
	}
}