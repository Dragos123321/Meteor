#pragma once

namespace mtr
{
	struct Padding
	{
		static constexpr Padding uniform(float value);
		static constexpr Padding horizontal(float value);
		static constexpr Padding horizontal(float left, float right);
		static constexpr Padding vertical(float value);
		static constexpr Padding vertical(float top, float bottom);

		constexpr Padding() = default;
		constexpr Padding(float left, float right, float top, float bottom);

		float left = 0.f;
		float right = 0.f;
		float top = 0.f;
		float bottom = 0.f;
	};

#pragma region Implementation
	constexpr Padding Padding::uniform(float value)
	{
		return Padding(value, value, value, value);
	}

	constexpr Padding Padding::horizontal(float value)
	{
		return Padding(value, value, 0.f, 0.f);
	}

	constexpr Padding Padding::horizontal(float left, float right)
	{
		return Padding(left, right, 0.f, 0.f);
	}

	constexpr Padding Padding::vertical(float value)
	{
		return Padding(0.f, 0.f, value, value);
	}

	constexpr Padding Padding::vertical(float top, float bottom)
	{
		return Padding(0.f, 0.f, top, bottom);
	}

	constexpr Padding::Padding(float left, float right, float top, float bottom) :
		left{ left },
		right{ right },
		top{ top },
		bottom{ bottom }
	{}
#pragma endregion
}
