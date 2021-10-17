#pragma once

#include "meteor/core/non_copyable.h"

#include <glad/glad.h>

#include <cstdint>

namespace mtr::gl
{
	class RenderbufferBinder;

	class Renderbuffer : NonCopyable
	{
		friend class RenderbufferBinder;

	public:
		Renderbuffer() = default;
		Renderbuffer(uint32_t width, uint32_t height);
		Renderbuffer(Renderbuffer&& other) noexcept;
		~Renderbuffer();

		Renderbuffer& operator=(Renderbuffer&& other) noexcept;

		GLuint id() const;
		bool valid() const;
		uint32_t width() const;
		uint32_t height() const;

	private:
		void dispose();

		GLuint m_id = 0;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};

	class RenderbufferBinder : NonCopyable, NonMovable
	{
	public:
		explicit RenderbufferBinder(Renderbuffer& renderbuffer);
		~RenderbufferBinder();

		void resize(uint32_t width, uint32_t height);

	private:
		Renderbuffer* m_renderbuffer = nullptr;
	};
}