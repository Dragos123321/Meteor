#include "meteor/audio/audio_buffer.h"
#include "meteor/audio/audio_data.h"

namespace mtr
{
	AudioBuffer::AudioBuffer()
	{
		alGenBuffers(1, &m_id);
	}

	AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept :
		m_id{ other.m_id }
	{
		other.m_id = 0;
	}

	AudioBuffer::AudioBuffer(const AudioData& data) :
		AudioBuffer()
	{
		upload_data(data);
	}

	AudioBuffer::~AudioBuffer()
	{
		if (m_id) alDeleteBuffers(1, &m_id);
	}

	AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept
	{
		if (m_id) alDeleteBuffers(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;
		
		return *this;
	}

	void AudioBuffer::upload_data(const AudioData& data)
	{
		alBufferData(m_id, data.channel_count() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
			data.output(), data.output_size(), data.sample_rate());
	}

	int AudioBuffer::sample_rate() const
	{
		int sample_rate = 0;
		alGetBufferi(m_id, AL_FREQUENCY, &sample_rate);

		return sample_rate;
	}

	int AudioBuffer::channel_count() const
	{
		int channel_count = 0;
		alGetBufferi(m_id, AL_CHANNELS, &channel_count);

		return channel_count;
	}

	ALuint AudioBuffer::id() const
	{
		return m_id;
	}
}