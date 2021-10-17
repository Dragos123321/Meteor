#include "meteor/core/errors.h"
#include "meteor/graphics/image.h"

#include <stb/stb_image.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace mtr
{
	Image::Image(std::string_view path)
	{
		int channel_count = 0, image_width = 0, image_height = 0;

		m_data = stbi_load(path.data(), &image_width, &image_height, &channel_count, 4);
		m_width = static_cast<uint32_t>(image_width);
		m_height = static_cast<uint32_t>(image_height);

		if (!m_data)
		{
			if (!fs::exists(path))
				throw FileNotFoundError(path);
			else
				throw InvalidFormatError(path);
		}
	}

	Image::Image(Image&& other) noexcept :
		m_data{ other.m_data },
		m_width{ other.m_width },
		m_height{ other.m_height }
	{
		other.m_data = nullptr;
		other.m_width = 0;
		other.m_height = 0;
	}

	Image::~Image()
	{
		dispose();
	}

	Image& Image::operator=(Image&& other) noexcept
	{
		dispose();

		m_data = other.m_data;
		m_width = other.m_width;
		m_height = other.m_height;

		other.m_data = nullptr;
		other.m_width = 0;
		other.m_height = 0;

		return *this;
	}

	const uint8_t* Image::data() const
	{
		return m_data;
	}

	uint32_t Image::width() const
	{
		return m_width;
	}

	uint32_t Image::height() const
	{
		return m_height;
	}

	glm::uvec2 Image::size() const
	{
		return { m_width, m_height };
	}

	void Image::dispose()
	{
		if (m_data) stbi_image_free(m_data);
	}
}