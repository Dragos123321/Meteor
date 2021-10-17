#include "meteor/audio/audio_buffer.h"
#include "meteor/audio/audio_source.h"
#include "meteor/core/utils.h"

#include <algorithm>

namespace mtr
{
	AudioSource::AudioSource()
	{
		alGenSources(1, &m_id);
	}

	AudioSource::AudioSource(AudioSource&& other) noexcept :
		m_id{ other.m_id }
	{
		other.m_id = 0;
	}

	AudioSource::~AudioSource()
	{
		if (m_id) alDeleteSources(1, &m_id);
	}

	AudioSource& AudioSource::operator=(AudioSource&& other) noexcept
	{
		if (m_id) alDeleteSources(1, &m_id);

		m_id = other.m_id;
		other.m_id = 0;

		return *this;
	}

	void AudioSource::play() const
	{
		alSourcePlay(m_id);
	}

	void AudioSource::pause() const
	{
		alSourcePause(m_id);
	}

	void AudioSource::resume() const
	{
		if (state() == AudioSourceState::Paused) play();
	}

	void AudioSource::stop() const
	{
		alSourceStop(m_id);
	}

	void AudioSource::set_volume(float volume)
	{
		alSourcef(m_id, AL_GAIN, std::clamp(volume, 0.f, 1.f));
	}

	void AudioSource::set_looping(bool looping)
	{
		alSourcei(m_id, AL_LOOPING, looping);
	}

	void AudioSource::set_pitch(float pitch)
	{
		alSourcef(m_id, AL_PITCH, std::clamp(pitch, 0.f, 5.f));
	}

	void AudioSource::set_buffer(const AudioBuffer& buffer)
	{
		set_buffer(buffer.id());
	}

	void AudioSource::set_buffer(ALuint buffer_id)
	{
		alSourcei(m_id, AL_BUFFER, static_cast<ALint>(buffer_id));
	}

	float AudioSource::volume() const
	{
		float volume = 0.f;
		alGetSourcef(m_id, AL_GAIN, &volume);

		return volume;
	}

	bool AudioSource::looping() const
	{
		int looping = 0;
		alGetSourcei(m_id, AL_LOOPING, &looping);

		return looping;
	}

	float AudioSource::pitch() const
	{
		float pitch = 0.f;
		alGetSourcef(m_id, AL_PITCH, &pitch);

		return pitch;
	}

	AudioSourceState AudioSource::state() const
	{
		int state = 0;
		alGetSourcei(m_id, AL_SOURCE_STATE, &state);

		return static_cast<AudioSourceState>(state);
	}

	bool AudioSource::buffers_processed() const
	{
		int buffers_processed = 0;
		alGetSourcei(m_id, AL_BUFFERS_PROCESSED, &buffers_processed);
	
		return buffers_processed;
	}

	ALuint AudioSource::id() const
	{
		return m_id;
	}

	void AudioSource::queue_buffer(const AudioBuffer& buffer)
	{
		queue_buffer(buffer.id());
	}

	void AudioSource::queue_buffer(ALuint buffer_id)
	{
		alSourceQueueBuffers(m_id, 1, &buffer_id);
	}

	ALuint AudioSource::unqueue_buffer()
	{
		ALuint buffer = 0;
		alSourceUnqueueBuffers(m_id, 1, &buffer);

		return buffer;
	}

}