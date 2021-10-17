#include "consts.h"
#include "game.h"

#include <meteor/core/config.h>

int main()
{
	uint32_t window_width = static_cast<uint32_t>(game::WINDOW_WIDTH);
	uint32_t window_height = static_cast<uint32_t>(game::WINDOW_HEIGHT);

	mtr::Config config;
	config.set_window_title("Hyper Jump");
	config.set_window_size(window_width, window_height);

	Game game(config);
	game.run();
}