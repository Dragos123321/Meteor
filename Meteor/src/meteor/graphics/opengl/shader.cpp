#include "meteor/graphics/opengl/shader.h"

#include <fmt/core.h>

namespace mtr::gl
{
	/*
		Shader
	*/
	Shader::Shader(std::string_view source, ShaderType type) :
		m_type{ type }
	{
		m_id = glCreateShader(static_cast<GLenum>(type));

		const char* source_ptr = &source[0];
		glShaderSource(m_id, 1, &source_ptr, nullptr);
		glCompileShader(m_id);

		check_compile_errors();
	}

	Shader::Shader(Shader&& other) noexcept :
		m_id{ other.m_id },
		m_type{ other.m_type }
	{
		other.m_id = 0;
		other.m_type = ShaderType::None;
	}

	Shader::~Shader()
	{
		dispose();
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		m_type = other.m_type;

		other.m_id = 0;
		other.m_type = ShaderType::None;

		return *this;
	}

	GLuint Shader::id() const
	{
		return m_id;
	}

	ShaderType Shader::type() const
	{
		return m_type;
	}

	void Shader::dispose()
	{
		if (m_id) glDeleteShader(m_id);
	}

	void Shader::check_compile_errors()
	{
		GLint success = GL_FALSE;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLint info_log_length = 0;
			glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &info_log_length);

			std::string info_log(static_cast<size_t>(info_log_length), ' ');
			glGetShaderInfoLog(m_id, info_log_length, &info_log_length, info_log.data());
			
			throw ShaderCompileError(std::move(info_log), m_type);
		}
	}

	/*
		ShaderCompileError
	*/
	ShaderCompileError::ShaderCompileError(std::string&& info_log, ShaderType shader_type) :
		std::runtime_error{ fmt::format("failed to compile shader: '{}'", info_log) },
		m_info_log{ std::move(info_log) },
		m_shader_type{ shader_type }
	{}

	const std::string& ShaderCompileError::info_log() const
	{
		return m_info_log;
	}

	ShaderType ShaderCompileError::shader_type() const
	{
		return m_shader_type;
	}
}