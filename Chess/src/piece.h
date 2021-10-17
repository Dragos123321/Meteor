#pragma once

#include <vector>
#include <string_view>

#include <glm/glm.hpp>

class Piece
{
public:
	explicit Piece() = default;
	explicit Piece(int pos_x, int pos_y);

	virtual void init(int pos_x, int pos_y);

	bool can_move_to(int target_x, int target_y);

	void move_to(const glm::ivec2& target);
	void move_to(int target_x, int target_y);

	virtual std::string_view get_model_path() const = 0;
	const std::vector<glm::vec2>& get_available_movements() const;
	virtual glm::vec2 get_position() const;

	virtual int get_id() const;
	virtual void set_id(int id);

	virtual void compute_available_movements(std::vector<Piece*>, std::vector<Piece*>) = 0;
	virtual void delete_available_movements(std::vector<int>);
	void clear_available_movements();

	virtual std::string_view get_name() const = 0;

	virtual ~Piece() = default;

protected:
	int m_pos_x;
	int m_pos_y;
	int m_id;

	std::vector<glm::vec2> m_available_movements;
};

