#include "meteor/graphics/camera_3d.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace mtr 
{
	const float Camera3D::DEFAULT_ROTATION_X = -90.0f;
	const float Camera3D::DEFAULT_ROTATION_Y = 0.0f;
	const float Camera3D::DEFAULT_ROTATION_Z = 0.0f;
	const float Camera3D::DEFAULT_SPEED = 2.5f;
	const float Camera3D::DEFAULT_MOUSE_SENSITIVITY = 0.25f;
	const float Camera3D::DEFAULT_ZOOM = 45.0f;

	Camera3D::Camera3D(const glm::vec3& position, const glm::vec3& up, float rotation_x, float rotation_y, float rotation_z) :
		m_position{ position },
		m_world_up{ up },
		m_rotation_x{ rotation_x },
		m_rotation_y{ rotation_y },
		m_rotation_z{ rotation_z },
		m_move_speed{ DEFAULT_SPEED },
		m_mouse_sensitivity{ DEFAULT_MOUSE_SENSITIVITY },
		m_zoom{ DEFAULT_ZOOM } 
	{
		update();
	}

	Camera3D::Camera3D(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float rotation_x, float rotation_y, float rotation_z) :
		m_position{ pos_x, pos_y, pos_z },
		m_world_up{ up_x, up_y, up_z },
		m_rotation_x{ rotation_x },
		m_rotation_y{ rotation_y },
		m_rotation_z{ rotation_z },
		m_move_speed{ DEFAULT_SPEED },
		m_mouse_sensitivity{ DEFAULT_MOUSE_SENSITIVITY },
		m_zoom{ DEFAULT_ZOOM }
	{
		update();
	}

	Camera3D::Camera3D(Camera3D&& other) noexcept :
		m_position{ other.m_position },
		m_up{ other.m_up },
		m_front{ other.m_front },
		m_right{ other.m_right },
		m_world_up{ other.m_world_up },
		m_rotation_x{ other.m_rotation_x },
		m_rotation_y{ other.m_rotation_y },
		m_rotation_z{ other.m_rotation_z },
		m_move_speed{ other.m_move_speed },
		m_mouse_sensitivity{ other.m_mouse_sensitivity },
		m_zoom{ other.m_zoom }
	{
		update();

		other.m_position = glm::vec3(1.0f);
		other.m_up = glm::vec3(1.0f);
		other.m_front = glm::vec3(1.0f);
		other.m_right = glm::vec3(1.0f);
		other.m_world_up = glm::vec3(1.0f);
		other.m_rotation_x = 0.0f;
		other.m_rotation_y = 0.0f;
		other.m_rotation_z = 0.0f;
		other.m_move_speed = 0.0f;
		other.m_mouse_sensitivity = 0.0f;
		other.m_zoom = 0.0f;
	}

	Camera3D& Camera3D::operator=(Camera3D&& other) noexcept
	{
		m_position = other.m_position;
		m_up = other.m_up;
		m_front = other.m_front;
		m_right = other.m_right;
		m_world_up = other.m_world_up;
		m_rotation_x = other.m_rotation_x;
		m_rotation_y = other.m_rotation_y;
		m_rotation_z = other.m_rotation_z;
		m_move_speed = other.m_move_speed;
		m_mouse_sensitivity = other.m_mouse_sensitivity;
		m_zoom = other.m_zoom;

		update();

		other.m_position = glm::vec3(1.0f);
		other.m_up = glm::vec3(1.0f);
		other.m_front = glm::vec3(1.0f);
		other.m_right = glm::vec3(1.0f);
		other.m_world_up = glm::vec3(1.0f);
		other.m_rotation_x = 0.0f;
		other.m_rotation_y = 0.0f;
		other.m_rotation_z = 0.0f;
		other.m_move_speed = 0.0f;
		other.m_mouse_sensitivity = 0.0f;
		other.m_zoom = 0.0f;

		return *this;
	}

	Camera3D::~Camera3D()
	{
		m_position = glm::vec3(1.0f);
		m_up = glm::vec3(1.0f);
		m_front = glm::vec3(1.0f);
		m_right = glm::vec3(1.0f);
		m_world_up = glm::vec3(1.0f);

		m_rotation_x = 0.0f;
		m_rotation_y = 0.0f;
		m_rotation_z = 0.0f;

		m_move_speed = 0.0f;
		m_mouse_sensitivity = 0.0f;
		m_zoom = 0.0f;
	}

	void Camera3D::move_on_x(const CameraDirection& direction, float delta_time)
	{
		float velocity = m_move_speed * delta_time;

		switch (direction)
		{
		case CameraDirection::Positive:
			m_position += m_right * velocity;
			break;

		case CameraDirection::Negative:
			m_position -= m_right * velocity;
			break;
		}
	}

	void Camera3D::move_on_y(const CameraDirection& direction, float delta_time)
	{
		float velocity = m_move_speed * delta_time;

		switch (direction)
		{
		case CameraDirection::Positive:
			m_position += m_up * velocity;
			break;

		case CameraDirection::Negative:
			m_position -= m_up * velocity;
			break;
		}
	}

	void Camera3D::move_on_z(const CameraDirection& direction, float delta_time)
	{
		float velocity = m_move_speed * delta_time;

		switch (direction)
		{
		case CameraDirection::Positive:
			m_position += m_front * velocity;
			break;

		case CameraDirection::Negative:
			m_position -= m_front * velocity;
			break;
		}
	}

	void Camera3D::rotate(float offset_x, float offset_y, bool constrain_rotation_y)
	{
		m_rotation_x += offset_x * m_mouse_sensitivity;
		m_rotation_y += offset_y * m_mouse_sensitivity;

		if (constrain_rotation_y)
		{
			if (m_rotation_y >= 89.0f)
			{
				m_rotation_y = 89.0f;
			}			
			else if (m_rotation_y <= -89.0f)
			{
				m_rotation_y = -89.0f;
			}		
		}

		update();
	}

	void Camera3D::zoom(float offset_y)
	{
		if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		{
			m_zoom -= offset_y;
		}	
		else if (m_zoom <= 1.0f)
		{
			m_zoom = 1.0f;
		}	
		else if (m_zoom >= 45.0f)
		{
			m_zoom = 45.0f;
		}		
	}

	void Camera3D::set_position(const glm::vec3& position)
	{
		m_position = position;
	}

	glm::mat4 Camera3D::view_matrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	const glm::vec3& Camera3D::position() const
	{
		return m_position;
	}

	const glm::vec3& Camera3D::direction() const
	{
		return m_front;
	}

	float Camera3D::zoom() const
	{
		return m_zoom;
	}

	void Camera3D::update()
	{
		glm::vec3 front(
			cos(glm::radians(m_rotation_x)) * cos(glm::radians(m_rotation_y)),
			sin(glm::radians(m_rotation_y)),
			sin(glm::radians(m_rotation_x)) * cos(glm::radians(m_rotation_y))
		);
	
		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_world_up));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

}