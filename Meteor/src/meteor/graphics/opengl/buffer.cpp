#include "meteor/graphics/opengl/buffer.h"

#include <fmt/format.h>

namespace mtr::gl
{
	BufferOutOfMemoryError::BufferOutOfMemoryError(size_t requested_bytes) :
		std::runtime_error{ fmt::format("failed to allocate {} bytes", requested_bytes) },
		m_requested_bytes{ requested_bytes }
	{}

	size_t BufferOutOfMemoryError::requested_bytes() const
	{
		return m_requested_bytes;
	}
}