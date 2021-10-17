#pragma once

#include "meteor/core/non_copyable.h"

#include <glfw/glfw3.h>

#include <stdexcept>

namespace mtr
{
	enum class GlfwSetting
	{
		None,
		WindowTitle,
		CursorMode
	};

	enum class CursorMode
	{
		Normal = GLFW_CURSOR_NORMAL,
		Hidden = GLFW_CURSOR_HIDDEN,
		Disabled = GLFW_CURSOR_DISABLED
	};

	class GlfwContext : NonCopyable, NonMovable
	{
	public:
		GlfwContext();
		~GlfwContext();

		GlfwSetting reset_setting();

		void set_window_title(std::string_view window_title);
		void set_cursor_mode(CursorMode cursor_mode);		

		const std::string& window_title() const;
		CursorMode cursor_mode() const;

	private:
		static void glfw_error_callback(int code, const char* description);

		GlfwSetting m_setting = GlfwSetting::None;
		std::string m_window_title;
		CursorMode m_cursor_mode = CursorMode::Normal;
	};

	enum class GlfwErrorCode
	{
		NoError = GLFW_NO_ERROR,
		NotInitialized = GLFW_NOT_INITIALIZED,
		NoCurrentContext = GLFW_NO_CURRENT_CONTEXT,
		InvalidEnum = GLFW_INVALID_ENUM,
		InvalidValue = GLFW_INVALID_VALUE,
		OutOfMemory = GLFW_OUT_OF_MEMORY,
		ApiUnavailable = GLFW_API_UNAVAILABLE,
		VersionUnavailable = GLFW_VERSION_UNAVAILABLE,
		PlatformError = GLFW_PLATFORM_ERROR,
		FormatUnavailable = GLFW_FORMAT_UNAVAILABLE
	};

	class GlfwError : public std::runtime_error
	{
	public:
		GlfwError(GlfwErrorCode code, const char* description);

		GlfwErrorCode code() const;

	private:
		GlfwErrorCode m_code = GlfwErrorCode::NoError;
	};
}