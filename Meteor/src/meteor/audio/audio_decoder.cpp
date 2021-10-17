#include "meteor/audio/audio_decoder.h"
#include "meteor/audio/audio_stream.h"
#include "meteor/core/errors.h"

#include <stb/stb_vorbis.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace mtr
{
	OwnedAudioData AudioDecoder::decode_from_file(std::string_view path)
	{
		int16_t* output = nullptr;
		int channel_count = 0;
		int sample_rate = 0;

		int sample_count = stb_vorbis_decode_filename(path.data(), &channel_count, &sample_rate, &output);

		if (sample_count == -1)
		{
			if (!fs::exists(path))
				throw FileNotFoundError(path);
			else
				throw InvalidFormatError(path);
		}

		return OwnedAudioData(output, channel_count, sample_rate, sample_count);
	}

	AudioData AudioDecoder::decode_next_samples(stb_vorbis* stream, 
		const stb_vorbis_info& stream_info, int16_t* output, size_t max_output_size)
	{
		int sample_count = stb_vorbis_get_samples_short_interleaved(stream, stream_info.channels,
			output, static_cast<int>(max_output_size));
		
		return AudioData(output, stream_info.channels, stream_info.sample_rate, sample_count);
	}
}