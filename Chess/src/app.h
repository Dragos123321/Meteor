#pragma once

#include <meteor/core/application.h>
#include <meteor/graphics/opengl/framebuffer.h>
#include <meteor/graphics/renderers/texture_renderer_2d.h>
#include <meteor/graphics/opengl/texture_2d.h>
#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/opengl/program.h>
#include <meteor/graphics/point_light.h>

#include <glm/glm.hpp>

#include "game.h"

namespace mtr
{
	class ThreadPool;
}

namespace mtr::gl
{
	class Framebuffer;
}

namespace mtr
{
	struct FrameResizeEvent;
	struct KeyPressEvent;
	struct KeyReleaseEvent;
	struct MouseMoveEvent;
	struct ButtonPressEvent;
}

class App : public mtr::Application
{
public:
	explicit App(const mtr::Config& config);

	void on_key_press(const mtr::KeyPressEvent& event) override;
	void on_key_release(const mtr::KeyReleaseEvent& event) override;
	void on_frame_resize(const mtr::FrameResizeEvent& event) override;
	virtual void on_button_press(const mtr::ButtonPressEvent& event) override;

	void update() override;
	void draw() override;

private:
	void load_framebuffer();
	void load_shaders();
	void load_default_cubemap();
	void load_yellow_cubemap();
	void load_red_cubemap();
	void load_white_cubemap();
	void load_space_cubemap();
	void load_cubemaps();
	void load_models();
	void load_board_texture();

	void init_game();
	void init_lights();

	void render_models(const glm::mat4& view, const glm::mat4& projection, mtr::gl::Program& program);

	void render_selection();
	void process_selection(int x, int y);

	float m_old_x = -1.f;
	float m_old_y = -1.f;

	mtr::gl::Framebuffer m_framebuffer;
	mtr::Camera2D m_camera_2d;
	mtr::TextureRenderer2D m_texture_renderer_2d;
	mtr::gl::Program m_program;
	mtr::gl::Program m_board_program;
	mtr::gl::Program m_selection_program;

	Scene m_scene;
	Game m_game;

	mtr::CubemapRenderer m_cubemap_renderer;

	mtr::gl::CubemapTexture m_default_cubemap_texture;
	mtr::gl::CubemapTexture m_yellow_cubemap_texture;
	mtr::gl::CubemapTexture m_red_cubemap_texture;
	mtr::gl::CubemapTexture m_white_cubemap_texture;
	mtr::gl::CubemapTexture m_space_cubemap_texture;

	mtr::gl::Texture2D m_board_texture;

	mtr::Model m_bishop_model;
	mtr::Model m_knight_model;
	mtr::Model m_king_model;
	mtr::Model m_pawn_model;
	mtr::Model m_queen_model;
	mtr::Model m_rook_model;
	mtr::Model m_board_model;

	const size_t m_nr_point_lights = 2;
	std::array<mtr::PointLight, 2> m_point_light;
};

