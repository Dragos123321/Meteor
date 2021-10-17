#include "rook.h"

void Rook::compute_available_movements(std::vector<Piece*> own, std::vector<Piece*> opp)
{
	glm::ivec2 pos;

	pos.x = m_pos_x + 1;
	pos.y = m_pos_y;
	bool found = false;

	while (pos.x < 8 && !found)
	{
		for (unsigned int i = 0; i < own.size(); i++)
		{
			if (own[i]->get_position().x == pos.x && own[i]->get_position().y == pos.y)
			{
				found = true;
			}
		}

		for (unsigned int j = 0; j < opp.size(); j++)
		{
			if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
			{
				m_available_movements.push_back(pos);
				found = true;
			}
		}

		if (!found)
		{
			m_available_movements.push_back(pos);
		}

		pos.x++;
	}

	pos.x = m_pos_x - 1;
	pos.y = m_pos_y;
	found = false;

	while (pos.x >= 0 && !found)
	{
		for (unsigned int i = 0; i < own.size(); i++)
		{
			if (own[i]->get_position().x == pos.x && own[i]->get_position().y == pos.y)
			{
				found = true;
			}
		}

		for (unsigned int j = 0; j < opp.size(); j++)
		{
			if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
			{
				m_available_movements.push_back(pos);
				found = true;
			}
		}

		if (!found)
		{
			m_available_movements.push_back(pos);
		}

		pos.x--;
	}

	pos.x = m_pos_x;
	pos.y = m_pos_y + 1;
	found = false;

	while (pos.y < 8 && !found)
	{
		for (unsigned int i = 0; i < own.size(); i++)
		{
			if (own[i]->get_position().x == pos.x && own[i]->get_position().y == pos.y)
			{
				found = true;
			}
		}

		for (unsigned int j = 0; j < opp.size(); j++)
		{
			if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
			{
				m_available_movements.push_back(pos);
				found = true;
			}
		}

		if (!found)
		{
			m_available_movements.push_back(pos);
		}

		pos.y++;
	}

	pos.x = m_pos_x;
	pos.y = m_pos_y - 1;
	found = false;

	while (pos.y >= 0 && !found)
	{
		for (unsigned int i = 0; i < own.size(); i++)
		{
			if (own[i]->get_position().x == pos.x && own[i]->get_position().y == pos.y)
			{
				found = true;
			}
		}

		for (unsigned int j = 0; j < opp.size(); j++)
		{
			if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
			{
				m_available_movements.push_back(pos);
				found = true;
			}
		}

		if (!found)
		{
			m_available_movements.push_back(pos);
		}

		pos.y--;
	}
}

std::string_view Rook::get_model_path() const
{
	return std::string_view("chess_assets/pieces/tour.obj");
}

std::string_view Rook::get_name() const
{
	return std::string_view("Rook");
}