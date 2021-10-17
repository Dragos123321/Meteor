#pragma once

#include <glm/vec3.hpp>

namespace mtr 
{
	struct SpotLight
	{
		constexpr SpotLight() = default;
		constexpr SpotLight(const glm::vec3& position, const glm::vec3& direction,
			const glm::vec3& ambient = { 1.f, 1.f, 1.f },
			const glm::vec3& diffuse = { 1.f, 1.f, 1.f },
			const glm::vec3& specular = { 1.f, 1.f, 1.f },
			const glm::vec3& color = { 1.f, 1.f, 1.f }
		);

		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec3 direction = { 0.f, 0.f, 0.f };
		glm::vec3 ambient = { 1.f, 1.f, 1.f };
		glm::vec3 diffuse = { 1.f, 1.f, 1.f };
		glm::vec3 specular = { 1.f, 1.f, 1.f };
		glm::vec3 color = { 1.f, 1.f, 1.f };
		glm::vec3 ecuation_consts = { 1.f, 0.045f, 0.075f };
		float cutoff = 0.f;
		float outer_cutoff = 0.f;
	};

#pragma region Implementation
	constexpr SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient,
		const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& color) :

		position{ position },
		direction{ direction },
		ambient{ ambient },
		diffuse{ diffuse },
		specular{ specular },
		color{ color }
	{}
#pragma endregion
}