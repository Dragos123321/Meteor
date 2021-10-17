#pragma once

#include "meteor/audio/audio_buffer.h"
#include "meteor/audio/audio_source.h"
#include "meteor/core/non_copyable.h"

#include <stb/stb_vorbis.h>

#include <memory>
#include <string_view>
#include <vector>

namespace mtr
{
	class AudioStreamData;

	class AudioStream : NonCopyable
	{
	public:
		AudioStream();
		explicit AudioStream(std::string_view path);

		void open(std::string_view path);
		void close();

		void play();
		void pause();
		void resume();
		void stop();

		void set_volume(float volume);
		void set_looping(bool looping);
		void set_pitch(float pitch);

		int channel_count() const;
		int sample_rate() const;
		float volume() const;
		bool looping() const;
		float pitch() const;

	private:
		std::shared_ptr<AudioStreamData> m_stream;
	};

	class AudioStreamData : NonCopyable
	{
		friend class AudioEngineX;

	public:
		AudioStreamData() = default;
		explicit AudioStreamData(std::string_view path);
		AudioStreamData(AudioStreamData&& other) noexcept;
		~AudioStreamData();

		AudioStreamData& operator=(AudioStreamData&& other) noexcept;

		void open(std::string_view path);
		void close();

		void play();
		void pause();
		void resume();
		void stop();
		
		void set_volume(float volume);
		void set_looping(bool looping);
		void set_pitch(float pitch);

		int channel_count() const;
		int sample_rate() const;
		float volume() const;
		bool looping() const;
		float pitch() const;

		bool needs_update() const;
		void update();

	private:
		void open_stream(std::string_view path);
		AudioData decode_next_samples();

		stb_vorbis* m_stream = nullptr;
		stb_vorbis_info m_info = { 0 };
		std::vector<int16_t> m_output;
		AudioSource m_source;
		AudioBuffer m_buffers[2];
		bool m_looping = true;
	};
}
