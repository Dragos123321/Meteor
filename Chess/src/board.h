#pragma once

#include "scene.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "player.h"

class Board
{
public:
	Board() = default;
	~Board() = default;

	std::vector<std::vector<Piece*>> init_classic(Scene* scene);
	std::vector<std::vector<Piece*>> init_from_file(Scene* scene);

	void compute_all_cells();

	const glm::vec3 get_pos_at(int i, int j) const;
	const glm::vec3 get_out() const;
	Piece* get_piece_by_id(int id);

	void move_piece_to(int id, int x, int y);

private:
	std::vector<Piece*> init_piece_from_file(int side);
	std::vector<Piece*> init_piece(int side);

	Scene* m_scene;
	std::map<int, Piece*> m_piece_manager_id;

	glm::vec3 compute_real_position(int i, int j);

	std::vector<std::vector<glm::vec3>> m_cells;

	const glm::vec3 m_center_to_square0 = glm::vec3(-265.0f, 0.0f, -267.0f);
	const glm::vec3 m_out_of_bound = glm::vec3(2000.f, 0.f, 2000.f);
	const float m_square_offset = 76.0f;
};

