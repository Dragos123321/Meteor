#include "scene.h"

#include <meteor/graphics/model_loader.h>
#include <meteor/graphics/opengl/drawing.h>
#include <meteor/graphics/opengl/framebuffer.h>

#include <string>

Scene::Scene(int width, int height) :
	m_scene_width{ width },
	m_scene_height{ height },
	m_camera{ glm::vec3(0.9f, 604.f, 563.f) },
	m_selection_color{ glm::vec3(1.f, 1.f, 0.f) }
{
	init_pieces();

	m_camera.rotate(0.f, -200.f);
	m_current_id = 0;
}

size_t Scene::size() const
{
	return m_piece_manager_list.size();
}

PieceManager& Scene::operator[](size_t index) 
{
	return m_piece_manager_list[index];
}

size_t Scene::add_piece(std::string_view name, int team, const glm::vec3& pos)
{
	PieceManager piece;

	if (team == 1)
	{
		piece = PieceManager::load_piece(name, glm::vec3(1.f));
	}

	else
	{
		piece = PieceManager::load_piece(name, glm::vec3(0.2f));
	}

	piece.set_id(++m_current_id);

	piece.translate(pos);
	m_piece_manager_list.push_back(piece);
	
	return m_piece_manager_list.size();
}

std::vector<size_t> Scene::add_pieces(const std::vector<std::string>& names, std::vector<int> team, const std::vector<glm::vec3>& positions)
{
	PieceManager board = m_piece_manager_list[0];
	m_piece_manager_list.clear();
	m_piece_manager_list.push_back(board);
	m_current_id++;

	std::map<std::string, size_t> loaded_models;
	std::vector<size_t> indices;

	for (size_t i = 0; i < names.size(); ++i)
	{
		PieceManager piece;
		auto it = loaded_models.find(names[i].data());
		
		if (it == loaded_models.end())
		{
			if (team[i] == 1)
			{
				piece = PieceManager::load_piece(names[i], glm::vec3(1.f));
			}

			else
			{
				piece = PieceManager::load_piece(names[i], glm::vec3(0.2f));
			}

			loaded_models[names[i].data()] = m_piece_manager_list.size();
		}

		else
		{
			if (team[i] == 1)
			{
				piece = PieceManager(m_piece_manager_list[it->second]);
				piece.set_color(glm::vec3(1.f));
				piece.set_model_matrix(glm::mat4(1.f));
			}

			else
			{
				piece = PieceManager(m_piece_manager_list[it->second]);
				piece.set_color(glm::vec3(0.2f));
				piece.set_model_matrix(glm::mat4(1.f));
			}
		}

		piece.set_id(m_current_id++);
		piece.translate(positions[i]);
		m_piece_manager_list.push_back(piece);
		indices.push_back(m_piece_manager_list.size());

		if (team[i] == 2 && (names[i] == "Knight" || names[i] == "Bishop"))
			m_piece_manager_list[i].rotate_180();
	}

	return indices;
}

void Scene::init_pieces()
{
	m_loaded_models["chess_assets/pieces/bishop.obj"] = PieceManager::load_piece("Bishop", glm::vec3(1.f));
	m_loaded_models["chess_assets/pieces/knight.obj"] = PieceManager::load_piece("Knight", glm::vec3(1.f));
	m_loaded_models["chess_assets/pieces/king.obj"] = PieceManager::load_piece("King", glm::vec3(1.f));
	m_loaded_models["chess_assets/pieces/pawn.obj"] = PieceManager::load_piece("Pawn", glm::vec3(1.f));
	m_loaded_models["chess_assets/pieces/queen.obj"] = PieceManager::load_piece("Queen", glm::vec3(1.f));
	m_loaded_models["chess_assets/pieces/rook.obj"] = PieceManager::load_piece("Rook", glm::vec3(1.f));

	PieceManager m_board = PieceManager::load_piece("Plane", glm::vec3(0.f));

	m_piece_manager_list.push_back(m_board);
}

void Scene::select_model(size_t index)
{
	m_selected_model = index;
	m_piece_selection = true;
}

void Scene::unselect()
{
	m_piece_selection = false;
}

void Scene::slide_piece_to(size_t index, const glm::vec3& target)
{
	m_piece_manager_list[index].request_movement(target);
}

void Scene::jump_piece_to(size_t index, const glm::vec3& target)
{
	m_piece_manager_list[index].request_jump_movement(target);
}

void Scene::eject_piece(size_t index)
{
	m_piece_manager_list[index].request_eject_movement();
}

void Scene::delete_piece(size_t index)
{
	m_piece_manager_list.erase(m_piece_manager_list.begin() + index);
}

bool Scene::selected() const
{
	return m_piece_selection;
}

size_t Scene::get_selected() const
{
	return m_selected_model;
}

mtr::Camera3D& Scene::camera() 
{
	return m_camera;
}
