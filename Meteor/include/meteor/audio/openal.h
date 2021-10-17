#pragma once

#include "meteor/core/non_copyable.h"

#include <al/al.h>
#include <al/alc.h>

#include <stdexcept>

namespace mtr
{
	class OpenALContext : NonCopyable, NonMovable
	{
	public:
		OpenALContext();
		~OpenALContext();

	private:
		ALCdevice* m_device = nullptr;
		ALCcontext* m_context = nullptr;
	};

	enum class OpenALErrorCode
	{
		NoError = AL_NO_ERROR,
		InvalidName = AL_INVALID_NAME,
		InvalidEnum = AL_INVALID_ENUM,
		InvalidValue = AL_INVALID_VALUE,
		InvalidOperation = AL_INVALID_OPERATION,
		OutOfMemory = AL_OUT_OF_MEMORY
	};

	class OpenALError : public std::runtime_error
	{
	public:
		OpenALError(OpenALErrorCode code, const char* description);

		OpenALErrorCode code() const;

	private:
		OpenALErrorCode m_code;
	};
}