#include "meteor/graphics/opengl/index_buffer.h"

namespace mtr::gl
{
	/*
		IndexBufferBinder
	*/
	IndexBufferBinder::IndexBufferBinder(IndexBuffer& index_buffer) :
		BufferBinder<GLuint, GL_ELEMENT_ARRAY_BUFFER>(index_buffer)
	{}

	/*
		ConstIndexBufferBinder
	*/
	ConstIndexBufferBinder::ConstIndexBufferBinder(const IndexBuffer& index_buffer) :
		ConstBufferBinder<GLuint, GL_ELEMENT_ARRAY_BUFFER>(index_buffer)
	{}
}