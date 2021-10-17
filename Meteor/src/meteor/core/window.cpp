#include "meteor/core/config.h"
#include "meteor/core/window.h"
#include "meteor/graphics/image.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <stdexcept>

namespace mtr
{
	Window::Window(const Config& config)
	{
		glfwWindowHint(GLFW_VISIBLE, false);
		glfwWindowHint(GLFW_RESIZABLE, config.window_resizable());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.opengl_version_major());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.opengl_version_minor());
		glfwWindowHint(GLFW_SAMPLES, static_cast<int>(config.window_sample_count()));

		m_handle = glfwCreateWindow(
			static_cast<int>(config.window_width()), 
			static_cast<int>(config.window_height()),
			config.window_title().data(), nullptr, nullptr);

		int window_width = 0, window_height = 0;
		glfwGetWindowSize(m_handle, &window_width, &window_height);

		const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int window_x = (video_mode->width - window_width) / 2;
		int window_y = (video_mode->height - window_height) / 2;
		glfwSetWindowPos(m_handle, window_x, window_y);

		if (!config.window_icon_path().empty())
		{
			Image image(config.window_icon_path());

			GLFWimage icon;
			icon.width = image.width();
			icon.height = image.height();
			icon.pixels = const_cast<uint8_t*>(image.data());

			glfwSetWindowIcon(m_handle, 1, &icon);
		}

		glfwMakeContextCurrent(m_handle);
		glfwSwapInterval(1);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			throw std::runtime_error("failed to load OpenGL");
		}
	}

	Window::Window(Window&& other) noexcept :
		m_handle(other.m_handle)
	{
		other.m_handle = nullptr;
	}

	Window::~Window()
	{
		hide();
		glfwDestroyWindow(m_handle);
	}

	void Window::hide() const
	{
		glfwHideWindow(m_handle);
	}

	void Window::show() const
	{
		glfwShowWindow(m_handle);
	}

	void Window::swap_buffers() const
	{
		glfwSwapBuffers(m_handle);
	}

	void Window::set_cursor_mode(CursorMode cursorMode)
	{
		glfwSetInputMode(m_handle, GLFW_CURSOR, static_cast<int>(cursorMode));
	}

	void Window::set_title(std::string_view title)
	{
		glfwSetWindowTitle(m_handle, title.data());
	}

	void Window::set_fullscreen(bool fullscreen)
	{
		if (fullscreen)
		{
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(m_handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{

		}
	}

	GLFWwindow* Window::glfw_handle() const
	{
		return m_handle;
	}

	glm::uvec2 Window::size() const
	{
		int width = 0, height = 0;
		glfwGetWindowSize(m_handle, &width, &height);

		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}

	glm::uvec2 Window::framebuffer_size() const
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(m_handle, &width, &height);

		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	}
}