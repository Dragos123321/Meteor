#pragma once

#include <vector>

#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

class Player
{
public:
	Player() = default;
	~Player() = default;

	void init(short side);
	void init(short side, const std::vector<Piece*>& pieces);

	void compute_available_movements(std::vector<Piece*>, std::vector<Piece*>);
	void delete_piece_at(const glm::vec2& pos);
	void delete_pieces();

	const std::vector<Piece*>& get_pieces() const;
	Piece* get_piece_by_id(int id) const;
	Piece* get_king() const;

private:
	short m_number;
	std::vector<Piece*> m_pieces;
	King* m_king;
};

