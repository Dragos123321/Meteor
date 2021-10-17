#include "meteor/audio/audio_data.h"

#include <cstdlib>
#include <utility>

namespace mtr
{
	/*
		AudioData
	*/
	AudioData::AudioData(int16_t* output, int channel_count, int sample_rate, int sample_count) :
		m_output{ output },
		m_channel_count{ channel_count },
		m_sample_rate{ sample_rate },
		m_sample_count{ sample_count }
	{}

	AudioData::AudioData(AudioData&& other) noexcept :
		m_output{ other.m_output },
		m_channel_count{ other.m_channel_count },
		m_sample_rate{ other.m_sample_rate },
		m_sample_count{ other.m_sample_count }
	{
		other.m_output = nullptr;
		other.m_channel_count = 0;
		other.m_sample_rate = 0;
		other.m_sample_count = 0;
	}

	AudioData& AudioData::operator=(AudioData&& other) noexcept
	{
		m_output = other.m_output;
		m_channel_count = other.m_channel_count;
		m_sample_rate = other.m_sample_rate;
		m_sample_count = other.m_sample_count;

		other.m_output = nullptr;
		other.m_channel_count = 0;
		other.m_sample_rate = 0;
		other.m_channel_count = 0;

		return *this;
	}

	int16_t* AudioData::output() const
	{
		return m_output;
	}

	int AudioData::output_size() const
	{
		return m_sample_count * m_channel_count * static_cast<int>(sizeof(int16_t));
	}

	int AudioData::channel_count() const
	{
		return m_channel_count;
	}

	int AudioData::sample_rate() const
	{
		return m_sample_rate;
	}

	int AudioData::sample_count() const
	{
		return m_sample_count;
	}

	bool AudioData::is_empty() const
	{
		return m_sample_count == 0;
	}

	/*
		OwnedAudioData
	*/
	OwnedAudioData::OwnedAudioData(OwnedAudioData&& other) noexcept :
		AudioData{ std::move(other) }
	{}

	OwnedAudioData::~OwnedAudioData()
	{
		if (m_output) free(m_output);
	}

	OwnedAudioData& OwnedAudioData::operator=(OwnedAudioData&& other) noexcept
	{
		if (m_output) free(m_output);

		AudioData::operator=(std::move(other));

		return *this;
	}

	OwnedAudioData::OwnedAudioData(int16_t* output, int channel_count, int sample_rate, int sample_count) :
		AudioData{ output, channel_count, sample_rate, sample_count }
	{}
}