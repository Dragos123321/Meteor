#pragma once

#include <stdexcept>

namespace mtr
{
	class FileNotFoundError : public std::runtime_error
	{
	public:
		explicit FileNotFoundError(std::string_view file);

		const std::string& file() const;

	private:
		std::string m_file;
	};

	class InvalidFormatError : public std::runtime_error
	{
	public:
		explicit InvalidFormatError(std::string_view file);

		const std::string& file() const;

	private:
		std::string m_file;
	};
}
