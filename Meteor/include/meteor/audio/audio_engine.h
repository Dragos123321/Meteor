#pragma once

#include "meteor/audio/audio_stream.h"
#include "meteor/core/non_copyable.h"

#include <future>
#include <vector>

namespace mtr
{
	class ThreadPool;

	class AudioEngine : NonCopyable, NonMovable
	{
	public:
		AudioEngine();

		void add_stream(const std::shared_ptr<AudioStreamData>& stream);

		void begin_update(ThreadPool& thread_pool);
		void end_update();

	private:
		std::vector<std::weak_ptr<AudioStreamData>> m_streams;
		std::vector<std::future<void>> m_futures;
	};

	extern AudioEngine* AUDIO;
}
