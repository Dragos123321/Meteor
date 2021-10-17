#pragma once

namespace mtr
{
	class NonCopyable
	{
	public:
		NonCopyable() = default;

	private:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	class NonMovable
	{
	public:
		NonMovable() = default;

	private:
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;
	};

	class NonMoveAssignable
	{
	public:
		NonMoveAssignable() = default;

	private:
		NonMoveAssignable& operator=(NonMoveAssignable&&) = delete;
	};
}