#include "meteor/graphics/opengl/render_settings.h"

namespace mtr::gl
{
	void RenderSettings::apply() const
	{
		blend.apply();
		depth_test.apply();
		scissor_test.apply();
		stencil_test.apply();

		glDepthFunc(static_cast<GLenum>(depth_function));
	}
}