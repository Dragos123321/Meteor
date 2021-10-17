#pragma once

#include <chrono>

namespace mtr
{
	class FpsLimiter
	{
	public:
		using Clock = std::chrono::high_resolution_clock;
		using Duration = Clock::duration;
		using TimePoint = Clock::time_point;

		explicit FpsLimiter(float target_fps = 60.f);

		void begin_frame();
		bool should_update();
		bool should_draw();
		void end_frame();

		Duration elapsed_time() const;
		uint32_t raw_tps() const;
		uint32_t raw_fps() const;

	private:
		TimePoint m_start_time;
		float m_target_fps;

		Duration m_last_time;
		Duration m_accumulated_time;
		Duration m_last_timer;
		Duration m_delta_time;
		bool m_should_draw = false;

		uint32_t m_raw_tps;
		uint32_t m_raw_fps;
		uint32_t m_curr_tps = 0;
		uint32_t m_curr_fps = 0;
	};
}
