#pragma once

#include <glad/glad.h>

namespace mtr::gl
{
	class BindingData
	{
	public:
		constexpr BindingData() = default;

		bool bind(GLuint object);
		bool unbind(bool unbind = false);
		void reset();

	private:
		GLuint m_bound_object = 0;
		GLuint m_active_binders = 0;
	};
}
