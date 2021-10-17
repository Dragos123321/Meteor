#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/opengl/vertex_buffer.h"
#include "meteor/graphics/opengl/vertex_layout.h"

#include <iostream>

namespace mtr::gl
{
	class VertexLayout;

	class VertexArray : NonCopyable
	{
	public:
		VertexArray() = default;

		template<typename T>
		VertexArray(const VertexLayout& vertex_layout, VertexBuffer<T>& vertex_buffer);

		VertexArray(VertexArray&& other) noexcept;
		~VertexArray();

		VertexArray& operator=(VertexArray&& other) noexcept;

		GLuint id() const;
		bool valid() const;

	private:
		void dispose();

		GLuint m_id = 0;
	};

	class VertexArrayBinder : NonCopyable, NonMovable
	{
	public:
		explicit VertexArrayBinder(const VertexArray& array);
		~VertexArrayBinder();

	private:
		const VertexArray* m_array = nullptr;
	};

#pragma region Implementation
	template<typename T>
	VertexArray::VertexArray(const VertexLayout& vertex_layout, VertexBuffer<T>& vertex_buffer)
	{
		glGenVertexArrays(1, &m_id);
		
		VertexArrayBinder vertex_array_binder(*this);
		VertexBufferBinder vertex_buffer_binder(vertex_buffer);

		for (GLuint i = 0; i != vertex_layout.attributes().size(); ++i)
		{
			const auto& attribute = vertex_layout.attributes()[i];

			if (is_int_type(attribute.type()))
			{
				if (attribute.normalized())
				{
					glVertexAttribPointer(i, component_count(attribute.type()), component_type(attribute.type()),
						true, static_cast<GLsizei>(vertex_layout.stride()), reinterpret_cast<void*>(attribute.offset()));
				}
				else
				{
					glVertexAttribIPointer(i, component_count(attribute.type()), component_type(attribute.type()),
						static_cast<GLsizei>(vertex_layout.stride()), reinterpret_cast<void*>(attribute.offset()));
				}
			}
			else
			{	
				glVertexAttribPointer(i, component_count(attribute.type()), component_type(attribute.type()),
					false, static_cast<GLsizei>(vertex_layout.stride()), reinterpret_cast<void*>(attribute.offset()));
			}

			glEnableVertexAttribArray(i);
			glVertexAttribDivisor(i, static_cast<GLuint>(attribute.divizor()));
		}
	}
#pragma endregion
}