#pragma once

#include "level.h"

#include <meteor/graphics/camera_2d.h>
#include <meteor/graphics/renderers/shape_renderer_2d.h>

class PlayState
{
public:
	PlayState(size_t width, size_t height);

	void draw();

private:
	Level m_level;
	mtr::Camera2D m_camera_2d;
	mtr::ShapeRenderer2D m_shape_renderer_2d;
};