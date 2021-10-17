#pragma once

#include "meteor/math/rect.h"

namespace mtr
{
	template<typename T>
	struct Circle
	{
		using Vec2 = glm::tvec2<T>;
		using Rect = Rect<T>;

		Circle();
		Circle(T x, T y, T r);

		Vec2 position() const;
		Rect bounds() const;

		T x, y, r;
	};

	using FloatCircle = Circle<float>;
	using DoubleCircle = Circle<double>;

	template<typename T>
	Circle<T>::Circle() :
		x{ T() }, y{ T() }, r{ T() }
	{}

	template<typename T>
	Circle<T>::Circle(T x, T y, T r) :
		x{ x }, y{ y }, r{ r }
	{}

	template<typename T>
	typename Circle<T>::Vec2 Circle<T>::position() const
	{
		return { x, y }
	}

	template<typename T>
	typename Circle<T>::Rect Circle<T>::bounds() const
	{
		return { x - r, y - r, r + r, r + r };
	}
}