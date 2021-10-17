#pragma once

#include "meteor/core/non_copyable.h"

#include <al/al.h>

namespace mtr
{
	class AudioData;

	class AudioBuffer : NonCopyable
	{
	public:
		AudioBuffer();
		AudioBuffer(AudioBuffer&& other) noexcept;
		explicit AudioBuffer(const AudioData& data);
		~AudioBuffer();

		AudioBuffer& operator=(AudioBuffer&& other) noexcept;

		void upload_data(const AudioData& data);

		int sample_rate() const;
		int channel_count() const;
		ALuint id() const;

	private:
		ALuint m_id = 0;
	};
}
