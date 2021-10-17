#pragma once

namespace mtr
{
	enum class SizeType
	{
		Absolute,
		Scale
	};

	struct GridStyle
	{
		constexpr GridStyle() = default;
		constexpr GridStyle(SizeType size_type, float value);

		constexpr float compute_size(float available_space) const;

		SizeType size_type = SizeType::Absolute;
		float value = 0.f;
	};

#pragma region Implementation
	constexpr GridStyle::GridStyle(SizeType size_type, float value) :
		size_type{ size_type },
		value{ value }
	{}

	constexpr float GridStyle::compute_size(float available_space) const
	{
		switch (size_type)
		{
		case SizeType::Absolute:
			return value;

		case SizeType::Scale:
			return value * available_space;

		default:
			return 0.f;
		}
	}
#pragma endregion
}