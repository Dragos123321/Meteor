#include "meteor/core/errors.h"

#include <fmt/core.h>
#include <fmt/format.h>

namespace mtr
{
	/*
		FileNotFoundError
	*/
	FileNotFoundError::FileNotFoundError(std::string_view file) :
		std::runtime_error{ fmt::format("file '{}' was not found", file) },
		m_file{ file }
	{}

	const std::string& FileNotFoundError::file() const
	{
		return m_file;
	}

	/*
		InvalidFormatError
	*/
	InvalidFormatError::InvalidFormatError(std::string_view file) :
		std::runtime_error{ fmt::format("file '{}' has an invalid format", file) },
		m_file{ file }
	{}

	const std::string& InvalidFormatError::file() const
	{
		return m_file;
	}
}