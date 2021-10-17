#include "meteor/core/benchmark.h"

#include <fmt/core.h>

namespace mtr
{
	Benchmark::Benchmark(const char* benchmark_name) :
		m_start_time{ Clock::now() },
		m_benchmark_name{ benchmark_name }
	{
		fmt::print("Started '{}'\n", m_benchmark_name);
	}

	Benchmark::~Benchmark()
	{
		auto duration = std::chrono::duration_cast<
			std::chrono::milliseconds>(Clock::now() - m_start_time).count();

		fmt::print("Finished '{}' ({}ms)\n", m_benchmark_name, duration);
	}
}