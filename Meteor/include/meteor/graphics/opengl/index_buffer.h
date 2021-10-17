#pragma once

#include "meteor/graphics/opengl/buffer.h"

namespace mtr::gl
{
	using IndexBuffer = Buffer<GLuint>;

	class IndexBufferBinder : public BufferBinder<GLuint, GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		explicit IndexBufferBinder(IndexBuffer& index_buffer);
	};

	class ConstIndexBufferBinder : public ConstBufferBinder<GLuint, GL_ELEMENT_ARRAY_BUFFER>
	{
	public:
		explicit ConstIndexBufferBinder(const IndexBuffer& index_buffer);
	};
}
