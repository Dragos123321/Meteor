#pragma once

#include "meteor/core/non_copyable.h"

#include <glad/glad.h>

#include <stdexcept>
#include <string_view>

namespace mtr::gl
{
	enum class ShaderType
	{
		None = -1,
		Vertex = GL_VERTEX_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		TessControl = GL_TESS_CONTROL_SHADER,
		TessEvaluation = GL_TESS_EVALUATION_SHADER,
		Compute = GL_COMPUTE_SHADER
	};

	class Shader : NonCopyable
	{
	public:
		Shader(std::string_view source, ShaderType type);
		Shader(Shader&& other) noexcept;
		~Shader();

		Shader& operator=(Shader&& other) noexcept;

		GLuint id() const;
		ShaderType type() const;
		
	private:
		void dispose();
		void check_compile_errors();

		GLuint m_id = 0;
		ShaderType m_type;
	};

	class ShaderCompileError : public std::runtime_error
	{
	public:
		ShaderCompileError(std::string&& info_log, ShaderType shader_type);

		const std::string& info_log() const;
		ShaderType shader_type() const;

	private:
		std::string m_info_log;
		ShaderType m_shader_type;
	};
}
