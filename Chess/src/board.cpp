#include "board.h"

#include <fstream>
#include <sstream>

std::vector<std::vector<Piece*>> Board::init_classic(Scene* scene)
{
    m_scene = scene;
    compute_all_cells();

    std::vector<std::vector<Piece*>> pieces;

    pieces.push_back(init_piece(1));
    pieces.push_back(init_piece(2));

    std::vector<std::string> name;
    std::vector<int> team;
    std::vector<glm::vec3> positions;

    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < pieces[i].size(); ++j)
        {
            name.push_back(pieces[i][j]->get_name().data());
            team.push_back(i + 1);
            positions.push_back(get_pos_at(pieces[i][j]->get_position().x, pieces[i][j]->get_position().y));
        }
    }

    std::vector<size_t> pieces_id = scene->add_pieces(name, team, positions);

    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < pieces[i].size(); ++j)
        {
            int index = (i * 16) + j;
            pieces[i][j]->set_id(pieces_id[index]);
            m_piece_manager_id[pieces_id[index]] = pieces[i][j];
        }
    }

    return pieces;
}

std::vector<std::vector<Piece*>> Board::init_from_file(Scene* scene)
{
    m_scene = scene;

    std::vector<std::vector<Piece*>> pieces(2);

    pieces[0] = init_piece_from_file(1);
    pieces[1] = init_piece_from_file(2);

    std::vector<std::string> name;
    std::vector<int> team;
    std::vector<glm::vec3> positions;

    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < pieces.size(); ++j)
        {
            name.push_back(pieces[i][j]->get_name().data());
            team.push_back(i + 1);
            positions.push_back(get_pos_at(pieces[i][j]->get_position().x, pieces[i][j]->get_position().y));
        }
    }

    std::vector<size_t> pieces_id = scene->add_pieces(name, team, positions);

    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < pieces.size(); ++j)
        {
            int index = (i * 16) + j;
            pieces[i][j]->set_id(pieces_id[index]);
            m_piece_manager_id[pieces_id[index]] = pieces[i][j];
        }
    }

    return pieces;
}

void Board::compute_all_cells()
{
    m_cells = std::vector<std::vector<glm::vec3>>(8);

    for (size_t i = 0; i < m_cells.size(); ++i)
    {
        m_cells[i].resize(8);
        for (size_t j = 0; j < m_cells[i].size(); ++j)
        {
            m_cells[i][j] = compute_real_position(i, j);
        }
    }
}

const glm::vec3 Board::get_pos_at(int i, int j) const
{
    return m_cells[i][j];
}

const glm::vec3 Board::get_out() const
{
    return m_out_of_bound;
}

Piece* Board::get_piece_by_id(int id)
{
    return m_piece_manager_id[id];
}

void Board::move_piece_to(int id, int x, int y)
{
    Piece* piece = m_piece_manager_id[id];

    if (piece == nullptr) return;

    std::string name = piece->get_name().data();

    if (name == "Knight")
    {
        m_scene->jump_piece_to(id - 1, get_pos_at(x, y));
    }

    else
        m_scene->slide_piece_to(id - 1, get_pos_at(x, y));

    piece->move_to(x, y);
}

std::vector<Piece*> Board::init_piece_from_file(int side)
{
	std::vector<Piece*> pieces;
	std::string line;
	std::ifstream file;
	file.open("save.txt");

	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string player_id, piece_name, x, y;

		std::getline(iss, player_id, ' ');
		std::getline(iss, piece_name, ' ');
		std::getline(iss, x, ' ');
		std::getline(iss, y, ' ');

		if (std::stoi(player_id) == side)
		{
			Piece* piece = nullptr;

			if (piece_name == "King")
				piece = new King();
			if (piece_name == "Bishop")
				piece = new Bishop();
			if (piece_name == "Knight")
				piece = new Knight();
			if (piece_name == "Pawn")
				piece = new Pawn(side);
			if (piece_name == "Queen")
				piece = new Queen();
			if (piece_name == "Rook")
				piece = new Rook();

			piece->init(std::stoi(x), std::stoi(y));
			pieces.push_back(piece);
		}
	}

	return pieces;
}

std::vector<Piece*> Board::init_piece(int side)
{
    std::vector<Piece*> pieces;

    Piece* pawn1 = new Pawn(side);
    Piece* pawn2 = new Pawn(side);
    Piece* pawn3 = new Pawn(side);
    Piece* pawn4 = new Pawn(side);
    Piece* pawn5 = new Pawn(side);
    Piece* pawn6 = new Pawn(side);
    Piece* pawn7 = new Pawn(side);
    Piece* pawn8 = new Pawn(side);
    Piece* rook1 = new Rook();
    Piece* rook2 = new Rook();
    Piece* knight1 = new Knight();
    Piece* knight2 = new Knight();
    Piece* bishop1 = new Bishop();
    Piece* bishop2 = new Bishop();
    Piece* queen = new Queen();
    Piece* king = new King();

    if (side == 1)
    {
        pawn1->init(6, 0);
        pawn2->init(6, 1);
        pawn3->init(6, 2);
        pawn4->init(6, 3);
        pawn5->init(6, 4);
        pawn6->init(6, 5);
        pawn7->init(6, 6);
        pawn8->init(6, 7);
        rook1->init(7, 0);
        knight1->init(7, 1);
        bishop1->init(7, 2);
        queen->init(7, 3);
        king->init(7, 4);
        bishop2->init(7, 5);
        knight2->init(7, 6);
        rook2->init(7, 7);
    }

    else if (side == 2)
    {
        pawn1->init(1, 0);
        pawn2->init(1, 1);
        pawn3->init(1, 2);
        pawn4->init(1, 3);
        pawn5->init(1, 4);
        pawn6->init(1, 5);
        pawn7->init(1, 6);
        pawn8->init(1, 7);
        rook1->init(0, 0);
        knight1->init(0, 1);
        bishop1->init(0, 2);
        queen->init(0, 3);
        king->init(0, 4);
        bishop2->init(0, 5);
        knight2->init(0, 6);
        rook2->init(0, 7);
    }

    pieces.push_back(pawn1);
    pieces.push_back(pawn2);
    pieces.push_back(pawn3);
    pieces.push_back(pawn4);
    pieces.push_back(pawn5);
    pieces.push_back(pawn6);
    pieces.push_back(pawn7);
    pieces.push_back(pawn8);
    pieces.push_back(rook1);
    pieces.push_back(rook2);
    pieces.push_back(knight1);
    pieces.push_back(knight2);
    pieces.push_back(bishop1);
    pieces.push_back(bishop2);
    pieces.push_back(queen);
    pieces.push_back(king);

    return pieces;
}

glm::vec3 Board::compute_real_position(int i, int j)
{
	return m_center_to_square0 + glm::vec3(j * m_square_offset, 0.f, i * m_square_offset);
}
