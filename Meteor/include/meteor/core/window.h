#pragma once

#include "meteor/core/glfw.h"
#include "meteor/core/non_copyable.h"

#include <glm/glm.hpp>

#include <string_view>

namespace mtr
{
	class Config;

	class Window : NonCopyable, NonMoveAssignable
	{
	public:
		explicit Window(const Config& config);
		Window(Window&& other) noexcept;
		~Window();

		void hide() const;
		void show() const;
		void swap_buffers() const;

		void set_cursor_mode(CursorMode cursorMode);
		void set_title(std::string_view title);
		void set_fullscreen(bool fullscreen);

		GLFWwindow* glfw_handle() const;
		glm::uvec2 size() const;
		glm::uvec2 framebuffer_size() const;
		
	private:
		GLFWwindow* m_handle = nullptr;
	};
}
