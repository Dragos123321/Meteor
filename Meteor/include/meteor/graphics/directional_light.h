#pragma once

#include <glm/vec3.hpp>

namespace mtr 
{
	class DirectionalLight
	{
	public:
		constexpr DirectionalLight() = default;
		explicit constexpr DirectionalLight(const glm::vec3& direction, 
			const glm::vec3& ambient = { 1.0f, 1.0f, 1.0f },
			const glm::vec3& diffuse = { 1.0f, 1.0f, 1.0f }, 
			const glm::vec3& specular = { 1.0f, 1.0f, 1.0f });

		glm::vec3 direction = { 1.f, 0.f, 0.f };
		glm::vec3 ambient = { 1.f, 1.f, 1.f };
		glm::vec3 diffuse = { 1.f, 1.f, 1.f };
		glm::vec3 specular = { 1.f, 1.f, 1.f };
	};

#pragma region Implementation
	constexpr DirectionalLight::DirectionalLight(const glm::vec3& direction,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) :

		direction{ direction },
		ambient{ ambient },
		diffuse{ diffuse },
		specular{ specular }
	{}
#pragma endregion
}

