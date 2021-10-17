#pragma once

#include "meteor/core/non_copyable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace mtr 
{
	/*enum class CameraMovement 
	{
		Forward,
		Backward,
		Left,
		Right
	};*/

	enum class CameraDirection
	{
		Positive,
		Negative
	};

	class Camera3D : NonCopyable
	{
	public:
		static const float DEFAULT_ROTATION_X;
		static const float DEFAULT_ROTATION_Y;
		static const float DEFAULT_ROTATION_Z;
		static const float DEFAULT_SPEED;
		static const float DEFAULT_MOUSE_SENSITIVITY;
		static const float DEFAULT_ZOOM;

		explicit Camera3D(const glm::vec3& position = { 0.0f, 0.0f, 0.0f }, 
						const glm::vec3& up = { 0.0f, 1.0f, 0.0f }, 
						float rotation_x = DEFAULT_ROTATION_X, 
						float rotation_y = DEFAULT_ROTATION_Y,
						float rotation_z = DEFAULT_ROTATION_Z);

		Camera3D(float pos_x, float pos_y, float pos_z,
			   float up_x, float up_y, float up_z, 
			   float rotation_x = DEFAULT_ROTATION_X,
			   float rotation_y = DEFAULT_ROTATION_Y,
			   float rotation_z = DEFAULT_ROTATION_Z);

		Camera3D(Camera3D&& other) noexcept;

		Camera3D& operator=(Camera3D&& other) noexcept;

		~Camera3D();

		void move_on_x(const CameraDirection& direction, float delta_time);
		void move_on_y(const CameraDirection& direction, float delta_time);
		void move_on_z(const CameraDirection& direction, float delta_time);
		void rotate(float offset_x, float offset_y, bool constrain_rotation_y = true);
		void zoom(float offset_y);

		void set_position(const glm::vec3& position);

		glm::mat4 view_matrix() const;
		const glm::vec3& position() const;
		const glm::vec3& direction() const;
		float zoom() const;

	private:
		void update();

		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_front;
		glm::vec3 m_right;
		glm::vec3 m_world_up;

		float m_rotation_x;
		float m_rotation_y;
		float m_rotation_z;
	
		float m_move_speed;
		float m_mouse_sensitivity;
		float m_zoom;
	};
}