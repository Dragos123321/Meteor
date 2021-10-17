#pragma once

#include "renderer.h"

#include <meteor/core/application.h>
#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/opengl/framebuffer.h>
#include <meteor/graphics/opengl/program.h>
#include <meteor/graphics/renderers/texture_renderer_2d.h>
#include <meteor/input/input_axis.h>

#include <array>

struct MandelbrotData 
{
	float scale;
	float x;
	float y;
	int max_iterations;
};

class Fractal : public mtr::Application
{
public:
	static const float CAMERA_SPEED;

	explicit Fractal(const mtr::Config& config);

	void on_key_press(const mtr::KeyPressEvent& event) override;
	void on_key_release(const mtr::KeyReleaseEvent& event) override;
	void on_frame_resize(const mtr::FrameResizeEvent& event) override;

	void update() override;
	void draw() override;

private:
	void load_shaders();
	void setup_color_maps();

	mtr::InputAxis m_horizontal_axis;
	mtr::InputAxis m_vertical_axis;
	mtr::InputAxis m_scale;
	mtr::InputAxis m_iterations;

	mtr::Camera2D m_camera_2d;
	Renderer m_renderer;

	mtr::gl::Texture2D m_board_texture;
	mtr::gl::Program m_program;

	MandelbrotData m_data;

	size_t m_color_map_index = 0;
	std::array<std::array<glm::vec3, 17>, 4> m_color_maps;

	int neg_iterations = 0;
	int pos_iterations = 0;
};