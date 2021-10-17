#include "meteor/audio/audio_engine.h"
#include "meteor/core/thread_pool.h"

namespace mtr
{
	AudioEngine* AUDIO = nullptr;

	AudioEngine::AudioEngine()
	{
		AUDIO = this;
	}

	void AudioEngine::add_stream(const std::shared_ptr<AudioStreamData>& stream)
	{
		m_streams.push_back(stream);
	}

	void AudioEngine::begin_update(ThreadPool& thread_pool)
	{
		for (size_t i = 0; i < m_streams.size();)
		{
			if (m_streams[i].expired())
			{
				if (m_streams.size() > 1)
				{
					std::swap(m_streams[i], m_streams.back());
				}
				
				m_streams.pop_back();
			}
			else
			{
				auto stream = m_streams[i].lock();

				if (stream->needs_update())
				{
					m_futures.emplace_back(thread_pool.execute([stream]() {
						stream->update();
					}));
				}

				++i;
			}
		}
	}

	void AudioEngine::end_update()
	{
		for (auto& future : m_futures)
		{
			future.get();
		}

		m_futures.clear();
	}
}