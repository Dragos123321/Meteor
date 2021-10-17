#pragma once

#include "meteor/graphics/opengl/context.h"

#include <stdexcept>

namespace mtr::gl
{
	enum class BufferUsage
	{
		StreamDraw = GL_STREAM_DRAW, 
		StreamRead = GL_STREAM_READ, 
		StreamCopy = GL_STREAM_COPY, 
		StaticDraw = GL_STATIC_DRAW, 
		StaticRead = GL_STATIC_READ, 
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW, 
		DynamicRead = GL_DYNAMIC_READ, 
		DynamicCopy = GL_DYNAMIC_COPY
	};

	template<typename T, GLenum TARGET> 
	class BufferBinder;

	template<typename T>
	class Buffer : NonCopyable
	{
	public:
		template<typename U, GLenum TARGET>
		friend class BufferBinder;

		Buffer();
		Buffer(Buffer&& other) noexcept;
		~Buffer();

		Buffer& operator=(Buffer&& other) noexcept;

		GLuint id() const;
		bool valid() const;
		GLuint size() const;
		GLuint capacity() const;

	protected:
		void dispose();

		GLuint m_id = 0;
		GLuint m_size = 0;
		GLuint m_capacity = 0;
	};

	template<typename T, GLenum TARGET>
	class BufferBinder : NonCopyable, NonMovable
	{
	public:
		explicit BufferBinder(Buffer<T>& buffer, GLuint binding = 0);
		~BufferBinder();

		void upload_data(const T* data, size_t count, BufferUsage usage = BufferUsage::StaticDraw);

		const Buffer<T>& buffer() const;

	private:
		Buffer<T>* m_buffer = nullptr;
	};

	template<typename T, GLenum TARGET>
	class ConstBufferBinder : NonCopyable, NonMovable 
	{
	public:
		explicit ConstBufferBinder(const Buffer<T>& buffer, GLuint binding = 0);
		~ConstBufferBinder();

		const Buffer<T>& buffer() const;

	private:
		const Buffer<T>* m_buffer = nullptr;
	};

	class BufferOutOfMemoryError : public std::runtime_error
	{
	public:
		explicit BufferOutOfMemoryError(size_t requested_bytes);

		size_t requested_bytes() const;

	private:
		size_t m_requested_bytes = 0;
	};

#pragma region Implementation
	/*
		Buffer
	*/
	template<typename T>
	Buffer<T>::Buffer()
	{
		glGenBuffers(1, &m_id);
	}

	template<typename T>
	Buffer<T>::Buffer(Buffer&& other) noexcept :
		m_id{ other.m_id },
		m_size{ other.m_size },
		m_capacity{ other.m_capacity }
	{
		other.m_id = 0;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	template<typename T>
	Buffer<T>::~Buffer()
	{
		dispose();
	}

	template<typename T>
	Buffer<T>& Buffer<T>::operator=(Buffer&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		m_size = other.m_size;
		m_capacity = other.m_capacity;

		other.m_id = 0;
		other.m_size = 0;
		other.m_capacity = 0;

		return *this;
	}

	template<typename T>
	GLuint Buffer<T>::id() const
	{
		return m_id;
	}

	template<typename T>
	bool Buffer<T>::valid() const
	{
		return m_id;
	}

	template<typename T>
	GLuint Buffer<T>::size() const
	{
		return m_size;
	}

	template<typename T>
	GLuint Buffer<T>::capacity() const
	{
		return m_capacity;
	}

	template<typename T>
	void Buffer<T>::dispose()
	{
		if (m_id) glDeleteBuffers(1, &m_id);
	}

	/*
		BufferBinder
	*/
	template<typename T, GLenum TARGET>
	BufferBinder<T, TARGET>::BufferBinder(Buffer<T>& buffer, GLuint binding) :
		m_buffer{ &buffer }
	{
		CONTEXT->bind_buffer(TARGET, buffer.id(), binding);
	}

	template<typename T, GLenum TARGET>
	BufferBinder<T, TARGET>::~BufferBinder()
	{
		CONTEXT->unbind_buffer(TARGET);
	}

	template<typename T, GLenum TARGET>
	void BufferBinder<T, TARGET>::upload_data(const T* data, size_t count, BufferUsage usage)
	{
		GLuint data_size = static_cast<GLuint>(count * sizeof(T));

		if (data_size > m_buffer->capacity())
		{
			glBufferData(TARGET, static_cast<GLsizeiptr>(data_size), data, static_cast<GLenum>(usage));

			if (glGetError() == GL_OUT_OF_MEMORY)
			{
				throw BufferOutOfMemoryError(data_size);
			}

			m_buffer->m_capacity = static_cast<GLuint>(count);
			m_buffer->m_size = static_cast<GLuint>(count);
		}
		else
		{
			glBufferSubData(TARGET, 0, static_cast<GLsizeiptr>(data_size), data);
			m_buffer->m_size = static_cast<GLuint>(count);
		}
	}

	template<typename T, GLenum TARGET>
	const Buffer<T>& BufferBinder<T, TARGET>::buffer() const
	{
		return *m_buffer;
	}

	/*
		ConstBufferBinder
	*/
	template<typename T, GLenum TARGET>
	ConstBufferBinder<T, TARGET>::ConstBufferBinder(const Buffer<T>& buffer, GLuint binding) :
		m_buffer{ &buffer }
	{
		CONTEXT->bind_buffer(TARGET, buffer.id(), binding);
	}

	template<typename T, GLenum TARGET>
	ConstBufferBinder<T, TARGET>::~ConstBufferBinder()
	{
		CONTEXT->unbind_buffer(TARGET);
	}

	template<typename T, GLenum TARGET>
	const Buffer<T>& ConstBufferBinder<T, TARGET>::buffer() const
	{
		return *m_buffer;
	}
#pragma endregion
}