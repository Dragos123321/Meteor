#include "fractal.h"

#include <meteor/core/config.h>

#include <fmt/core.h>

int main()
{
	mtr::Config config;
	config.set_window_title("Fractals");
	config.set_window_icon_path("assets/icons/icon.png");
	config.set_window_size(800, 600);
	config.set_opengl_version(3, 3);
	config.set_window_resizable(true);
	config.set_window_sample_count(4);
	config.set_target_fps(60.f);

	try
	{
		Fractal fractal(config);
		fractal.run();

		return 0;
	}
	catch (const std::exception& error)
	{
		fmt::print(stderr, "Uncaught '{}': {}\n", typeid(error).name(), error.what());
		std::cin.get();
	}

	return 0;
}