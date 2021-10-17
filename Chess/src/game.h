#pragma once

#include "board.h"

class Game
{
public:
	void init_classic_game(Scene* scene);
	void init_from_file(Scene* scene);
	void save();

	std::vector<Piece*> check(Player player, Player opponent, const glm::vec2& king_pos) const;
	bool check_mate(Player player, Player opponent, std::vector<Piece*> threatening_pieces);

	void change_turn();

	void try_movement(int id, int x, int y);
	void try_movement(int id1, int id2);

	void compute_available_movements();

	int get_player_id() const;
	void end_game(int winner);

	void eject_piece(int x, int y);

	bool is_finished() const;

	size_t current_cubemap() const;

private:
	Scene* m_scene;
	Board m_board;
	Player m_player1, m_player2, m_none;
	int m_turn;

	size_t m_current_cubemap = 0;
	bool m_is_finished = false;
};

