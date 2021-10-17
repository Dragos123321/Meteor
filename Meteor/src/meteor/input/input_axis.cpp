#include "meteor/input/input_axis.h"

namespace mtr
{
	InputAxis::InputAxis(InputAxisValue value) :
		m_value{ value }
	{}

	void InputAxis::reset()
	{
		m_value = InputAxisValue::None;
	}

	void InputAxis::press_negative_axis()
	{
		m_value = InputAxisValue::Negative;
	}

	void InputAxis::press_positive_axis()
	{
		m_value = InputAxisValue::Positive;
	}

	void InputAxis::release_negative_axis()
	{
		if (m_value == InputAxisValue::Negative)
		{
			m_value = InputAxisValue::None;
		}
	}

	void InputAxis::release_positive_axis()
	{
		if (m_value == InputAxisValue::Positive)
		{
			m_value = InputAxisValue::None;
		}
	}

	InputAxisValue InputAxis::value() const
	{
		return m_value;
	}
}