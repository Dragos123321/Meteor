#include "level.h"

Level::Level(size_t width, size_t height) :
	m_tiles{ width, height }
{}

const Matrix<int>& Level::tiles() const
{
	return m_tiles;
}
