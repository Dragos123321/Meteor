#include "meteor/graphics/opengl/vertex_layout.h"

namespace mtr::gl
{
	void gl::VertexLayout::add_attribute(VertexAttributeType type, size_t offset, bool normalized, size_t divizor)
	{
		m_attributes.push_back(VertexAttribute(type, offset, normalized, divizor));
	}

	size_t VertexLayout::stride() const
	{
		return m_stride;
	}

	const std::vector<VertexAttribute>& VertexLayout::attributes() const
	{
		return m_attributes;
	}
}