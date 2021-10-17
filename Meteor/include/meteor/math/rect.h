#pragma once

#include <glm/vec2.hpp>

namespace mtr
{
	template<typename T>
	struct Rect
	{
		using Vec2 = glm::tvec2<T>;

		static Rect<T> with_position(const Vec2& position);
		static Rect<T> with_size(const Vec2& size);
			
		Rect();
		Rect(T x, T y, T w, T h);

		void set_left(T left);
		void set_right(T right);
		void set_top(T top);
		void set_bottom(T bottom);

		void set_size(const Vec2& size);

		T top() const;
		T bottom() const;
		T left() const;
		T right() const;

		Vec2 position() const;
		Vec2 center() const;
		Vec2 top_left() const;
		Vec2 top_right() const;
		Vec2 bottom_right() const;
		Vec2 bottom_left() const;
		Vec2 size() const;

		bool contains(const Vec2& point) const;
		bool intersects(const Rect& other) const;

		T x, y, w, h;
	};

	using IntRect = Rect<int>;
	using UintRect = Rect<unsigned int>;
	using FloatRect = Rect<float>;
	using DoubleRect = Rect<double>;

#pragma region Implementation
	template<typename T>
	Rect<T> Rect<T>::with_position(const Vec2& position)
	{
		return Rect(position.x, position.y, T(), T());
	}

	template<typename T>
	Rect<T> Rect<T>::with_size(const Vec2& size)
	{
		return Rect(T(), T(), size.x, size.y);
	}

	template<typename T>
	Rect<T>::Rect() :
		x{ T() }, y{ T() }, w{ T() }, h{ T() }
	{}

	template<typename T>
	Rect<T>::Rect(T x, T y, T w, T h) :
		x{ x }, y{ y }, w{ w }, h{ h }
	{}

	template<typename T>
	void Rect<T>::set_left(T left)
	{
		x = left;
	}

	template<typename T>
	void Rect<T>::set_right(T right)
	{
		x = right - w;
	}

	template<typename T>
	void Rect<T>::set_top(T top)
	{
		y = top;
	}

	template<typename T>
	void Rect<T>::set_bottom(T bottom)
	{
		y = bottom - h;
	}

	template<typename T>
	void Rect<T>::set_size(const Vec2& size)
	{
		w = size.x;
		h = size.y;
	}

	template<typename T>
	T Rect<T>::top() const
	{
		return y;
	}

	template<typename T>
	T Rect<T>::bottom() const
	{
		return y + h;
	}

	template<typename T>
	inline T Rect<T>::left() const
	{
		return x;
	}

	template<typename T>
	T Rect<T>::right() const
	{
		return x + w;
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::position() const
	{
		return { x, y };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::center() const
	{
		return { x + w / 2, y + h / 2 };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::top_left() const
	{
		return { x, y };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::top_right() const
	{
		return { x + w, y };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::bottom_right() const
	{
		return { x + w, y + h };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::bottom_left() const
	{
		return { x, y + h };
	}

	template<typename T>
	typename Rect<T>::Vec2 Rect<T>::size() const
	{
		return { w, h };
	}

	template<typename T>
	bool Rect<T>::contains(const Vec2& point) const
	{
		return
			point.x >= x && point.x <= right() &&
			point.y >= y && point.y <= bottom();
	}

	template<typename T>
	bool Rect<T>::intersects(const Rect& other) const
	{
		return
			x <= other.right() && right() >= other.x &&
			y <= other.bottom() && bottom() >= other.y;
	}
#pragma endregion
}