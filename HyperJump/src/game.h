#include "play_state.h"

#include <meteor/core/application.h>

class Game : public mtr::Application
{
public:
	explicit Game(const mtr::Config& config);

	void draw() override;

private:
	PlayState m_play_state;
};