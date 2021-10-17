#include "game.h"

#include <fstream>

void Game::init_classic_game(Scene* scene)
{
	m_scene = scene;

	std::vector<std::vector<Piece*>> pieces;
	pieces = m_board.init_classic(scene);

	m_player1.delete_pieces();
	m_player2.delete_pieces();
	m_none.init(0);
	m_player1.init(1, pieces[0]);
	m_player2.init(2, pieces[1]);

	m_turn = 1;

	compute_available_movements();
}

void Game::init_from_file(Scene* scene)
{
	std::vector<std::vector<Piece*>> pieces;
	pieces = m_board.init_from_file(scene);

	m_none.init(0);
	m_player1.init(1, pieces[0]);
	m_player2.init(2, pieces[1]);

	std::string line;
	std::ifstream file;
	file.open("save.txt");

	file >> line;

	scene->unselect();

	m_turn = std::stoi(line);

	compute_available_movements();
}

void Game::save()
{
	std::ofstream file;
	file.open("save.txt");

	file << m_turn << std::endl;

	for (auto piece : m_player1.get_pieces())
		file << "1 " << piece->get_name() << " " << piece->get_position().x << " " << piece->get_position().y << std::endl;

	for (auto piece : m_player2.get_pieces())
		file << "2 " << piece->get_name() << " " << piece->get_position().x << " " << piece->get_position().y << std::endl;

	file.close();
}

std::vector<Piece*> Game::check(Player player, Player opponent, const glm::vec2& king_pos) const
{
	std::vector<Piece*> threatening_pieces = std::vector<Piece*>(0);

	for (size_t i = 0; i < opponent.get_pieces().size(); ++i)
	{
		for (size_t j = 0; j < opponent.get_pieces()[i]->get_available_movements().size(); ++j)
		{
			glm::vec2 check_square = opponent.get_pieces()[i]->get_available_movements()[j];
			if (check_square.x == king_pos.x && check_square.y == king_pos.y)
			{
				threatening_pieces.push_back(opponent.get_pieces()[i]);
				if (threatening_pieces.size() >= 2)
				{
					return threatening_pieces;
				}
			}
		}
	}

	return threatening_pieces;
}

bool Game::check_mate(Player player, Player opponent, std::vector<Piece*> threatening_pieces)
{
	bool in_check_mate = true;
	glm::vec2 temp_pos = player.get_king()->get_position();
	glm::vec2 king_pos = temp_pos;

	for (size_t i = 0; i < player.get_king()->get_available_movements().size(); ++i)
	{
		king_pos = player.get_king()->get_available_movements()[i];
		player.get_king()->move_to(king_pos);
		opponent.compute_available_movements(opponent.get_pieces(), player.get_pieces());
		if (check(player, opponent, king_pos).size() == 0)
		{
			in_check_mate = false;
			break;
		}
	}

	player.get_king()->move_to(temp_pos);
	opponent.compute_available_movements(opponent.get_pieces(), player.get_pieces());

	if (!in_check_mate) return false;

	if (threatening_pieces.size() > 1) return true;

	glm::vec2 pos_to_reach = threatening_pieces[0]->get_position();

	for (size_t i = 0; i < player.get_pieces().size(); ++i)
	{
		for (size_t j = 0; j < player.get_pieces()[i]->get_available_movements().size(); ++j)
		{
			glm::vec2 pos_movement = player.get_pieces()[i]->get_available_movements()[j];
			if (pos_to_reach == pos_movement) return false;
		}
	}

	king_pos = player.get_king()->get_position();

	for (size_t i = 0; i < player.get_pieces().size(); ++i)
	{
		if (player.get_pieces()[i]->get_name() == "King") continue;
		temp_pos = player.get_pieces()[i]->get_position();

		for (size_t j = 0; j < player.get_pieces()[i]->get_available_movements().size(); ++j)
		{
			glm::vec2 pos_movement = player.get_pieces()[i]->get_available_movements()[j];
			player.get_pieces()[i]->move_to(pos_movement);
			opponent.compute_available_movements(opponent.get_pieces(), player.get_pieces());

			if (check(player, opponent, king_pos).size() == 0)
			{
				player.get_pieces()[i]->move_to(temp_pos);
				return false;
			}
		}

		player.get_pieces()[i]->move_to(temp_pos);
	}

	std::cout << "\nCHECKMATE \n";

	m_is_finished = true;

	return true;
}

void Game::change_turn()
{
	m_turn = (m_turn == 1) ? 2 : 1;

	compute_available_movements();

	m_scene->unselect();

	if (m_turn == 1)
	{
		std::vector<Piece*> check_state = check(m_player1, m_player2, m_player1.get_king()->get_position());
		if (check_state.size() > 0)
		{
			m_current_cubemap = 1;
			if (check_mate(m_player1, m_player2, check_state)) end_game(2);
		}

		else
			m_current_cubemap = 0;
	}

	else
	{

		std::vector<Piece*> check_state = check(m_player2, m_player1, m_player2.get_king()->get_position());
		if (check_state.size() > 0)
		{
			m_current_cubemap = 1;
			if (check_mate(m_player2, m_player1, check_state)) end_game(1);
		}

		else
			m_current_cubemap = 0;
	}
}

void Game::try_movement(int id, int x, int y)
{
	Player& current = (m_turn == 1) ? m_player1 : m_player2;
	Player& opponent = (m_turn == 1) ? m_player2 : m_player1;

	Piece* current_piece = current.get_piece_by_id(id);

	if (current_piece)
	{
		if (current_piece->can_move_to(x, y))
		{
			glm::vec2 temp_pos = current_piece->get_position();
			current_piece->move_to(x, y);
			opponent.compute_available_movements(opponent.get_pieces(), current.get_pieces());

			std::vector<Piece*> threatening_pieces = check(current, opponent, current.get_king()->get_position());

			if (threatening_pieces.size() == 0)
			{
				current_piece->move_to(temp_pos);
				eject_piece(x, y);
				m_board.move_piece_to(id, x, y);
				change_turn();
			}

			else if (threatening_pieces.size() == 1)
			{
				glm::vec2 pos = threatening_pieces.at(0)->get_position();
				if (pos == current_piece->get_position())
				{
					current_piece->move_to(temp_pos);
					eject_piece(x, y);
					m_board.move_piece_to(id, x, y);
					change_turn();
				}
				opponent.compute_available_movements(opponent.get_pieces(), current.get_pieces());
			}
		}
	}
}

void Game::try_movement(int id1, int id2)
{
	Player& current2 = (m_turn == 2) ? m_player1 : m_player2;
	Piece* piece_player2 = current2.get_piece_by_id(id2);

	if (piece_player2)
	{
		try_movement(id1, piece_player2->get_position().x, piece_player2->get_position().y);
	}
}

void Game::compute_available_movements()
{
	m_player1.compute_available_movements(m_player1.get_pieces(), m_player2.get_pieces());
	m_player2.compute_available_movements(m_player2.get_pieces(), m_player1.get_pieces());
}

int Game::get_player_id() const
{
	return m_turn;
}

void Game::end_game(int winner)
{
	m_current_cubemap = 2;

	Player looser;

	if (winner == 1)
	{
		looser = m_player2;
	}

	else
		looser = m_player1;

	for (size_t i = 0; i < looser.get_pieces().size(); ++i)
	{
		Piece* piece = looser.get_pieces()[i];
		if (piece->get_name() == "King") continue;

		m_scene->eject_piece(piece->get_id() - 1);
	}
}

void Game::eject_piece(int x, int y)
{
	Piece* piece;
	glm::vec2 position = glm::vec2(x, y);

	for (size_t i = 0; i < m_player1.get_pieces().size(); ++i)
	{
		piece = m_player1.get_pieces()[i];
		if (piece->get_position().x == position.x && piece->get_position().y == position.y)
		{
			m_scene->eject_piece(piece->get_id() - 1);
			m_player1.delete_piece_at(position);
		}
	}

	for (size_t i = 0; i < m_player2.get_pieces().size(); ++i)
	{
		piece = m_player2.get_pieces()[i];
		if (piece->get_position().x == position.x && piece->get_position().y == position.y)
		{
			m_scene->eject_piece(piece->get_id() - 1);
			m_player2.delete_piece_at(position);
		}
	}
}

bool Game::is_finished() const
{
	return m_is_finished;
}

size_t Game::current_cubemap() const
{
	return m_current_cubemap;
}
