#include "meteor/core/glfw.h"

#include <stdexcept>

namespace mtr
{
	/*
		GlfwContext
	*/
	GlfwContext::GlfwContext()
	{
		glfwSetErrorCallback(glfw_error_callback);
		glfwInit();
	}

	GlfwContext::~GlfwContext()
	{
		glfwTerminate();
	}

	GlfwSetting GlfwContext::reset_setting()
	{
		GlfwSetting setting = m_setting;
		m_setting = GlfwSetting::None;

		return setting;
	}

	void GlfwContext::set_window_title(std::string_view window_title)
	{
		m_setting = GlfwSetting::WindowTitle;
		m_window_title = window_title;
	}

	void GlfwContext::set_cursor_mode(CursorMode cursor_mode)
	{
		m_setting = GlfwSetting::CursorMode;
		m_cursor_mode = cursor_mode;
	}

	const std::string& GlfwContext::window_title() const
	{
		return m_window_title;
	}

	CursorMode GlfwContext::cursor_mode() const
	{
		return m_cursor_mode;
	}

	void GlfwContext::glfw_error_callback(int code, const char* description)
	{
		throw GlfwError(static_cast<GlfwErrorCode>(code), description);
	}

	/*
		GlfwError
	*/
	GlfwError::GlfwError(GlfwErrorCode code, const char* description) :
		std::runtime_error{ description }
	{}

	GlfwErrorCode GlfwError::code() const
	{
		return m_code;
	}
}