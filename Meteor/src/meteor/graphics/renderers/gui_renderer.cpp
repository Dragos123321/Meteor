#include "meteor/graphics/renderers/gui_renderer.h"

namespace mtr
{
	const float GuiRenderer::DEFAULT_SHAPE_SMOOTHNESS = 0.7f;

	void GuiRenderer::clear()
	{
		m_shape_renderer.clear();
		m_text_renderer.clear();
	}

	void GuiRenderer::draw_rounded_rect(const FloatRect& bounds, const ColorRgbaF& color, float radius, float smoothness)
	{
		m_shape_renderer.draw_rounded_rect(bounds, color, radius, smoothness);
	}

	void GuiRenderer::draw_text(const Text& text)
	{
		m_text_renderer.draw_text(text);
	}

	void GuiRenderer::render(const Camera2D& camera)
	{
		m_shape_renderer.render(camera);
		m_text_renderer.render(camera);
	}
}