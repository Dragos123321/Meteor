#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/opengl/renderbuffer.h"
#include "meteor/graphics/opengl/texture_2d.h"

#include <stdexcept>

namespace mtr::gl
{
	class FramebufferBinder;

	class Framebuffer : NonCopyable
	{
		friend class FramebufferBinder;

	public:
		Framebuffer() = default;
		Framebuffer(int width, int height, const TextureSettings& texture_settings);
		Framebuffer(Framebuffer&& other) noexcept;
		~Framebuffer();

		Framebuffer& operator=(Framebuffer&& other) noexcept;

		GLuint id() const;
		bool valid() const;
		const Texture2D& color_texture() const;

	private:
		void dispose();

		GLuint m_id = 0;
		Texture2D m_color_texture;
		Renderbuffer m_renderbuffer;
	};

	enum class FramebufferBindingMode
	{
		Read,
		Draw,
		ReadAndDraw
	};

	class FramebufferBinder : NonCopyable, NonMovable
	{
	public:
		explicit FramebufferBinder(Framebuffer& framebuffer, 
			FramebufferBindingMode binding_mode = FramebufferBindingMode::ReadAndDraw);
		~FramebufferBinder();

		void resize(uint32_t width, uint32_t height);

	private:
		Framebuffer* m_framebuffer = nullptr;
		FramebufferBindingMode m_binding_mode;
	};

	class IncompleteFramebufferError : public std::runtime_error
	{
	public:
		explicit IncompleteFramebufferError(GLuint framebuffer_id);

		GLuint framebuffer_id() const;

	private:
		GLuint m_framebuffer_id = 0;
	};
}