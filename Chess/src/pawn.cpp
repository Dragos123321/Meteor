#include "pawn.h"

Pawn::Pawn(int side)
{
	if (side == 1)
	{
		m_part = -1;
	}

	else if (side == 2)
	{
		m_part = 1;
	}
}

void Pawn::compute_available_movements(std::vector<Piece*> own, std::vector<Piece*> opp)
{
    glm::ivec2 pos;
    bool found = false;

    pos.x = m_pos_x + m_part;
    pos.y = m_pos_y;
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
            found = true;
        }
    }

    if (!found && pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8)
    {
        m_available_movements.push_back(pos);

        if ((m_part == -1 && m_pos_x == 6) || (m_part == 1 && m_pos_x == 1))
        {
            pos.x = m_pos_x + 2 * m_part;
            pos.y = m_pos_y;
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
                    found = true;
                }
            }

            if (!found && pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8)
            {
                m_available_movements.push_back(pos);
            }
        }
    }

    pos.x = m_pos_x + m_part;
    pos.y = m_pos_y + 1;
    for (unsigned int j = 0; j < opp.size(); j++)
    {
        if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
        {
            m_available_movements.push_back(pos);
        }
    }

    pos.x = m_pos_x + m_part;
    pos.y = m_pos_y - 1;
    for (unsigned int j = 0; j < opp.size(); j++)
    {
        if (opp[j]->get_position().x == pos.x && opp[j]->get_position().y == pos.y)
        {
            m_available_movements.push_back(pos);
        }
    }
}

std::string_view Pawn::get_model_path() const
{
	return std::string_view("chess_assets/pieces/pion.obj");
}

std::string_view Pawn::get_name() const
{
	return std::string_view("Pawn");
}
