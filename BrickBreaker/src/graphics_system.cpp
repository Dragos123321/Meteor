#include "consts.h"
#include "graphics_system.h"
#include "paddle.h"

#include <meteor/graphics/image.h>

GraphicsSystem::GraphicsSystem() :
	m_camera_2d{ game::ARENA_WIDTH, game::ARENA_HEIGHT }
{
	mtr::gl::TextureSettings texture_settings;
	texture_settings.format = mtr::gl::TextureFormat::Rgba;
	texture_settings.min_filter = mtr::gl::TextureMinFilter::Nearest;
	texture_settings.mag_filter = mtr::gl::TextureMagFilter::Nearest;

	m_brick_texture = mtr::gl::Texture2D(mtr::Image("brick_breaker_assets/brick.png"), texture_settings);
	m_ball_texture = mtr::gl::Texture2D(mtr::Image("brick_breaker_assets/ball.png"), texture_settings);
	m_paddle_texture = mtr::gl::Texture2D(mtr::Image("brick_breaker_assets/paddle.png"), texture_settings);
}

void GraphicsSystem::render(const std::vector<Brick>& bricks, const std::vector<Ball>& balls, const Paddle& paddle)
{
	m_texture_renderer_2d.clear();

	for (const auto& brick : bricks)
	{
		m_texture_renderer_2d.draw(m_brick_texture, brick.bounds, brick.color);
	}

	for (const auto& ball : balls)
	{
		m_texture_renderer_2d.draw(m_ball_texture, ball.bounds, mtr::ColorRgb(20, 220, 40));
	}

	m_texture_renderer_2d.draw(m_paddle_texture, paddle.bounds, mtr::ColorRgb(200, 10, 180));
	m_texture_renderer_2d.render(m_camera_2d);
}
