#include "meteor/core/fps_limiter.h"

namespace mtr
{
	using namespace std::chrono_literals;

	FpsLimiter::FpsLimiter(float target_fps) :
		m_start_time{ Clock::now() },
		m_target_fps{ target_fps },
		m_last_time{ 0ns },
		m_accumulated_time{ 0ns },
		m_last_timer{ 0ns },
		m_delta_time{
			static_cast<long long>(1.0 / static_cast<double>(target_fps) * 1'000'000'000.0)
		},
		m_raw_tps{ static_cast<uint32_t>(target_fps) },
		m_raw_fps{ static_cast<uint32_t>(target_fps) }
	{}

	FpsLimiter::Duration FpsLimiter::elapsed_time() const
	{
		return Clock::now() - m_start_time;
	}

	uint32_t FpsLimiter::raw_tps() const
	{
		return m_raw_tps;
	}

	uint32_t FpsLimiter::raw_fps() const
	{
		return m_raw_fps;
	}

	void FpsLimiter::begin_frame()
	{
		auto current_time = elapsed_time();

		m_accumulated_time += current_time - m_last_time;
		m_last_time = current_time;
		m_should_draw = false;
	}

	bool FpsLimiter::should_update()
	{
		if (m_accumulated_time >= m_delta_time)
		{
			m_accumulated_time -= m_delta_time;
			m_should_draw = true;
			m_curr_tps++;

			return true;
		}

		return false;
	}

	bool FpsLimiter::should_draw()
	{
		if (m_should_draw)
		{
			m_curr_fps++;
		}

		return m_should_draw;
	}

	void FpsLimiter::end_frame()
	{
		if (elapsed_time() - m_last_timer >= 1s)
		{
			m_raw_tps = m_curr_tps;
			m_raw_fps = m_curr_fps;
			m_curr_tps = 0;
			m_curr_fps = 0;

			m_last_timer += 1s;
		}
	}
}