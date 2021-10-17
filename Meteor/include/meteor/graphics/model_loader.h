#pragma once

#include "meteor/graphics/model.h"

#include <string_view>

namespace mtr
{
	class ModelBuilder;
	class ThreadPool;

	extern ModelBuilder load_model(std::string_view path, ThreadPool& thread_pool);

	class ModelBuilder
	{
	public:
		explicit ModelBuilder(std::vector<MeshBuilder>&& mesh_builders);

		Model build() const;

	private:
		std::vector<MeshBuilder> m_mesh_builders;
	};

	class AssimpError : public std::runtime_error
	{
	public:
		explicit AssimpError(const char* message);
	};
}
