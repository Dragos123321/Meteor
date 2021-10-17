#include "meteor/core/utils.h"

#include <fstream>

namespace mtr
{
	std::string read_to_string(std::string_view path)
	{
		std::ifstream fin;
		fin.exceptions(std::ios::failbit | std::ios::badbit);
		fin.open(path.data());

		std::string content;
		content.assign(std::istreambuf_iterator<char>(fin), {});
		return content;
	}

	void write_to_file(std::string_view path, std::string_view text)
	{
		std::ofstream fout;
		fout.exceptions(std::ios::failbit | std::ios::badbit);
		fout.open(path.data());

		fout << text;
	}
}