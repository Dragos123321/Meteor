#pragma once

#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(int window_width, int window_height);
	~Camera() = default;

	void update();

	const glm::vec3& position() const;
	const glm::vec3& direction() const;
	const glm::vec4& rotation() const;

	void set_forward(bool value);
	void set_backward(bool value);
	void set_left(bool value);
	void set_right(bool value);
	void set_zoom_pos(bool value);
	void set_zoom_neg(bool value);

	const glm::mat4& view_matrix() const;

private:
	void init_camera();
	const glm::vec3& polar_to_cartesian(float phi, float theta, float d);
	const glm::vec3& cartesian_to_polar(const glm::vec3& init_pos);

	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec4 m_rotation;
	glm::vec4 m_speed;

	float m_movement_speed_factor;
	float m_pitch_sensitivity;
	float m_yaw_sensitivity;
	float m_angle_h;
	float m_angle_v;
	float m_distance;

	int m_window_width;
	int m_window_height;
	int m_window_mid_x;
	int m_window_mid_y;

	bool m_holding_forward = false;
	bool m_holding_backward = false;
	bool m_holding_left = false;
	bool m_holding_right = false;
	bool m_holding_zoom_pos = false;
	bool m_holding_zoom_neg = false;
};

