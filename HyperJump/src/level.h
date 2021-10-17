#pragma once

#include "matrix.h"

class Level
{
public:
	Level(size_t width, size_t height);

	const Matrix<int>& tiles() const;

private:
	Matrix<int> m_tiles;
};