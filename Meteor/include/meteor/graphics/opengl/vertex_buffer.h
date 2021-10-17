#pragma once

#include "meteor/graphics/opengl/buffer.h"

namespace mtr::gl
{
	template<typename T>
	using VertexBuffer = Buffer<T>;

	template<typename T>
	class VertexBufferBinder : public BufferBinder<T, GL_ARRAY_BUFFER>
	{
	public:
		explicit VertexBufferBinder(VertexBuffer<T>& vertex_buffer);
	};

#pragma region Implementation
	template<typename T>
	VertexBufferBinder<T>::VertexBufferBinder(VertexBuffer<T>& vertex_buffer) :
		BufferBinder<T, GL_ARRAY_BUFFER>(vertex_buffer)
	{}
#pragma endregion
}