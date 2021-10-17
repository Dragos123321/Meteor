#pragma once

#include <glm/glm.hpp>

#include <string>

namespace mtr
{
	class Config
	{
	public:
		Config() = default;

		void set_window_title(std::string_view window_title);
		void set_window_icon_path(std::string_view window_icon_path);
		void set_window_size(uint32_t window_width, uint32_t window_height);
		void set_window_size(const glm::uvec2& window_size);
		void set_window_resizable(bool window_resizable);
		void set_window_sample_count(size_t window_samples);
		void set_opengl_version(int major, int minor);
		void set_target_fps(float target_fps);
		void set_thread_pool_worker_count(size_t thread_pool_worker_count);

		const std::string& window_title() const;
		const std::string& window_icon_path() const;
		const glm::uvec2& window_size() const;
		uint32_t window_width() const;
		uint32_t window_height() const;
		bool window_resizable() const;
		int opengl_version_major() const;
		int opengl_version_minor() const;
		size_t window_sample_count() const;
		float target_fps() const;
		size_t thread_pool_worker_count() const;

	private:
		std::string m_window_title = "Meteor Game Engine";
		std::string m_window_icon_path;
		glm::uvec2 m_window_size{ 640, 480 };
		bool m_window_resizable = false;
		size_t m_window_sample_count = 0;
		int m_opengl_version_major = 1;
		int m_opengl_version_minor = 0;
		float m_target_fps = 60.f;
		size_t m_thread_pool_worker_count = 0;
	};
}