#include "meteor/graphics/camera_2d.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mtr
{
	Camera2D::Camera2D(float width, float height) :
		m_bounds{ FloatRect::with_size({ width, height }) }
	{}

	void Camera2D::set_size(const glm::vec2& size)
	{
		m_bounds.set_size(size);
		m_needs_ortho_update = true;
	}

	void Camera2D::set_scale(float scale)
	{
		m_scale = scale;
		m_needs_scale_update = true;
	}

	void Camera2D::move(const glm::vec2& position_offset)
	{
		m_bounds.x += position_offset.x;
		m_bounds.y += position_offset.y;
		m_needs_ortho_update = true;
	}

	void Camera2D::resize(const glm::vec2& size_offset)
	{
		m_bounds.w += size_offset.x;
		m_bounds.h += size_offset.y;
		m_needs_ortho_update = true;
	}

	void Camera2D::scale(float scale_offset)
	{
		m_scale += scale_offset;
		m_needs_scale_update = true;
	}

	const glm::mat4& Camera2D::final_matrix() const
	{
		update_final_matrix();
		return m_final_matrix;
	}

	const FloatRect& Camera2D::bounds() const
	{
		return m_bounds;
	}

	float Camera2D::scale() const
	{
		return m_scale;
	}

	void Camera2D::update_ortho_matrix() const
	{
		if (m_needs_ortho_update)
		{
			m_ortho_matrix = glm::ortho(m_bounds.x, m_bounds.right(), m_bounds.bottom(), m_bounds.y);
			m_needs_ortho_update = false;
			m_needs_final_update = true;
		}
	}

	void Camera2D::update_scale_matrix() const
	{
		if (m_needs_scale_update)
		{
			m_scale_matrix = glm::scale(glm::identity<glm::mat4>(), { m_scale, m_scale, 1.f });
			m_needs_scale_update = false;
			m_needs_final_update = true;
		}
	}

	void Camera2D::update_final_matrix() const
	{
		update_ortho_matrix();
		update_scale_matrix();

		if (m_needs_final_update)
		{
			m_final_matrix = m_scale_matrix * m_ortho_matrix;
			m_needs_final_update = false;
		}
	}
}
