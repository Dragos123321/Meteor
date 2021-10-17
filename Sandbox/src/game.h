#pragma once

#include "board_renderer.h"

#include <meteor/core/application.h>
#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/camera_3d.h>
#include <meteor/graphics/model.h>
#include <meteor/graphics/opengl/cubemap_texture.h>
#include <meteor/graphics/opengl/framebuffer.h>
#include <meteor/graphics/opengl/program.h>
#include <meteor/graphics/renderers/cubemap_renderer.h>
#include <meteor/graphics/renderers/gui_renderer.h>
#include <meteor/graphics/renderers/texture_renderer_2d.h>
#include <meteor/graphics/directional_light.h>
#include <meteor/graphics/point_light.h>
#include <meteor/graphics/renderers/point_light_renderer.h>
#include <meteor/gui/font.h>
#include <meteor/gui/grid_panel.h>
#include <meteor/gui/label.h>

#include <array>

namespace mtr
{
	struct FrameResizeEvent;
	struct KeyPressEvent;
	struct KeyReleaseEvent;
	struct MouseMoveEvent;
}

enum class HorizontalAxis
{
	None,
	Left,
	Right
};

enum class VerticalAxis
{
	None,
	Up,
	Down
};

class Game : public mtr::Application
{
public:
	static const float CAMERA_SPEED;

	explicit Game(const mtr::Config& config);

	void on_key_press(const mtr::KeyPressEvent& event) override;
	void on_key_release(const mtr::KeyReleaseEvent& event) override;
	void on_mouse_move(const mtr::MouseMoveEvent& event) override;
	void on_frame_resize(const mtr::FrameResizeEvent& event) override;

	void update() override;
	void draw() override;

private:
	void load_framebuffer();
	void load_shaders();
	void load_cubemap();

	void load_board();
	void draw_pawns(const glm::mat4& view, const glm::mat4& projection);
	void draw_queens(const glm::mat4& view, const glm::mat4& projection);
	void draw_kings(const glm::mat4& view, const glm::mat4& projection);
	void draw_rooks(const glm::mat4& view, const glm::mat4& projection);
	void draw_knights(const glm::mat4& view, const glm::mat4& projection);
	void draw_bishops(const glm::mat4& view, const glm::mat4& projection);

	void setup_dir_light();
	void setup_point_lights();

	mtr::AudioStream m_stream;

	HorizontalAxis m_horizontal_axis = HorizontalAxis::None;
	VerticalAxis m_vertical_axis = VerticalAxis::None;
	float m_old_x = -1.f;
	float m_old_y = -1.f;

	mtr::Camera2D m_camera_2d;
	mtr::TextureRenderer2D m_texture_renderer_2d;
	mtr::gl::Framebuffer m_framebuffer;

	mtr::GridPanel m_panel;
	mtr::GuiRenderer m_gui_renderer;
	mtr::Font m_font;

	mtr::Camera3D m_camera_3d;
	std::array<mtr::Model, 6> m_models;
	mtr::CubemapRenderer m_cubemap_renderer;
	BoardRenderer m_board_renderer;
	mtr::gl::CubemapTexture m_cubemap_texture;
	mtr::gl::Texture2D m_board_texture;
	mtr::gl::Program m_program;

	mtr::DirectionalLight m_dir_light;
	const size_t m_point_light_count = 4;
	std::array<mtr::PointLight, 4> m_point_light;
	mtr::PointLightRenderer m_point_light_renderer;

	std::shared_ptr<mtr::Label> m_label;

	float m_exposure = 1.0f;

	float m_horizontal_padding = 80.f;
	float m_vertical_padding = 100.f;
	float m_side_padding = 600.f;
};