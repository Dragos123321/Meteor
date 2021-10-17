#pragma once

#include "meteor/core/non_copyable.h"

#include <cstdint>

namespace mtr
{
	class AudioDecoder;
	class AudioStreamData;

	class AudioData : NonCopyable
	{
		friend class AudioDecoder;
		friend class AudioStreamData;

	public:
		AudioData(AudioData&& other) noexcept;
		virtual ~AudioData() = default;

		AudioData& operator=(AudioData&& other) noexcept;

		int16_t* output() const;
		int output_size() const;
		int channel_count() const;
		int sample_rate() const;
		int sample_count() const;
		bool is_empty() const;

	protected:
		AudioData() = default;
		AudioData(int16_t* output, int channel_count, int sample_rate, int sample_count);

		int16_t* m_output = nullptr;
		int m_channel_count = 0;
		int m_sample_rate = 0;
		int m_sample_count = 0;
	};

	class OwnedAudioData : public AudioData
	{
		friend class AudioDecoder;
		friend class AudioStreamData;

	public:
		OwnedAudioData(OwnedAudioData&& other) noexcept;
		~OwnedAudioData();

		OwnedAudioData& operator=(OwnedAudioData&& other) noexcept;

	private:
		OwnedAudioData(int16_t* output, int channel_count, int sample_rate, int sample_count);
	};
}
