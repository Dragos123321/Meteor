#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/mesh.h"

#include <vector>

namespace mtr 
{
	class Model : NonCopyable
	{
	public:
		Model() = default;
		explicit Model(const std::vector<MeshBuilder>& mesh_builders);
		Model(Model&& other) noexcept;

		Model& operator=(Model&& other) noexcept;

		const std::vector<Mesh>& meshes() const;

	private:
		std::vector<Mesh> m_meshes;
	};
}