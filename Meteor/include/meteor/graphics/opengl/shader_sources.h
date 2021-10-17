#pragma once

#include <string>

namespace mtr::gl
{
	class ShaderSources
	{
	public:
		ShaderSources() = default;
		ShaderSources(std::string&& vertex, std::string&& fragment);
		ShaderSources(const char* vertex, const char* fragment);

		bool valid() const;

		std::string vertex;
		std::string fragment;
		std::string geometry;
		std::string tess_control;
		std::string tess_evaluation;
	};
}