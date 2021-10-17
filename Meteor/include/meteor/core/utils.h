#pragma once

#include "meteor/core/non_copyable.h"

#include <string>

namespace mtr
{
	extern std::string read_to_string(std::string_view path);
	extern void write_to_file(std::string_view path, std::string_view text);
}
