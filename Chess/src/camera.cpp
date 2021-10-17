#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int window_width, int window_height) :
	m_window_width{ window_width },
	m_window_height{ window_height },
	m_window_mid_x{ window_width / 2 },
	m_window_mid_y{ window_height / 2 },
	m_position{ 700.f, 300.f, 200.f },
	m_direction{ 0.f, 0.f, 0.f },
	m_rotation{ 0.f, 0.f, 0.f, 0.f },
	m_speed{ 0.f, 0.f, 0.f, 0.f },
	m_movement_speed_factor{ 100.f },
	m_pitch_sensitivity{ 0.2f },
	m_yaw_sensitivity{ 0.2f }
{
	glm::vec3 initial_position = cartesian_to_polar(m_position);
	m_angle_h = initial_position[0];
	m_angle_v = initial_position[1];
	m_distance = initial_position[2];
}

void Camera::update()
{
	if (m_holding_forward)
	{
		m_angle_v -= glm::radians(45.f) / 60.f;
		if (m_angle_v < glm::radians(0.001f))
		{
			m_angle_v = glm::radians(0.001f);
		}
	}

	if (m_holding_backward)
	{
		m_angle_v += glm::radians(45.f) / 60.f;
		if (m_angle_v > glm::radians(90.f))
		{
			m_angle_v = glm::radians(90.f);
		}
	}

	if (m_holding_left)
	{
		m_angle_h += glm::radians(180.f) / 60.f;
		if (m_angle_h > glm::radians(360.f))
		{
			m_angle_h = glm::radians(0.f);
		}
	}

	if (m_holding_right)
	{
		m_angle_h -= glm::radians(180.f) / 60.f;
		if (m_angle_h < glm::radians(0.f))
		{
			m_angle_h = glm::radians(360.f);
		}
	}

	if (m_holding_zoom_neg)
	{
		m_distance += static_cast<float>(500) / 60.f;
		if (m_distance > 3000)
		{
			m_distance -= static_cast<float>(500) / 60.f;
		}
	}

	if (m_holding_zoom_pos)
	{
		m_distance -= static_cast<float>(500) / 60.f;
		if (m_distance < 0.001)
		{
			m_distance += static_cast<float>(500) / 60.f;
		}
	}

	m_position = polar_to_cartesian(m_angle_h, m_angle_v, m_distance);
}

const glm::vec3& Camera::position() const
{
	return m_position;
}

const glm::vec3& Camera::direction() const
{
	return m_direction;
}

const glm::vec4& Camera::rotation() const
{
	return m_rotation;
}

void Camera::set_forward(bool value)
{
	m_holding_forward = value;
}

void Camera::set_backward(bool value)
{
	m_holding_backward = value;
}

void Camera::set_left(bool value)
{
	m_holding_left = value;
}

void Camera::set_right(bool value)
{
	m_holding_right = value;
}

void Camera::set_zoom_pos(bool value)
{
	m_holding_zoom_pos = value;
}

void Camera::set_zoom_neg(bool value)
{
	m_holding_zoom_neg = value;
}

const glm::mat4& Camera::view_matrix() const
{
	glm::mat4 m_view_matrix = glm::mat4(1.f);
	m_view_matrix = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	return m_view_matrix;
}

void Camera::init_camera()
{
	m_position = glm::vec3(700.f, 300.f, 200.f);
	m_direction = glm::vec3(0.f, 0.f, 0.f);
	m_rotation = glm::vec4(0.f, 0.f, 0.f, 0.f);
	m_speed = glm::vec4(0.f, 0.f, 0.f, 0.f);
	m_movement_speed_factor = 100.f;
	m_pitch_sensitivity = 0.2f;
	m_yaw_sensitivity = 0.2f;

	glm::vec3 initial_position = cartesian_to_polar(m_position);
	m_angle_h = initial_position[0];
	m_angle_v = initial_position[1];
	m_distance = initial_position[2];
}

const glm::vec3& Camera::polar_to_cartesian(float phi, float theta, float d)
{
	glm::vec3 pos;
	pos.x = d * sin(theta) * cos(phi);
	pos.y = d * cos(theta);
	pos.z = d * sin(theta) * sin(phi);

	return pos;
}

const glm::vec3& Camera::cartesian_to_polar(const glm::vec3& init_pos)
{
	float x = init_pos.x;
	float y = init_pos.y;
	float z = init_pos.z;

	float phi = atan(y / x);
	float r = sqrt(x * x + y * y + z * z);
	float theta = acos(z / r);

	glm::vec3 pos = glm::vec3(phi, theta, r);
	return pos;
}
