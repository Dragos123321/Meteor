#pragma once

#include "ball.h"
#include "brick.h"

#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/opengl/texture_2d.h>
#include <meteor/graphics/renderers/texture_renderer_2d.h>

#include <vector>

struct Paddle;

class GraphicsSystem
{
public:
	GraphicsSystem();

	void render(const std::vector<Brick>& bricks, const std::vector<Ball>& balls, const Paddle& paddle);

private:
	mtr::Camera2D m_camera_2d;
	mtr::gl::Texture2D m_brick_texture;
	mtr::gl::Texture2D m_ball_texture;
	mtr::gl::Texture2D m_paddle_texture;
	mtr::TextureRenderer2D m_texture_renderer_2d;
};
