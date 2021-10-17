#include "meteor/core/application.h"
#include "meteor/core/config.h"

#include <glfw/glfw3.h>
#include <stb/stb_image.h>

namespace mtr
{
	Application* APP = nullptr;

	Application::Application(const Config& config) :
		m_window{ config },
		m_thread_pool{ config.thread_pool_worker_count() },
		m_fps_limiter{ config.target_fps() }
	{
		stbi_set_flip_vertically_on_load(true);

		APP = this;
		GLFWwindow* window = m_window.glfw_handle();

		m_window_width = m_window.framebuffer_size().x;
		m_window_height = m_window.framebuffer_size().y;

		glfwSetWindowCloseCallback(window, glfw_close_requested_callback);
		glfwSetKeyCallback(window, glfw_key_callback);
		glfwSetMouseButtonCallback(window, glfw_button_callback);
		glfwSetCursorPosCallback(window, glfw_mouse_move_callback);
		glfwSetScrollCallback(window, glfw_scroll_callback);
		glfwSetWindowSizeCallback(window, glfw_resize_callback);
		glfwSetFramebufferSizeCallback(window, glfw_frame_resize_callback);
	}

	void Application::quit()
	{
		m_running = false;
		glfwPostEmptyEvent();
	}

	void Application::run()
	{
		std::future<void> game_loop_future;

		try
		{
			game_loop_future = start_game_loop();
			m_window.show();

			while (m_running)
			{
				glfwWaitEvents();
				handle_glfw_settings();
			}				
		}
		catch (...)
		{
			m_running = false;
			m_game_loop_thread.join();
			game_loop_future.get();
			throw;
		}

		m_game_loop_thread.join();
		game_loop_future.get();
	}

	std::future<void> Application::start_game_loop()
	{
		glfwMakeContextCurrent(nullptr);

		auto game_loop = std::packaged_task<void()>([this]() {
			try
			{
				glfwMakeContextCurrent(m_window.glfw_handle());
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				while (m_running)
				{
					m_fps_limiter.begin_frame();

					while (m_fps_limiter.should_update())
					{
						// m_audio_x.end_update();
						m_audio.end_update();

						handle_events();
						update();

						// m_audio_x.begin_update(m_thread_pool);
						m_audio.begin_update(m_thread_pool);
					}

					if (m_fps_limiter.should_draw())
					{
						draw();
					}

					m_fps_limiter.end_frame();
				}
			}
			catch (...)
			{
				quit();
				throw;
			}
		});

		auto future = game_loop.get_future();

		m_game_loop_thread = std::thread([game_loop{ std::move(game_loop) }]() mutable {
			game_loop();
		});

		return future;
	}

	void Application::swap_buffers() const
	{
		m_window.swap_buffers();
	}

	void Application::set_key_repeat_enabled(bool key_repeat_enabled)
	{
		m_key_repeat_enabled = key_repeat_enabled;
	}

	void Application::set_mouse_tracking_enabled(bool mouse_tracking_enabled)
	{
		m_mouse_tracking_enabled = mouse_tracking_enabled;
	}

	void Application::set_window_title(std::string_view window_title)
	{
		{
			std::scoped_lock lock(m_glfw_mutex);
			m_glfw.set_window_title(window_title);
		}

		glfwPostEmptyEvent();
	}

	void Application::set_cursor_mode(CursorMode cursor_mode)
	{
		{
			std::scoped_lock lock(m_glfw_mutex);
			m_glfw.set_cursor_mode(cursor_mode);
		}
		
		glfwPostEmptyEvent();
	}

	uint32_t Application::window_width() const
	{
		return m_window_width;
	}

	uint32_t Application::window_height() const
	{
		return m_window_height;
	}

	bool Application::mouse_tracking_enabled() const
	{
		return m_mouse_tracking_enabled;
	}

	bool Application::key_repeat_enabled() const
	{
		return m_key_repeat_enabled;
	}

	uint32_t Application::raw_tps() const
	{
		return m_fps_limiter.raw_tps();
	}

	uint32_t Application::raw_fps() const
	{
		return m_fps_limiter.raw_fps();
	}

	void Application::handle_events()
	{
		std::scoped_lock lock(m_event_mutex);

		try
		{
			while (m_running && !m_event_queue.empty())
			{
				auto& event = m_event_queue.front();

				switch (static_cast<EventType>(event.index()))
				{
				case EventType::CloseRequest:
					if (on_close_request() == CloseAction::Quit)
					{
						quit();
					}
					break;

				case EventType::KeyPress:
					on_key_press(std::get<KeyPressEvent>(event));
					break;

				case EventType::KeyRelease:
					on_key_release(std::get<KeyReleaseEvent>(event));
					break;

				case EventType::ButtonPress:
					on_button_press(std::get<ButtonPressEvent>(event));
					break;

				case EventType::ButtonRelease:
					on_button_release(std::get<ButtonReleaseEvent>(event));
					break;

				case EventType::MouseMove:
					on_mouse_move(std::get<MouseMoveEvent>(event));
					break;

				case EventType::Scroll:
					on_scroll(std::get<ScrollEvent>(event));
					break;

				case EventType::Resize:
					on_resize(std::get<ResizeEvent>(event));
					break;

				case EventType::FrameResize:
				{
					const auto& frame_resize_event = std::get<FrameResizeEvent>(event);
					m_window_width = frame_resize_event.size.x;
					m_window_height = frame_resize_event.size.y;
					on_frame_resize(frame_resize_event);
					break;
				}
				}

				m_event_queue.pop();
			}
		}
		catch (...)
		{
			quit();
			throw;
		}
	}

	void Application::handle_glfw_settings()
	{
		std::scoped_lock lock(m_glfw_mutex);

		switch (m_glfw.reset_setting())
		{
		case GlfwSetting::WindowTitle:
			m_window.set_title(m_glfw.window_title());
			break;

		case GlfwSetting::CursorMode:
			m_window.set_cursor_mode(m_glfw.cursor_mode());
			break;
		}
	}

#pragma region GLFW Callbacks
	void Application::glfw_close_requested_callback(GLFWwindow* window)
	{
		std::scoped_lock lock(APP->m_event_mutex);
		APP->m_event_queue.emplace(CloseRequestEvent());
	}

	void Application::glfw_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:
		{
			std::scoped_lock lock(APP->m_event_mutex);
			APP->m_event_queue.emplace(KeyPressEvent(
				static_cast<KeyCode>(key_code), scancode, Modifiers(mods)));
			break;
		}
		case GLFW_REPEAT:
		{
			if (APP->m_key_repeat_enabled)
			{
				std::scoped_lock lock(APP->m_event_mutex);
				APP->m_event_queue.emplace(KeyPressEvent(
					static_cast<KeyCode>(key_code), scancode, Modifiers(mods), true));
			}
			break;
		}
		case GLFW_RELEASE:
		{
			std::scoped_lock lock(APP->m_event_mutex);
			APP->m_event_queue.emplace(KeyReleaseEvent(
				static_cast<KeyCode>(key_code), scancode, Modifiers(mods)));
			break;
		}
		}
	}

	void Application::glfw_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		glm::vec2 cursor_position = glm::vec2(static_cast<float>(x), static_cast<float>(y));

		switch (action)
		{
		case GLFW_PRESS:
		{
			std::scoped_lock lock(APP->m_event_mutex);
			APP->m_event_queue.emplace(ButtonPressEvent(
				static_cast<MouseButton>(button), cursor_position, Modifiers(mods)));
			break;
		}
		case GLFW_RELEASE:
		{
			std::scoped_lock lock(APP->m_event_mutex);
			APP->m_event_queue.emplace(ButtonReleaseEvent(
				static_cast<MouseButton>(button), cursor_position, Modifiers(mods)));
			break;
		}
		}
	}

	void Application::glfw_mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (APP->m_mouse_tracking_enabled)
		{
			std::scoped_lock lock(APP->m_event_mutex);
			APP->m_event_queue.emplace(MouseMoveEvent({ xpos, ypos }));
		}
	}

	void Application::glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		std::scoped_lock lock(APP->m_event_mutex);
		APP->m_event_queue.emplace(ScrollEvent({ xoffset, yoffset }));
	}

	void Application::glfw_resize_callback(GLFWwindow* window, int width, int height)
	{
		std::scoped_lock lock(APP->m_event_mutex);
		APP->m_event_queue.emplace(ResizeEvent({ width, height }));
	}

	void Application::glfw_frame_resize_callback(GLFWwindow* window, int width, int height)
	{
		std::scoped_lock lock(APP->m_event_mutex);
		APP->m_event_queue.emplace(FrameResizeEvent({ width, height }));
	}
#pragma endregion
}