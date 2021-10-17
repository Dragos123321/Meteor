#pragma once

#include <glad/glad.h>

namespace mtr::gl
{
	enum class Capability
	{
		Blend = GL_BLEND,
		DepthTest = GL_DEPTH_TEST,
		ScissorTest = GL_SCISSOR_TEST,
		StencilTest = GL_STENCIL_TEST
	};

	enum class DepthFunction
	{
		Less = GL_LESS,
		LessOrEqual = GL_LEQUAL,
		Equal = GL_EQUAL,
		GreaterOrEqual = GL_GEQUAL,
		Greater = GL_GREATER
	};

	template<Capability C>
	struct CapabilityData
	{
		CapabilityData(bool enabled = false);

		CapabilityData& operator=(bool enabled);

		void apply() const;

		operator bool() const;
		Capability capability() const;

		bool enabled = false;
	};

	struct RenderSettings
	{
		void apply() const;

		CapabilityData<Capability::Blend> blend;
		CapabilityData<Capability::DepthTest> depth_test;
		CapabilityData<Capability::ScissorTest> scissor_test;
		CapabilityData<Capability::StencilTest> stencil_test;
		DepthFunction depth_function = DepthFunction::LessOrEqual;
	};

#pragma region Implementation
	template<Capability C>
	CapabilityData<C>::CapabilityData(bool enabled) :
		enabled{ enabled }
	{}

	template<Capability C>
	CapabilityData<C>& CapabilityData<C>::operator=(bool enabled)
	{
		this->enabled = enabled;
		return *this;
	}

	template<Capability C>
	void CapabilityData<C>::apply() const
	{
		if (*this)
		{
			glEnable(static_cast<GLenum>(capability()));
		}
		else
		{
			glDisable(static_cast<GLenum>(capability()));
		}
	}

	template<Capability C>
	CapabilityData<C>::operator bool() const
	{
		return enabled;
	}

	template<Capability C>
	Capability CapabilityData<C>::capability() const
	{
		return C;
	}
#pragma endregion
}
