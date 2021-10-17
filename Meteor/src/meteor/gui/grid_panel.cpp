#include "meteor/gui/grid_panel.h"

#include <algorithm>

namespace mtr
{
	void GridPanel::draw(GuiRenderer& gui_renderer, WidgetState state) const
	{
		Widget::draw(gui_renderer, state);

		size_t visible_widgets = visible_widget_count();

		for (size_t i = 0; i != visible_widgets; ++i)
		{
			if (static_cast<int>(i) == m_hovered_widget_index)
			{
				m_children[i]->draw(gui_renderer, WidgetState::Hovered);
			}
			else
			{
				m_children[i]->draw(gui_renderer, WidgetState::None);
			}
		}
	}

	void GridPanel::add_row(const GridStyle& row_style)
	{
		m_row_styles.push_back(row_style);
		perform_layout();
	}

	void GridPanel::add_column(const GridStyle& column_style)
	{
		m_column_styles.push_back(column_style);
		perform_layout();
	}

	void GridPanel::uniformize_rows()
	{
		if (row_count() == 0) return;

		float row_scale = 1.f / row_count();

		for (auto& row_style : m_row_styles)
			row_style = GridStyle(SizeType::Scale, row_scale);

		perform_layout();
	}

	void GridPanel::uniformize_columns()
	{
		if (column_count() == 0) return;

		float column_scale = 1.f / column_count();

		for (auto& column_style : m_column_styles)
			column_style = GridStyle(SizeType::Scale, column_scale);

		perform_layout();
	}

	void GridPanel::set_row_count(size_t row_count)
	{
		m_row_styles.resize(row_count);
		perform_layout();
	}

	void GridPanel::set_column_count(size_t column_count)
	{
		m_column_styles.resize(column_count);
		perform_layout();
	}

	void GridPanel::set_row_style(size_t row_index, const GridStyle& row_style)
	{
		m_row_styles.at(row_index) = row_style;
		perform_layout();
	}

	void GridPanel::set_column_style(size_t column_index, const GridStyle& column_style)
	{
		m_column_styles.at(column_index) = column_style;
		perform_layout();
	}

	void GridPanel::set_row_spacing(float row_spacing)
	{
		m_row_spacing = row_spacing;
		perform_layout();
	}

	void GridPanel::set_column_spacing(float column_spacing)
	{
		m_column_spacing = column_spacing;
		perform_layout();
	}

	size_t GridPanel::row_count() const
	{
		return m_row_styles.size();
	}

	size_t GridPanel::column_count() const
	{
		return m_column_styles.size();
	}

	size_t GridPanel::cell_count() const
	{
		return row_count() * column_count();
	}

	size_t GridPanel::visible_widget_count() const
	{
		return std::min(cell_count(), widget_count());
	}

	const std::vector<GridStyle>& GridPanel::row_styles() const
	{
		return m_row_styles;
	}

	const std::vector<GridStyle>& GridPanel::column_styles() const
	{
		return m_column_styles;
	}

	float GridPanel::row_spacing() const
	{
		return m_row_spacing;
	}

	float GridPanel::column_spacing() const
	{
		return m_column_spacing;
	}

	void GridPanel::set_bounds(const FloatRect& bounds)
	{
		m_bounds = bounds;
		perform_layout();
	}

	void GridPanel::update_layout()
	{
		FloatRect inner_bounds = compute_inner_bounds();
		float mobile_horizontal_space = inner_bounds.w - compute_fixed_horizontal_space();
		float mobile_vertical_space = inner_bounds.h - compute_fixed_vertical_space();

		size_t visible_widgets = visible_widget_count();
		FloatRect widget_bounds = inner_bounds;

		for (size_t i = 0; i != row_count(); ++i)
		{
			widget_bounds.x = inner_bounds.x;

			for (size_t j = 0; j != column_count(); ++j)
			{
				size_t widget_index = i * column_count() + j;
				if (widget_index >= visible_widgets) return;

				widget_bounds.w = m_column_styles[j].compute_size(mobile_horizontal_space);
				widget_bounds.h = m_row_styles[i].compute_size(mobile_vertical_space);

				m_children[widget_index]->set_bounds(widget_bounds);

				widget_bounds.x += (widget_bounds.w + m_column_spacing);
			}

			widget_bounds.y += (widget_bounds.h + m_row_spacing);
		}
	}

	float GridPanel::compute_fixed_horizontal_space() const
	{
		float space = 0.f;

		if (column_count() != 0)
		{
			space += (column_count() - 1) * m_column_spacing;

			for (const auto& column_style : m_column_styles)
			{
				if (column_style.size_type == SizeType::Absolute)
				{
					space += column_style.value;
				}
			}
		}

		return space;
	}

	float GridPanel::compute_fixed_vertical_space() const
	{
		float space = 0.f;

		if (row_count() != 0)
		{
			space += (row_count() - 1) * m_row_spacing;

			for (const auto& row_style : m_row_styles)
			{
				if (row_style.size_type == SizeType::Absolute)
				{
					space += row_style.value;
				}
			}
		}

		return space;
	}
}