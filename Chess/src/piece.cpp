#include "piece.h"

Piece::Piece(int pos_x, int pos_y) :
	m_pos_x{ pos_x },
	m_pos_y{ pos_y }
{
}

void Piece::init(int pos_x, int pos_y)
{
	m_pos_x = pos_x;
	m_pos_y = pos_y;
}

bool Piece::can_move_to(int target_x, int target_y)
{
	for (size_t i = 0; i < m_available_movements.size(); ++i)
	{
		if (m_available_movements[i].x == target_x && m_available_movements[i].y == target_y)
			return true;
	}

	return false;
}

void Piece::move_to(const glm::ivec2& target)
{
	m_pos_x = target.x;
	m_pos_y = target.y;
}

void Piece::move_to(int target_x, int target_y)
{
	move_to(glm::ivec2(target_x, target_y));
}

const std::vector<glm::vec2>& Piece::get_available_movements() const
{
	return m_available_movements;
}

glm::vec2 Piece::get_position() const
{
	return glm::vec2(m_pos_x, m_pos_y);
}

int Piece::get_id() const
{
	return m_id;
}

void Piece::set_id(int id)
{
	m_id = id;
}

void Piece::delete_available_movements(std::vector<int>)
{
}

void Piece::clear_available_movements()
{
	m_available_movements.clear();
}
