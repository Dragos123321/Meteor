#pragma once

#include "meteor/audio/audio_engine.h"
#include "meteor/audio/openal.h"
#include "meteor/core/fps_limiter.h"
#include "meteor/core/glfw.h"
#include "meteor/core/thread_pool.h"
#include "meteor/core/window.h"
#include "meteor/input/event_variant.h"
#include "meteor/graphics/opengl/context.h"

namespace mtr
{
	class Config;

	class Application : NonCopyable, NonMovable
	{
	public:
		explicit Application(const Config& config);
		virtual ~Application() = default;

		void quit();
		void run();

		virtual CloseAction on_close_request() { return CloseAction::Quit; }
		virtual void on_key_press(const KeyPressEvent& event) {}
		virtual void on_key_release(const KeyReleaseEvent& event) {}
		virtual void on_button_press(const ButtonPressEvent& event) {}
		virtual void on_button_release(const ButtonReleaseEvent& event) {}
		virtual void on_mouse_move(const MouseMoveEvent& event) {}
		virtual void on_scroll(const ScrollEvent& event) {}
		virtual void on_resize(const ResizeEvent& event) {}
		virtual void on_frame_resize(const FrameResizeEvent& event) {}

		virtual void update() {}
		virtual void draw() {}

		void swap_buffers() const;

		void set_key_repeat_enabled(bool key_repeat_enabled);
		void set_mouse_tracking_enabled(bool mouse_tracking_enabled);
		void set_window_title(std::string_view window_title);
		void set_cursor_mode(CursorMode cursor_mode);

		uint32_t window_width() const;
		uint32_t window_height() const;
		bool mouse_tracking_enabled() const;
		bool key_repeat_enabled() const;

		uint32_t raw_tps() const;
		uint32_t raw_fps() const;

	protected:
		ThreadPool m_thread_pool;

	private:
		std::future<void> start_game_loop();
		void handle_events();
		void handle_glfw_settings();

		static void glfw_close_requested_callback(GLFWwindow*);
		static void glfw_key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods);
		static void glfw_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void glfw_mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
		static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfw_resize_callback(GLFWwindow* window, int width, int height);
		static void glfw_frame_resize_callback(GLFWwindow* window, int width, int height);
	
		GlfwContext m_glfw; // Must be first
		OpenALContext m_openal;

		Window m_window;
		uint32_t m_window_width = 0;
		uint32_t m_window_height = 0;

		std::queue<EventVariant> m_event_queue;
		FpsLimiter m_fps_limiter;
		AudioEngine m_audio;
		gl::Context m_opengl;

		std::atomic_bool m_running = true;
		std::thread m_game_loop_thread;
		std::mutex m_event_mutex;
		std::mutex m_glfw_mutex;

		std::atomic_bool m_key_repeat_enabled = false;
		std::atomic_bool m_mouse_tracking_enabled = false;
	};
}
