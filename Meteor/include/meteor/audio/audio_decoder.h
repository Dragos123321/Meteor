#pragma once

#include "meteor/audio/audio_data.h"

#include <stb/stb_vorbis.h>

#include <string_view>

namespace mtr
{
	class AudioDecoder
	{
	public:
		static OwnedAudioData decode_from_file(std::string_view path);

		static AudioData decode_next_samples(stb_vorbis* stream, 
			const stb_vorbis_info& stream_info, int16_t* output, size_t max_output_size);
	};
}
