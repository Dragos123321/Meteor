#include "piece_manager.h"

#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <meteor/graphics/model_loader.h>

PieceManager::PieceManager(std::string_view piece_name, glm::vec3 color) :
	m_piece_name{ piece_name },
	m_color{ color },
	m_model_matrix{ glm::mat4(1.f) }
{
}

PieceManager::PieceManager(const PieceManager& other) :
	m_piece_name{ other.m_piece_name },
	m_color{ other.m_color },
	m_id{ other.m_id },
	m_model_matrix{ other.m_model_matrix }
{
}

void PieceManager::set_id(int id)
{
	m_id = id;
}

void PieceManager::set_color(const glm::vec3& color)
{
	m_color = color;
}

void PieceManager::set_model_matrix(const glm::mat4& model)
{
	m_model_matrix = model;
}

void PieceManager::translate(const glm::vec3& vector)
{
	m_model_matrix = glm::translate(m_model_matrix, vector);
}

void PieceManager::rotate_180()
{
	m_model_matrix = glm::rotate(m_model_matrix, glm::radians(180.0f), glm::vec3(0.f, 1.f, 0.f));
	m_rotated = true;
}

void PieceManager::scale(const glm::vec3& vector)
{
	m_model_matrix = glm::scale(m_model_matrix, vector);
}

void PieceManager::request_movement(const glm::vec3& pos_end)
{
	m_position_start = glm::vec3(m_model_matrix * glm::vec4(0.f, 0.f, 0.f, 1.f));

	m_model_matrix_before_movement = glm::mat4(1.f);
	m_model_matrix_before_movement = glm::translate(m_model_matrix_before_movement, m_position_start);

	m_movement_requested = true;
	m_position_end = pos_end;
	m_movement_length = glm::length(m_position_end - m_position_start);
	m_movement_direction = glm::normalize(m_position_end - m_position_start);
	m_movement_start_time = glfwGetTime();
}

void PieceManager::request_jump_movement(const glm::vec3& pos_end)
{
	m_position_start = glm::vec3(m_model_matrix * glm::vec4(0.f, 0.f, 0.f, 1.f));

	m_model_matrix_before_movement = glm::mat4(1.f);
	m_model_matrix_before_movement = glm::translate(m_model_matrix_before_movement, m_position_start);

	m_jump_movement_requested = true;
	m_position_end = pos_end;
	m_movement_length = glm::length(m_position_end - m_position_start);
	m_movement_direction = glm::normalize(m_position_end - m_position_start);
	m_movement_start_time = glfwGetTime();

	float alpha = get_movement_length();
	b = 4 * game::JUMP_HIGH / alpha;
	a = -4 * game::JUMP_HIGH / static_cast<float>(powf(alpha, 2.f));
}

void PieceManager::request_eject_movement()
{
	m_position_start = glm::vec3(m_model_matrix * glm::vec4(0.f, 0.f, 0.f, 1.f));

	m_model_matrix_before_movement = glm::mat4(1.f);
	m_model_matrix_before_movement = glm::translate(m_model_matrix_before_movement, m_position_start);

	m_eject_movement_requested = true;
	m_position_end = glm::vec3(500.f, 0.f, 0.f);
	m_movement_length = glm::length(m_position_end - m_position_start);
	m_movement_direction = glm::normalize(m_position_end - m_position_start);
	m_movement_start_time = glfwGetTime();

	float alpha = get_movement_length();
	b = 4 * game::JUMP_HIGH / alpha;
	a = -4 * game::JUMP_HIGH / static_cast<float>(powf(alpha, 2.f));
}

bool PieceManager::is_movement_requested() const
{
	return m_movement_requested;
}

bool PieceManager::is_jump_movement_requested() const
{
	return m_jump_movement_requested;
}

bool PieceManager::is_eject_movement_requested() const
{
	return m_eject_movement_requested;
}

const glm::vec3& PieceManager::get_movement_direction() const
{
	return m_movement_direction;
}

double PieceManager::get_movement_start_time() const
{
	return m_movement_start_time;
}

float PieceManager::get_movement_length() const
{
	return m_movement_length;
}

void PieceManager::update()
{
	double elapsed_time = glfwGetTime() - m_movement_start_time;

	float movement_lenght = static_cast<float>(elapsed_time) * game::MOVEMENT_SPEED * get_movement_length();
	glm::vec3 translation = movement_lenght * m_movement_direction;

	float y;

	if (m_jump_movement_requested || m_eject_movement_requested)
	{
		y = a * pow(movement_lenght, 2.f) + b * movement_lenght;
		translation += glm::vec3(0.f, y, 0.f);
	}

	if (m_movement_length <= get_movement_length() || m_eject_movement_requested)
	{
		m_model_matrix = glm::translate(m_model_matrix_before_movement, translation);
		if (m_rotated)
			rotate_180();
	}

	if (elapsed_time >= 1.f / game::MOVEMENT_SPEED && !m_eject_movement_requested)
	{
		m_movement_requested = false;
		m_jump_movement_requested = false;

		m_model_matrix = glm::mat4(1.f);
		m_model_matrix = glm::translate(m_model_matrix, m_position_end);

		if (m_rotated)
			rotate_180();
	}

	if (elapsed_time >= 10.f / game::MOVEMENT_SPEED && m_eject_movement_requested)
	{
		m_eject_movement_requested = false;
	}
}

void PieceManager::end_movement()
{
	m_movement_requested = false;
}

PieceManager PieceManager::load_piece(std::string_view piece_name, const glm::vec3& color)
{
	PieceManager pm;
	pm.m_piece_name = piece_name;
	pm.m_color = color;

	return pm;
}
