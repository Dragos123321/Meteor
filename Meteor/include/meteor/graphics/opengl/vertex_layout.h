#pragma once

#include <glad/glad.h>

#include <vector>

namespace mtr::gl
{
	enum class VertexAttributeType
	{
		Byte,
		ByteVec2,
		ByteVec3,
		ByteVec4,
		UByte,
		UByteVec2,
		UByteVec3,
		UByteVec4,
		Short,
		ShortVec2,
		ShortVec3,
		ShortVec4,
		UShort,
		UShortVec2,
		UShortVec3,
		UShortVec4,
		Int,
		IntVec2,
		IntVec3,
		IntVec4,
		UInt,
		UIntVec2,
		UIntVec3,
		UIntVec4,
		Float,
		FloatVec2,
		FloatVec3,
		FloatVec4
	};

	constexpr GLenum component_type(VertexAttributeType attribute_type);
	constexpr int component_count(VertexAttributeType attribute_type);
	constexpr bool is_float_type(VertexAttributeType attribute_type);
	constexpr bool is_int_type(VertexAttributeType attribute_type);

	class VertexAttribute
	{
	public:
		constexpr VertexAttribute(VertexAttributeType type, size_t offset, bool normalized = false, size_t divizor = 0);

		constexpr VertexAttributeType type() const;
		constexpr size_t offset() const;
		constexpr size_t divizor() const;
		constexpr bool normalized() const;

	private:
		VertexAttributeType m_type;
		size_t m_offset;
		size_t m_divizor;
		bool m_normalized;
	};

	class VertexLayout
	{
	public:
		explicit constexpr VertexLayout(size_t stride);

		void add_attribute(VertexAttributeType type, size_t offset, bool normalized = false, size_t divizor = 0);

		size_t stride() const;
		const std::vector<VertexAttribute>& attributes() const;

	private:
		size_t m_stride = 0;
		std::vector<VertexAttribute> m_attributes;
	};

#pragma region Implementation
	/*
		VertexAttributeType
	*/
	constexpr GLenum component_type(VertexAttributeType attribute_type)
	{
		switch (attribute_type)
		{
		case VertexAttributeType::Byte:
		case VertexAttributeType::ByteVec2:
		case VertexAttributeType::ByteVec3:
		case VertexAttributeType::ByteVec4:
			return GL_BYTE;

		case VertexAttributeType::UByte:
		case VertexAttributeType::UByteVec2:
		case VertexAttributeType::UByteVec3:
		case VertexAttributeType::UByteVec4:
			return GL_UNSIGNED_BYTE;

		case VertexAttributeType::Short:
		case VertexAttributeType::ShortVec2:
		case VertexAttributeType::ShortVec3:
		case VertexAttributeType::ShortVec4:
			return GL_SHORT;

		case VertexAttributeType::UShort:
		case VertexAttributeType::UShortVec2:
		case VertexAttributeType::UShortVec3:
		case VertexAttributeType::UShortVec4:
			return GL_UNSIGNED_SHORT;

		case VertexAttributeType::Int:
		case VertexAttributeType::IntVec2:
		case VertexAttributeType::IntVec3:
		case VertexAttributeType::IntVec4:
			return GL_INT;

		case VertexAttributeType::UInt:
		case VertexAttributeType::UIntVec2:
		case VertexAttributeType::UIntVec3:
		case VertexAttributeType::UIntVec4:
			return GL_UNSIGNED_INT;

		case VertexAttributeType::Float:
		case VertexAttributeType::FloatVec2:
		case VertexAttributeType::FloatVec3:
		case VertexAttributeType::FloatVec4:
			return GL_FLOAT;

		default:
			return -1;
		}
	}

	constexpr int component_count(VertexAttributeType attribute_type)
	{
		switch (attribute_type)
		{
		case VertexAttributeType::Byte:
		case VertexAttributeType::UByte:
		case VertexAttributeType::Short:
		case VertexAttributeType::UShort:
		case VertexAttributeType::Int:
		case VertexAttributeType::UInt:
		case VertexAttributeType::Float:
			return 1;

		case VertexAttributeType::ByteVec2:
		case VertexAttributeType::UByteVec2:
		case VertexAttributeType::ShortVec2:
		case VertexAttributeType::UShortVec2:
		case VertexAttributeType::IntVec2:
		case VertexAttributeType::UIntVec2:
		case VertexAttributeType::FloatVec2:
			return 2;

		case VertexAttributeType::ByteVec3:
		case VertexAttributeType::UByteVec3:
		case VertexAttributeType::ShortVec3:
		case VertexAttributeType::UShortVec3:
		case VertexAttributeType::IntVec3:
		case VertexAttributeType::UIntVec3:
		case VertexAttributeType::FloatVec3:
			return 3;

		case VertexAttributeType::ByteVec4:
		case VertexAttributeType::UByteVec4:
		case VertexAttributeType::ShortVec4:
		case VertexAttributeType::UShortVec4:
		case VertexAttributeType::IntVec4:
		case VertexAttributeType::UIntVec4:
		case VertexAttributeType::FloatVec4:
			return 4;

		default:
			return -1;
		}
	}

	constexpr bool is_float_type(VertexAttributeType attribute_type)
	{
		switch (attribute_type)
		{
		case VertexAttributeType::Float:
		case VertexAttributeType::FloatVec2:
		case VertexAttributeType::FloatVec3:
		case VertexAttributeType::FloatVec4:
			return true;

		default:
			return false;
		}
	}

	constexpr bool is_int_type(VertexAttributeType attribute_type)
	{
		return !is_float_type(attribute_type);
	}

	/*
		VertexAttribute
	*/
	constexpr VertexAttribute::VertexAttribute(VertexAttributeType type, size_t offset, bool normalized, size_t divizor) :
		m_type{ type },
		m_offset{ offset },
		m_divizor{ divizor },
		m_normalized{ normalized }
	{}

	constexpr VertexAttributeType VertexAttribute::type() const
	{
		return m_type;
	}

	constexpr size_t VertexAttribute::offset() const
	{
		return m_offset;
	}

	constexpr size_t VertexAttribute::divizor() const
	{
		return m_divizor;
	}

	constexpr bool VertexAttribute::normalized() const
	{
		return m_normalized;
	}

	/*
		VertexLayout
	*/
	constexpr VertexLayout::VertexLayout(size_t stride) :
		m_stride{ stride }
	{}
#pragma endregion
}
