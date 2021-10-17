#include "consts.h"
#include "game.h"

#include <meteor/graphics/opengl/drawing.h>

Game::Game(const mtr::Config& config) :
	mtr::Application{ config }
{
	mtr::ColorRgba color1(240, 40, 50);
	mtr::ColorRgba color2(50, 40, 240);

	for (int x = 0; x != 7; ++x)
	{
		for (int y = 0; y != 3; ++y)
		{
			if ((x + y) % 2 == 0)
			{
				m_bricks.push_back(Brick(x, y, color1));
			}
			else
			{
				m_bricks.push_back(Brick(x, y, color2));
			}
		}
	}

	m_balls.push_back(Ball({ 100.f, 100.f }));
}

void Game::on_key_press(const mtr::KeyPressEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::Left:
		m_input_axis.press_negative_axis();
		break;

	case mtr::KeyCode::Right:
		m_input_axis.press_positive_axis();
		break;

	case mtr::KeyCode::Space:
		if (m_play_state == PlayState::Launch)
		{
			for (auto& ball : m_balls)
			{
				ball.direction = { 0.f, -1.f };
				m_play_state = PlayState::Move;
			}
		}
		break;
	}
}

void Game::on_key_release(const mtr::KeyReleaseEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::Left:
		m_input_axis.release_negative_axis();
		break;

	case mtr::KeyCode::Right:
		m_input_axis.release_positive_axis();
		break;
	}
}

void Game::update()
{
	float right_limit = game::ARENA_WIDTH - m_paddle.bounds.w;

	switch (m_input_axis.value())
	{
	case mtr::InputAxisValue::Negative:
		m_paddle.bounds.x = std::clamp(m_paddle.bounds.x - game::PADDLE_SPEED, 0.f, right_limit);
		break;

	case mtr::InputAxisValue::Positive:
		m_paddle.bounds.x = std::clamp(m_paddle.bounds.x + game::PADDLE_SPEED, 0.f, right_limit);
		break;
	}

	switch (m_play_state)
	{
	case PlayState::Launch:
		for (auto& ball : m_balls)
		{
			ball.bounds.x = m_paddle.bounds.x + (m_paddle.bounds.w - ball.bounds.w) / 2.f;
			ball.bounds.set_bottom(m_paddle.bounds.y - 0.1f);
		}
		break;

	case PlayState::Move:
		switch (m_update_system.update(m_bricks, m_balls, m_paddle))
		{
		case UpdateResult::Win:

			break;

		case UpdateResult::Lose:
			m_play_state = PlayState::Launch;
			break;
		}
		break;
	}
}

void Game::draw()
{
	mtr::gl::clear_color_and_depth(mtr::COLOR_WHITE);
	m_graphics.render(m_bricks, m_balls, m_paddle);
	swap_buffers();
}