#include "meteor/graphics/model.h"

namespace mtr 
{
	/*
		Model
	*/
	Model::Model(const std::vector<MeshBuilder>& mesh_builders)
	{
		m_meshes.reserve(mesh_builders.size());

		for (const auto& mesh_builder : mesh_builders)
		{
			m_meshes.push_back(mesh_builder.build());
		}
	}

	Model::Model(Model&& other) noexcept :
		m_meshes{ std::move(other.m_meshes) }
	{}

	Model& Model::operator=(Model&& other) noexcept
	{
		m_meshes = std::move(other.m_meshes);

		return *this;
	}

	const std::vector<Mesh>& Model::meshes() const
	{
		return m_meshes;
	}
}