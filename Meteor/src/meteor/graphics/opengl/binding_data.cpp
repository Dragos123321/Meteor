#include "meteor/graphics/opengl/binding_data.h"

#include <cassert>

namespace mtr::gl
{
	bool BindingData::bind(GLuint object)
	{
		assert(object != 0); // Check if object is valid

		if (m_bound_object == object)
		{
			++m_active_binders;
			return false;
		}

		assert(m_active_binders == 0); // Check if binder is unique

		m_bound_object = object;
		++m_active_binders;
		return true;
	}

	bool BindingData::unbind(bool unbind)
	{
		if (m_active_binders != 0)
		{
			--m_active_binders;
			
			if (m_active_binders == 0)
			{
				if (unbind)
				{
					m_bound_object = 0;
				}

				return true;
			}
		}

		return false;
	}

	void BindingData::reset()
	{
		assert(m_active_binders == 0); // Check if no binders are active
		m_bound_object = 0;
	}
}