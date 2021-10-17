#include "game.h"

#include <meteor/graphics/opengl/drawing.h>

Game::Game(const mtr::Config& config) :
	mtr::Application{ config },
	m_play_state{ 10, 16 }
{}

void Game::draw()
{
	mtr::gl::clear_color_and_depth();
	m_play_state.draw();
	swap_buffers();
}