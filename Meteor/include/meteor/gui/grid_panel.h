#pragma once

#include "meteor/gui/grid_style.h"
#include "meteor/gui/panel.h"

namespace mtr
{
	class GridPanel : public Panel
	{
	public:
		void draw(GuiRenderer& gui_renderer, WidgetState state = WidgetState::None) const override;

		void add_row(const GridStyle& row_style = GridStyle());
		void add_column(const GridStyle& column_style = GridStyle());

		void uniformize_rows();
		void uniformize_columns();

		void set_row_count(size_t row_count);
		void set_column_count(size_t column_count);
		void set_row_style(size_t row_index, const GridStyle& row_style);
		void set_column_style(size_t column_index, const GridStyle& column_style);
		void set_row_spacing(float row_spacing);
		void set_column_spacing(float column_spacing);

		size_t row_count() const;
		size_t column_count() const;
		size_t cell_count() const;
		size_t visible_widget_count() const;
		const std::vector<GridStyle>& row_styles() const;
		const std::vector<GridStyle>& column_styles() const;
		float row_spacing() const;
		float column_spacing() const;

		void set_bounds(const FloatRect& bounds) override;

	protected:
		void update_layout() override;
		float compute_fixed_horizontal_space() const;
		float compute_fixed_vertical_space() const;

		std::vector<GridStyle> m_row_styles;
		std::vector<GridStyle> m_column_styles;
		float m_row_spacing = 0.f;
		float m_column_spacing = 0.f;
	};
}
