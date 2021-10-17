#pragma once

#include "meteor/core/non_copyable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace mtr::gl
{
	class ProgramBinder;
	class ShaderSources;
	class Uniform;

	class Program : NonCopyable
	{
	public:
		Program() = default;
		Program(std::string_view vertex_shader_source, std::string_view fragment_shader_source);
		explicit Program(const ShaderSources& sources);
		Program(Program&& other) noexcept;
		~Program();

		Program& operator=(Program&& other) noexcept;

		GLint uniform_location(const std::string& uniform_name);

		bool valid() const;
		GLuint id() const;

	private:
		void dispose();
		GLint query_int(GLenum query) const;
		void check_link_errors();

		GLuint m_id = 0;
		std::unordered_map<std::string, GLint> m_uniforms;
	};

	class ProgramBinder : NonCopyable, NonMovable
	{
	public:
		explicit ProgramBinder(Program& program);
		~ProgramBinder();

		Uniform operator[](std::string_view uniform_name);

	private:
		Program* m_program = nullptr;
	};

	class Uniform
	{
	public:
		Uniform(std::string_view name, Program& program);

		void operator=(GLfloat value);
		void operator=(const glm::vec2& value);
		void operator=(const glm::vec3& value);
		void operator=(const glm::vec4& value);

		void operator=(GLdouble value);
		void operator=(const glm::dvec2& value);
		void operator=(const glm::dvec3& value);
		void operator=(const glm::dvec4& value);

		void operator=(GLint value);
		void operator=(const glm::ivec2& value);
		void operator=(const glm::ivec3& value);
		void operator=(const glm::ivec4& value);

		void operator=(GLuint value);
		void operator=(const glm::uvec2& value);
		void operator=(const glm::uvec3& value);
		void operator=(const glm::uvec4& value);

		void operator=(GLboolean value);
		void operator=(const glm::bvec2& value);
		void operator=(const glm::bvec3& value);
		void operator=(const glm::bvec4& value);

		void operator=(const glm::mat2& value);
		void operator=(const glm::mat3& value);
		void operator=(const glm::mat4& value);

		Uniform& operator[](std::string_view field_name);
		Uniform& operator[](size_t index);

		std::string name() const;

	private:
		GLint location();

		std::ostringstream m_name;
		Program* m_program = nullptr;
	};

	class ProgramLinkError : public std::runtime_error
	{
	public:
		explicit ProgramLinkError(std::string&& info_log);

		const std::string& info_log() const;

	private:
		std::string m_info_log;
	};

	class InvalidUniformError : public std::runtime_error
	{
	public:
		explicit InvalidUniformError(std::string_view uniform_name);

		const std::string& uniform_name() const;

	private:
		std::string m_uniform_name;
	};
}