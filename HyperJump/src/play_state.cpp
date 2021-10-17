#include "consts.h"
#include "play_state.h"

#include <meteor/math/rect.h>

PlayState::PlayState(size_t width, size_t height) :
	m_level{ width, height },
	m_camera_2d{ game::WINDOW_WIDTH, game::WINDOW_HEIGHT }
{}

void PlayState::draw()
{
	m_shape_renderer_2d.clear();

	auto bounds = mtr::FloatRect::with_size({ game::TILE_SIZE, game::TILE_SIZE });

	for (size_t i = 0; i != m_level.tiles().height(); ++i)
	{
		bounds.y = i * game::TILE_SIZE;

		for (size_t j = 0; j != m_level.tiles().width(); ++j)
		{
			bounds.x = j * game::TILE_SIZE;

			m_shape_renderer_2d.draw_border(bounds, 2.f, mtr::COLOR_BLACK);
		}
	}

	m_shape_renderer_2d.render(m_camera_2d);
}
