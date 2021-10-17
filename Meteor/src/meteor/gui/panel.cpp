#include "meteor/gui/panel.h"
#include "meteor/input/mouse_event.h"

namespace mtr
{
	Panel::Panel() :
		Widget{ WIDGET_STYLE_PANEL }
	{}

	Panel::WidgetPtr Panel::add_widget(const WidgetPtr& widget)
	{
		m_children.push_back(widget);
		perform_layout();

		return widget;
	}

	void Panel::set_bounds(const FloatRect& bounds)
	{
		m_bounds = bounds;
		perform_layout();
	}

	void Panel::set_padding(const Padding& padding)
	{
		m_padding = padding;
		perform_layout();
	}

	void Panel::pause_layout()
	{
		m_layout_active = false;
	}

	void Panel::perform_layout()
	{
		if (m_layout_active)
		{
			update_layout();
		}
	}

	void Panel::resume_layout(bool perform_layout)
	{
		m_layout_active = true;

		if (perform_layout)
		{
			update_layout();
		}
	}

	size_t Panel::widget_count() const
	{
		return m_children.size();
	}

	void Panel::on_mouse_move(const MouseMoveEvent& mouse_event)
	{
		int new_hovered_widget_index = -1;

		for (size_t i = 0; i != m_children.size(); ++i)
		{
			if (m_children[i]->bounds().contains(mouse_event.position))
			{
				new_hovered_widget_index = static_cast<int>(i);
				break;
			}
		}

		if (new_hovered_widget_index != m_hovered_widget_index)
		{
			if (m_hovered_widget_index != -1)
			{
				m_children[static_cast<size_t>(m_hovered_widget_index)]->on_mouse_exit();
			}

			if (new_hovered_widget_index != -1)
			{
				m_children[static_cast<size_t>(new_hovered_widget_index)]->on_mouse_enter();

				Panel* child_panel = dynamic_cast<Panel*>(
					m_children[static_cast<size_t>(new_hovered_widget_index)].get());

				if (child_panel)
				{
					child_panel->on_mouse_move(mouse_event);
				}
			}
		}

		m_hovered_widget_index = new_hovered_widget_index;
	}

	void Panel::on_mouse_exit()
	{
		m_hovered_widget_index = -1;
		Widget::on_mouse_exit();
	}

	void Panel::on_button_release(const ButtonReleaseEvent& button_event)
	{
		Widget::on_button_release(button_event);

		if (m_hovered_widget_index != -1)
		{
			m_children[static_cast<size_t>(m_hovered_widget_index)]->on_button_release(button_event);
		}
	}

	FloatRect Panel::compute_inner_bounds() const
	{
		return FloatRect(
			m_bounds.x + m_padding.left,
			m_bounds.y + m_padding.top,
			m_bounds.w - (m_padding.left + m_padding.right),
			m_bounds.h - (m_padding.top + m_padding.bottom)
		);
	}
}