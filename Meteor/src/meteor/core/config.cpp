#pragma once

#include "meteor/core/config.h"

namespace mtr
{
	void Config::set_window_title(std::string_view window_title)
	{
		m_window_title = window_title;
	}

	void Config::set_window_icon_path(std::string_view window_icon_path)
	{
		m_window_icon_path = window_icon_path;
	}

	void Config::set_window_size(uint32_t window_width, uint32_t window_height)
	{
		m_window_size = { window_width, window_height };
	}

	void Config::set_window_size(const glm::uvec2& window_size)
	{
		m_window_size = window_size;
	}

	void Config::set_window_resizable(bool window_resizable)
	{
		m_window_resizable = window_resizable;
	}

	void Config::set_window_sample_count(size_t window_samples)
	{
		m_window_sample_count = window_samples;
	}

	void Config::set_opengl_version(int major, int minor)
	{
		m_opengl_version_major = major;
		m_opengl_version_minor = minor;
	}

	void Config::set_target_fps(float target_fps)
	{
		m_target_fps = target_fps;
	}

	void Config::set_thread_pool_worker_count(size_t thread_pool_worker_count)
	{
		m_thread_pool_worker_count = thread_pool_worker_count;
	}

	const std::string& Config::window_title() const
	{
		return m_window_title;
	}

	const std::string& Config::window_icon_path() const
	{
		return m_window_icon_path;
	}

	const glm::uvec2& Config::window_size() const
	{
		return m_window_size;
	}

	uint32_t Config::window_width() const
	{
		return m_window_size.x;
	}

	uint32_t Config::window_height() const
	{
		return m_window_size.y;
	}

	bool Config::window_resizable() const
	{
		return m_window_resizable;
	}

	int Config::opengl_version_major() const
	{
		return m_opengl_version_major;
	}

	int Config::opengl_version_minor() const
	{
		return m_opengl_version_minor;
	}

	size_t Config::window_sample_count() const
	{
		return m_window_sample_count;
	}

	float Config::target_fps() const
	{
		return m_target_fps;
	}

	size_t Config::thread_pool_worker_count() const
	{
		return m_thread_pool_worker_count;
	}
}