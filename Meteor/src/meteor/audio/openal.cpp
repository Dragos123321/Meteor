#include "meteor/audio/openal.h"

namespace mtr
{
	OpenALContext::OpenALContext()
	{
		m_device = alcOpenDevice(nullptr);
		
		if (!m_device)
			throw OpenALError(OpenALErrorCode::NoError, "failed to open device");

		m_context = alcCreateContext(m_device, nullptr);
		alcMakeContextCurrent(m_context);
	}

	OpenALContext::~OpenALContext()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	OpenALError::OpenALError(OpenALErrorCode code, const char* description) :
		std::runtime_error{ description },
		m_code{ code }
	{}

	OpenALErrorCode OpenALError::code() const
	{
		return m_code;
	}
}