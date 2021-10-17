#include "meteor/graphics/opengl/shader_sources.h"

namespace mtr::gl
{
	ShaderSources::ShaderSources(std::string&& vertex, std::string&& fragment) :
		vertex{ std::move(vertex) },
		fragment{ std::move(fragment) }
	{}

	ShaderSources::ShaderSources(const char* vertex, const char* fragment) :
		vertex{ vertex },
		fragment{ fragment }
	{}

	bool ShaderSources::valid() const
	{
		return true; // TODO: Check values
	}
}