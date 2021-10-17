#pragma once

namespace mtr
{
	enum class InputAxisValue
	{
		Negative,
		None,
		Positive
	};

	class InputAxis
	{
	public:
		InputAxis(InputAxisValue value = InputAxisValue::None);

		void reset();
		void press_negative_axis();
		void press_positive_axis();
		void release_negative_axis();
		void release_positive_axis();

		InputAxisValue value() const;

	private:
		InputAxisValue m_value = InputAxisValue::None;
	};
}