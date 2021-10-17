#include "meteor/audio/audio_decoder.h"
#include "meteor/audio/audio_engine.h"
#include "meteor/audio/audio_stream.h"
#include "meteor/core/errors.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace mtr
{
	/*
		AudioStream
	*/
	AudioStream::AudioStream() :
		m_stream{ std::make_shared<AudioStreamData>() }
	{
		AUDIO->add_stream(m_stream);
	}

	AudioStream::AudioStream(std::string_view path) :
		m_stream{ std::make_shared<AudioStreamData>(path) }
	{
		AUDIO->add_stream(m_stream);
	}

	void AudioStream::open(std::string_view path)
	{
		m_stream->open(path);
	}

	void AudioStream::close()
	{
		m_stream->close();
	}

	void AudioStream::play()
	{
		m_stream->play();
	}

	void AudioStream::pause()
	{
		m_stream->pause();
	}

	void AudioStream::resume()
	{
		m_stream->resume();
	}

	void AudioStream::stop()
	{
		m_stream->stop();
	}

	void AudioStream::set_volume(float volume)
	{
		m_stream->set_volume(volume);
	}

	void AudioStream::set_looping(bool looping)
	{
		m_stream->set_looping(looping);
	}

	void AudioStream::set_pitch(float pitch)
	{
		m_stream->set_pitch(pitch);
	}

	int AudioStream::channel_count() const
	{
		return m_stream->channel_count();
	}

	int AudioStream::sample_rate() const
	{
		return m_stream->sample_rate();
	}

	float AudioStream::volume() const
	{
		return m_stream->volume();
	}

	bool AudioStream::looping() const
	{
		return m_stream->looping();
	}

	float AudioStream::pitch() const
	{
		return m_stream->pitch();
	}

	/*
		AudioStreamData
	*/
	AudioStreamData::AudioStreamData(std::string_view path)
	{
		open_stream(path);
	}

	AudioStreamData::AudioStreamData(AudioStreamData&& other) noexcept :
		m_stream{ other.m_stream },
		m_info{ other.m_info },
		m_output{ other.m_output },
		m_source{ std::move(other.m_source) },
		m_buffers{ std::move(other.m_buffers[0]), std::move(other.m_buffers[1]) },
		m_looping{ other.m_looping }
	{
		other.m_stream = nullptr;
		other.m_info = { 0 };
	}

	AudioStreamData::~AudioStreamData()
	{
		stop();
		stb_vorbis_close(m_stream);
	}

	AudioStreamData& AudioStreamData::operator=(AudioStreamData&& other) noexcept
	{
		stb_vorbis_close(m_stream);

		m_stream = other.m_stream;
		m_info = other.m_info;
		m_output = std::move(other.m_output);
		m_source = std::move(other.m_source);
		m_buffers[0] = std::move(other.m_buffers[0]);
		m_buffers[1] = std::move(other.m_buffers[1]);
		m_looping = other.m_looping;

		other.m_stream = nullptr;
		other.m_info = { 0 };
		other.m_looping = true;

		return *this;
	}

	void AudioStreamData::open(std::string_view path)
	{
		stop();
		open_stream(path);
	}

	void AudioStreamData::close()
	{
		stop();

		stb_vorbis_close(m_stream);
		m_stream = nullptr;
		m_info = { 0 };
	}

	void AudioStreamData::play()
	{
		if (m_stream)
		{
			stop();

			m_buffers[0].upload_data(decode_next_samples());
			m_buffers[1].upload_data(decode_next_samples());

			m_source.queue_buffer(m_buffers[0]);
			m_source.queue_buffer(m_buffers[1]);
			m_source.play();
		}
	}

	void AudioStreamData::pause()
	{
		m_source.pause();
	}

	void AudioStreamData::resume()
	{
		m_source.resume();
	}

	void AudioStreamData::stop()
	{
		m_source.stop();
		m_source.set_buffer(0);		

		if (m_stream)
			stb_vorbis_seek_start(m_stream);
	}

	bool AudioStreamData::needs_update() const
	{
		return m_stream;
	}

	void AudioStreamData::update()
	{
		auto get_processed_buffer = [this]() -> AudioBuffer& {
			if (m_source.unqueue_buffer() == m_buffers[0].id())
			{
				return m_buffers[0];
			}

			return m_buffers[1];
		};

		if (m_source.buffers_processed())
		{
			auto& processed_buffer = get_processed_buffer();
			auto audio_data = decode_next_samples();

			if (audio_data.is_empty())
			{
				if (m_looping) play();
			}
			else
			{
				processed_buffer.upload_data(audio_data);
				m_source.queue_buffer(processed_buffer);
			}	
		}
	}

	void AudioStreamData::set_volume(float volume)
	{
		m_source.set_volume(volume);
	}

	void AudioStreamData::set_looping(bool looping)
	{
		m_looping = looping;
	}

	void AudioStreamData::set_pitch(float pitch)
	{
		m_source.set_pitch(pitch);
	}

	int AudioStreamData::channel_count() const
	{
		return m_info.channels;
	}

	int AudioStreamData::sample_rate() const
	{
		return m_info.sample_rate;
	}

	float AudioStreamData::volume() const
	{
		return m_source.volume();
	}

	bool AudioStreamData::looping() const
	{
		return m_looping;
	}

	float AudioStreamData::pitch() const
	{
		return m_source.pitch();
	}

	void AudioStreamData::open_stream(std::string_view path)
	{
		stb_vorbis_close(m_stream);
		m_stream = stb_vorbis_open_filename(path.data(), nullptr, nullptr);

		if (!m_stream)
		{
			m_info = { 0 };
			m_output.clear();

			if (!fs::exists(path))
				throw FileNotFoundError(path);
			else
				throw InvalidFormatError(path);
		}

		m_info = stb_vorbis_get_info(m_stream);
		m_output.resize(sample_rate() / 2);
	}

	AudioData AudioStreamData::decode_next_samples()
	{
		return AudioDecoder::decode_next_samples(m_stream, m_info, m_output.data(), m_output.size());
	}
}