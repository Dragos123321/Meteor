#pragma once

#include "meteor/core/non_copyable.h"

#include <al/al.h>

namespace mtr
{
	class AudioBuffer;

	enum class AudioSourceState
	{
		Initial = AL_INITIAL,
		Playing = AL_PLAYING,
		Paused = AL_PAUSED,
		Stopped = AL_STOPPED
	};

	class AudioSource : NonCopyable
	{
	public:
		AudioSource();
		AudioSource(AudioSource&& other) noexcept;
		~AudioSource();

		AudioSource& operator=(AudioSource&& other) noexcept;

		void play() const;
		void pause() const;
		void resume() const;
		void stop() const;

		void set_volume(float volume);
		void set_looping(bool looping);
		void set_pitch(float pitch);

		void set_buffer(const AudioBuffer& buffer);
		void set_buffer(ALuint buffer_id);

		float volume() const;
		bool looping() const;
		float pitch() const;
		AudioSourceState state() const;
		bool buffers_processed() const;
		ALuint id() const;

		void queue_buffer(const AudioBuffer& buffer);
		void queue_buffer(ALuint buffer_id);
		ALuint unqueue_buffer();

	private:
		ALuint m_id = 0;
	};
}