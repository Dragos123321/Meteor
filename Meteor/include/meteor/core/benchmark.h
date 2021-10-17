#pragma once

#include "meteor/core/non_copyable.h"

#include <chrono>

namespace mtr
{
	class Benchmark : NonCopyable, NonMovable
	{
	public:
		Benchmark(const char* benchmark_name);
		~Benchmark();

	private:
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = Clock::time_point;

		TimePoint m_start_time;
		const char* m_benchmark_name;
	};
}