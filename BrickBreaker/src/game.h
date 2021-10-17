#include "ball.h"
#include "brick.h"
#include "graphics_system.h"
#include "paddle.h"
#include "update_system.h"

#include <meteor/core/application.h>
#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/image.h>
#include <meteor/graphics/opengl/texture_2d.h>
#include <meteor/graphics/renderers/texture_renderer_2d.h>
#include <meteor/input/input_axis.h>

enum class PlayState
{
	Launch,
	Move
};

class Game : public mtr::Application
{
public:
	explicit Game(const mtr::Config& config);

	void on_key_press(const mtr::KeyPressEvent& event) override;
	void on_key_release(const mtr::KeyReleaseEvent& event) override;

	void update() override;
	void draw() override;

private:
	GraphicsSystem m_graphics;
	UpdateSystem m_update_system;

	PlayState m_play_state = PlayState::Launch;
	mtr::InputAxis m_input_axis;

	Paddle m_paddle;
	std::vector<Brick> m_bricks;
	std::vector<Ball> m_balls;
};