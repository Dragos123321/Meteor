#pragma once

#include <meteor/graphics/opengl/cubemap_texture.h>
#include <meteor/graphics/renderers/cubemap_renderer.h>
#include <meteor/graphics/model.h>
#include <meteor/graphics/camera_3d.h>

#include "consts.h"
#include "piece_manager.h"
#include "camera.h"

#include <array>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
	Scene(int width, int height);

	size_t size() const;
	PieceManager& operator[](size_t index);

	size_t add_piece(std::string_view name, int team, const glm::vec3& pos);
	std::vector<size_t> add_pieces(const std::vector<std::string>& names, std::vector<int> team, const std::vector<glm::vec3>& positions);

	void init_pieces();

	void select_model(size_t index);
	void unselect();

	void slide_piece_to(size_t index, const glm::vec3& target);
	void jump_piece_to(size_t index, const glm::vec3& target);
	void eject_piece(size_t index);
	void delete_piece(size_t index);

	bool selected() const;
	size_t get_selected() const;

	mtr::Camera3D& camera();

private:
	mtr::Camera3D m_camera;

	std::vector<PieceManager> m_piece_manager_list;
	std::map<std::string, PieceManager> m_loaded_models;

	size_t m_selected_model;
	glm::vec3 m_selection_color;
	bool m_piece_selection = false;

	int m_scene_height = static_cast<int>(game::WINDOW_HEIGHT);
	int m_scene_width = static_cast<int>(game::WINDOW_WIDTH);

	int m_current_id;
};

