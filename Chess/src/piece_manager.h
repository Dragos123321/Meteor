#pragma once

#include <glm/glm.hpp>

#include "consts.h"

#include <string>

class PieceManager
{
public:
	PieceManager() = default;
	PieceManager(std::string_view piece_name, glm::vec3 color);
	PieceManager(const PieceManager& other);
	~PieceManager() = default;

	void set_id(int id);
	int get_id() const { return m_id; }

	std::string_view name() const { return m_piece_name; }
	const glm::vec3& color() const { return m_color; }

	void set_color(const glm::vec3& color);
	void set_model_matrix(const glm::mat4& model);
	
	const glm::mat4 get_model_matrix() const { return m_model_matrix; }

	void translate(const glm::vec3& vector);
	void rotate_180();
	void scale(const glm::vec3& vector);

	void request_movement(const glm::vec3& pos_end);
	void request_jump_movement(const glm::vec3& pos_end);
	void request_eject_movement();
	bool is_movement_requested() const;
	bool is_jump_movement_requested() const;
	bool is_eject_movement_requested() const;

	const glm::vec3& get_movement_direction() const;
	double get_movement_start_time() const;
	float get_movement_length() const;

	void update();
	void end_movement();

	bool is_rotated() const { return m_rotated; }

	static PieceManager load_piece(std::string_view piece_name, const glm::vec3& color);

private:
	int m_id = 0;
	glm::mat4 m_model_matrix = glm::mat4(1.f);
	
	std::string m_piece_name;
	glm::vec3 m_color;

	bool m_movement_requested = false;
	bool m_jump_movement_requested = false;
	bool m_eject_movement_requested = false;

	float a, b;
	float m_movement_length;
	
	glm::vec3 m_position_start, m_position_end, m_movement_direction;

	double m_movement_start_time;

	glm::mat4 m_model_matrix_before_movement;

	bool m_rotated = false;
};

