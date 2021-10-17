#pragma once

#include "meteor/core/non_copyable.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string_view>

namespace mtr
{
	class Image : NonCopyable
	{
	public:
		Image() = default;
		explicit Image(std::string_view path);
		Image(Image&& other) noexcept;
		~Image();

		Image& operator=(Image&& other) noexcept;

		const uint8_t* data() const;
		uint32_t width() const;
		uint32_t height() const;
		glm::uvec2 size() const;

	private:
		void dispose();

		uint8_t* m_data = nullptr;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};
}
