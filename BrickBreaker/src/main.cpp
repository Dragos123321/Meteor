#include "consts.h"
#include "game.h"

#include <fmt/format.h>

#include <meteor/core/config.h>

int main()
{
	mtr::Config config;
	config.set_window_title("Brick Breaker");
	config.set_window_size(static_cast<uint32_t>(game::ARENA_WIDTH), static_cast<uint32_t>(game::ARENA_HEIGHT));
	config.set_window_icon_path("brick_breaker_assets/icon.png");
	
	try
	{
		Game game(config);
		game.run();
	}
	catch (const std::exception& error)
	{
		fmt::print("Uncaught {}: {}", typeid(error).name(), error.what());
	}
}