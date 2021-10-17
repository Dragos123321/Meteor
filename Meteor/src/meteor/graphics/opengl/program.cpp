#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/shader.h"
#include "meteor/graphics/opengl/shader_sources.h"

#include <fmt/format.h>

namespace mtr::gl
{
	/*
		Program
	*/
	Program::Program(std::string_view vertex_shader_source, std::string_view fragment_shader_source) :
		Program{ ShaderSources(vertex_shader_source.data(), fragment_shader_source.data()) }
	{}

	Program::Program(const ShaderSources& sources)
	{
		m_id = glCreateProgram();

		if (!sources.valid())
			throw std::runtime_error("invalid shader sources");

		Shader vert_shader(sources.vertex, ShaderType::Vertex);
		Shader frag_shader(sources.fragment, ShaderType::Fragment);

		glAttachShader(m_id, vert_shader.id());
		glAttachShader(m_id, frag_shader.id());
		glLinkProgram(m_id);

		try
		{
			check_link_errors();
		}
		catch (...)
		{
			glDetachShader(m_id, vert_shader.id());
			glDetachShader(m_id, frag_shader.id());
			throw;
		}

		glDetachShader(m_id, vert_shader.id());
		glDetachShader(m_id, frag_shader.id());
	}

	Program::Program(Program&& other) noexcept :
		m_id{ other.m_id },
		m_uniforms{ std::move(other.m_uniforms) }
	{
		other.m_id = 0;
	}

	Program::~Program()
	{
		dispose();
	}

	Program& Program::operator=(Program&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		m_uniforms = std::move(other.m_uniforms);

		other.m_id = 0;

		return *this;
	}

	GLint Program::uniform_location(const std::string& uniform_name)
	{
		auto uniform_it = m_uniforms.find(uniform_name);

		if (uniform_it != m_uniforms.end())
		{
			return uniform_it->second;
		}

		GLint uniform_location = glGetUniformLocation(m_id, uniform_name.data());

		if (uniform_location == -1)
			throw InvalidUniformError(uniform_name);

		m_uniforms.insert({ uniform_name, uniform_location });
		return uniform_location;
	}

	bool Program::valid() const
	{
		return m_id;
	}

	GLuint Program::id() const
	{
		return m_id;
	}

	void Program::dispose()
	{
		if (m_id) glDeleteProgram(m_id);
	}

	GLint Program::query_int(GLenum query) const
	{
		GLint value = 0;
		glGetProgramiv(m_id, query, &value);

		return value;
	}

	void Program::check_link_errors()
	{
		GLint success = query_int(GL_LINK_STATUS);

		if (!success)
		{
			GLint info_log_length = query_int(GL_INFO_LOG_LENGTH);
			std::string info_log(static_cast<size_t>(info_log_length), ' ');
			glGetProgramInfoLog(m_id, info_log_length, &info_log_length, info_log.data());

			throw ProgramLinkError(std::move(info_log));
		}
	}

	/*
		ProgramBinder
	*/
	ProgramBinder::ProgramBinder(Program& program) :
		m_program{ &program }
	{
		CONTEXT->bind_program(program.id());
	}

	ProgramBinder::~ProgramBinder()
	{
		if (m_program) CONTEXT->unbind_program();
	}

	Uniform ProgramBinder::operator[](std::string_view uniform_name)
	{
		return Uniform(uniform_name, *m_program);
	}

	/*
		Uniform
	*/
	Uniform::Uniform(std::string_view name, Program& program) :
		m_program{ &program }
	{
		m_name << name.data();
	}

	void Uniform::operator=(GLfloat value)
	{
		glUniform1f(location(), value);
	}

	void Uniform::operator=(const glm::vec2& value)
	{
		glUniform2f(location(), value[0], value[1]);
	}

	void Uniform::operator=(const glm::vec3& value)
	{
		glUniform3f(location(), value[0], value[1], value[2]);
	}

	void Uniform::operator=(const glm::vec4& value)
	{
		glUniform4f(location(), value[0], value[1], value[2], value[3]);
	}

	void Uniform::operator=(GLdouble value)
	{
		glUniform1d(location(), value);
	}

	void Uniform::operator=(const glm::dvec2& value)
	{
		glUniform2d(location(), value[0], value[1]);
	}

	void Uniform::operator=(const glm::dvec3& value)
	{
		glUniform3d(location(), value[0], value[1], value[2]);
	}

	void Uniform::operator=(const glm::dvec4& value)
	{
		glUniform4d(location(), value[0], value[1], value[2], value[3]);
	}

	void Uniform::operator=(GLint value)
	{
		glUniform1i(location(), value);
	}

	void Uniform::operator=(const glm::ivec2& value)
	{
		glUniform2i(location(), value[0], value[1]);
	}

	void Uniform::operator=(const glm::ivec3& value)
	{
		glUniform3i(location(), value[0], value[1], value[2]);
	}

	void Uniform::operator=(const glm::ivec4& value)
	{
		glUniform4i(location(), value[0], value[1], value[2], value[3]);
	}

	void Uniform::operator=(GLuint value)
	{
		glUniform1ui(location(), value);
	}

	void Uniform::operator=(const glm::uvec2& value)
	{
		glUniform2ui(location(), value[0], value[1]);
	}

	void Uniform::operator=(const glm::uvec3& value)
	{
		glUniform3ui(location(), value[0], value[1], value[2]);
	}

	void Uniform::operator=(const glm::uvec4& value)
	{
		glUniform4ui(location(), value[0], value[1], value[2], value[3]);
	}

	void Uniform::operator=(GLboolean value)
	{
		glUniform1i(location(), value);
	}

	void Uniform::operator=(const glm::bvec2& value)
	{
		glUniform2i(location(), value[0], value[1]);
	}

	void Uniform::operator=(const glm::bvec3& value)
	{
		glUniform3i(location(), value[0], value[1], value[2]);
	}

	void Uniform::operator=(const glm::bvec4& value)
	{
		glUniform4i(location(), value[0], value[1], value[2], value[3]);
	}

	void Uniform::operator=(const glm::mat2& value)
	{
		glUniformMatrix2fv(location(), 1, false, &value[0][0]);
	}

	void Uniform::operator=(const glm::mat3& value)
	{
		glUniformMatrix3fv(location(), 1, false, &value[0][0]);
	}

	void Uniform::operator=(const glm::mat4& value)
	{
		glUniformMatrix4fv(location(), 1, false, &value[0][0]);
	}

	Uniform& Uniform::operator[](std::string_view field_name)
	{
		m_name << '.' << field_name;
		return *this;
	}

	Uniform& Uniform::operator[](size_t index)
	{
		m_name << '[' << index << ']';
		return *this;
	}

	std::string Uniform::name() const
	{
		return m_name.str();
	}

	GLint Uniform::location()
	{
		return m_program->uniform_location(name());
	}

	/*
		ProgramLinkError
	*/
	ProgramLinkError::ProgramLinkError(std::string&& info_log) :
		std::runtime_error{ fmt::format("failed to link program: '{}'", info_log) },
		m_info_log{ std::move(info_log) }
	{}

	const std::string& ProgramLinkError::info_log() const
	{
		return m_info_log;
	}

	/*
		InvalidUniformError
	*/
	InvalidUniformError::InvalidUniformError(std::string_view uniform_name) :
		std::runtime_error{ fmt::format("uniform '{}' was not found in shaders", uniform_name) },
		m_uniform_name{ uniform_name }
	{}

	const std::string& InvalidUniformError::uniform_name() const
	{
		return m_uniform_name;
	}
}