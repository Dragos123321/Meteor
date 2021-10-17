#pragma once

#include "meteor/gui/padding.h"
#include "meteor/gui/widget.h"

#include <memory>
#include <vector>

namespace mtr
{
	class Panel : public Widget
	{
	public:
		using WidgetPtr = std::shared_ptr<Widget>;

		Panel();

		WidgetPtr add_widget(const WidgetPtr& widget);
		void set_bounds(const FloatRect& bounds) override;
		void set_padding(const Padding& padding);

		void pause_layout();
		void perform_layout();
		void resume_layout(bool perform_layout = true);

		size_t widget_count() const;

		virtual void on_mouse_move(const MouseMoveEvent& mouse_event);
		void on_mouse_exit() override;
		void on_button_release(const ButtonReleaseEvent& button_event) override;

	protected:
		virtual void update_layout() = 0;
		FloatRect compute_inner_bounds() const;

		Padding m_padding;
		bool m_layout_active = true;
		std::vector<WidgetPtr> m_children;
		int m_hovered_widget_index = -1;
	};
}
