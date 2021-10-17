#include "player.h"

void Player::init(short side)
{
	m_number = side;
}

void Player::init(short side, const std::vector<Piece*>& pieces)
{
	m_number = side;
	m_pieces = pieces;

	for (size_t i = 0; i < m_pieces.size(); ++i)
	{
		std::string name = m_pieces[i]->get_name().data();

		if (name == "King")
		{
			m_king = static_cast<King*>(m_pieces[i]);
			break;
		}
	}
}

void Player::compute_available_movements(std::vector<Piece*> own, std::vector<Piece*> opp)
{
	for (size_t i = 0; i < m_pieces.size(); ++i)
	{
		m_pieces[i]->clear_available_movements();
		m_pieces[i]->compute_available_movements(own, opp);
	}
}

void Player::delete_piece_at(const glm::vec2& pos)
{
	for (size_t i = 0; i < m_pieces.size(); ++i)
	{
		if (m_pieces[i]->get_position() == pos)
		{
			m_pieces.erase(m_pieces.begin() + i);
			return;
		}
	}
}

void Player::delete_pieces()
{
	for (size_t i = 0; i < m_pieces.size(); ++i)
		delete m_pieces[i];

	m_pieces.clear();
}

const std::vector<Piece*>& Player::get_pieces() const
{
	return m_pieces;
}

Piece* Player::get_piece_by_id(int id) const
{
	for (size_t i = 0; i < m_pieces.size(); ++i)
	{
		if (m_pieces[i]->get_id() == id)
			return m_pieces[i];
	}

	return nullptr;
}

Piece* Player::get_king() const
{
	return static_cast<Piece*>(m_king);
}
