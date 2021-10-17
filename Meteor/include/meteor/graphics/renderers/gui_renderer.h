#pragma once

#include "meteor/graphics/renderers/gui_shape_renderer.h"
#include "meteor/graphics/renderers/gui_text_renderer.h"

namespace mtr
{
	class GuiRenderer
	{
	public:
		static const float DEFAULT_SHAPE_SMOOTHNESS;

		void clear();
		void draw_rounded_rect(const FloatRect& bounds, const ColorRgbaF& color, 
			float radius, float smoothness = DEFAULT_SHAPE_SMOOTHNESS);
		void draw_text(const Text& text);
		void render(const Camera2D& camera);

	private:
		GuiShapeRenderer m_shape_renderer;
		GuiTextRenderer m_text_renderer;
	};
}
