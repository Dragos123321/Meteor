#pragma once

#include <glm/vec3.hpp>

namespace mtr 
{
	struct PointLight
	{
		constexpr PointLight() = default;
		explicit constexpr PointLight(const glm::vec3& position,
			const glm::vec3& ambient = { 1.f, 1.f, 1.f },
			const glm::vec3& diffuse = { 1.f, 1.f, 1.f },
			const glm::vec3& specular = { 1.f, 1.f, 1.f },
			const glm::vec3& color = { 1.f, 1.f, 1.f });

		glm::vec3 position = { 1.f, 1.f, 1.f };
		glm::vec3 ambient = { 1.f, 1.f, 1.f };
		glm::vec3 diffuse = { 1.f, 1.f, 1.f };
		glm::vec3 specular = { 1.f, 1.f, 1.f };
		glm::vec3 color = { 1.f, 1.f, 1.f };
		glm::vec3 ecuation_consts = { 1.0f, 0.045f, 0.075f };
	};

#pragma region Implementation
	constexpr PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient,
		const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& color) :

		position{ position },
		ambient{ ambient },
		diffuse{ diffuse },
		specular{ specular },
		color{ color }
	{}
#pragma endregion
}