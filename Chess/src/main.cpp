#include "app.h"
#include "consts.h"

#include <meteor/core/config.h>
#include <meteor/core/utils.h>

#include <fmt/core.h>

int main()
{
	uint32_t window_width = static_cast<uint32_t>(game::WINDOW_WIDTH);
	uint32_t window_height = static_cast<uint32_t>(game::WINDOW_HEIGHT);

	mtr::Config config;
	config.set_window_title("Chess");
	config.set_window_icon_path("assets/icons/icon.png");
	config.set_window_size(window_width, window_height);
	config.set_opengl_version(3, 3);
	config.set_window_resizable(true);
	config.set_window_sample_count(4);
	config.set_target_fps(60.f);

	try
	{
		App game(config);
		game.run();

		return 0;
	}
	catch (const std::exception & error)
	{
		fmt::print(stderr, "Uncaught '{}': {}\n", typeid(error).name(), error.what());
		std::cin.get();
	}

	return 0;
}