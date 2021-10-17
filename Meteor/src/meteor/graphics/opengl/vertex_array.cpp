#include "meteor/graphics/opengl/context.h"
#include "meteor/graphics/opengl/vertex_array.h"

namespace mtr::gl
{
	/*
		VertexArray
	*/
	VertexArray::VertexArray(VertexArray&& other) noexcept :
		m_id{ other.m_id }
	{
		other.m_id = 0;
	}

	VertexArray::~VertexArray()
	{
		dispose();
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		dispose();

		m_id = other.m_id;
		other.m_id = 0;

		return *this;
	}

	GLuint VertexArray::id() const
	{
		return m_id;
	}

	bool VertexArray::valid() const
	{
		return m_id;
	}

	void VertexArray::dispose()
	{
		if (m_id) glDeleteVertexArrays(1, &m_id);
	}

	/*
		VertexArrayBinder
	*/
	VertexArrayBinder::VertexArrayBinder(const VertexArray& array) :
		m_array{ &array }
	{
		CONTEXT->bind_vertex_array(array.id());
	}

	VertexArrayBinder::~VertexArrayBinder()
	{
		CONTEXT->unbind_vertex_array();
	}
}