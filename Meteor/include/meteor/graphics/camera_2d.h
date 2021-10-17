#pragma once

#include "meteor/math/rect.h"

#include <glm/glm.hpp>

namespace mtr
{
	class Camera2D
	{
	public:
		Camera2D(float width, float height);

		void set_size(const glm::vec2& size);
		void set_scale(float scale);

		void move(const glm::vec2& position_offset);
		void resize(const glm::vec2& size_offset);
		void scale(float scale_offset);

		const glm::mat4& final_matrix() const;
		const FloatRect& bounds() const;
		float scale() const;

	private:
		void update_ortho_matrix() const;
		void update_scale_matrix() const;
		void update_final_matrix() const;

		FloatRect m_bounds;
		float m_scale = 1.f;

		mutable glm::mat4 m_ortho_matrix = glm::mat4{};
		mutable glm::mat4 m_scale_matrix = glm::mat4{};
		mutable glm::mat4 m_final_matrix = glm::mat4{};

		mutable bool m_needs_ortho_update = true;
		mutable bool m_needs_scale_update = true;
		mutable bool m_needs_final_update = true;
	};
}
