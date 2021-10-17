#include "meteor/core/thread_pool.h"
#include "meteor/graphics/image.h"
#include "meteor/graphics/model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace mtr
{
	using FutureImageVector = std::vector<std::future<Image>>;

	/*
		ModelImageData
	*/
	struct ModelImageData
	{
		FutureImageVector diffuse_images;
		FutureImageVector specular_images;
		FutureImageVector normal_map_images;
		FutureImageVector height_map_images;
	};

	/*
		ModelLoaderData
	*/
	struct ModelLoaderData
	{
		explicit ModelLoaderData(const aiScene* scene, std::string&& directory, ThreadPool& thread_pool);

		const aiScene* scene = nullptr;
		std::string directory;
		ThreadPool* thread_pool = nullptr;
		std::vector<std::future<std::vector<MeshVertex>>> vertices;
		std::vector<std::future<std::vector<GLuint>>> indexes;
		std::vector<ModelImageData> images;
	};

	ModelLoaderData::ModelLoaderData(const aiScene* scene, std::string&& directory, ThreadPool& thread_pool) :
		scene{ scene },
		directory{ std::move(directory) },
		thread_pool{ &thread_pool }
	{}

	/*
		Model loader
	*/
	std::vector<MeshVertex> load_vertices(const aiMesh* mesh)
	{
		std::vector<MeshVertex> vertices;
		vertices.reserve(mesh->mNumVertices);

		MeshVertex vertex;

		auto to_glm_vec2 = [](const aiVector3D& vec) {
			return glm::vec2(vec.x, vec.y);
		};

		auto to_glm_vec3 = [](const aiVector3D& vec) {
			return glm::vec3(vec.x, vec.y, vec.z);
		};

		for (unsigned int i = 0; i != mesh->mNumVertices; ++i)
		{
			vertex.position = to_glm_vec3(mesh->mVertices[i]);
			vertex.normals = to_glm_vec3(mesh->mNormals[i]);
			vertex.tangent = to_glm_vec3(mesh->mTangents[i]);
			vertex.bitangent = to_glm_vec3(mesh->mBitangents[i]);

			if (mesh->mTextureCoords[0])
				vertex.tex_coords = to_glm_vec2(mesh->mTextureCoords[0][i]);
			else
				vertex.tex_coords = glm::vec2(0.0f);

			vertices.push_back(vertex);
		}

		return vertices;
	}

	std::vector<GLuint> load_indexes(const aiMesh* mesh)
	{
		std::vector<GLuint> indexes;

		for (unsigned int i = 0; i != mesh->mNumFaces; ++i)
		{
			const auto& face = mesh->mFaces[i];
			indexes.insert(indexes.end(), face.mIndices, face.mIndices + face.mNumIndices);
		}

		return indexes;
	}

	FutureImageVector load_images(const aiMaterial* material, aiTextureType texture_type, 
		gl::TextureType gl_texture_type, std::string_view directory, ThreadPool& thread_pool)
	{
		unsigned int texture_count = material->GetTextureCount(texture_type);

		FutureImageVector images;
		images.reserve(texture_count);

		fs::path texture_path;
		aiString texture_file;

		for (unsigned int i = 0; i != texture_count; ++i)
		{
			material->GetTexture(texture_type, i, &texture_file);

			texture_path.clear();
			texture_path.append(directory);
			texture_path.append(texture_file.C_Str());

			images.push_back(thread_pool.execute([texture_path]() {
				return Image(texture_path.string());
			}));
		}

		return images;
	}

	void process_mesh(const aiMesh* mesh, ModelLoaderData& loader_data)
	{
		loader_data.vertices.push_back(loader_data.thread_pool->execute([mesh]() {
			return load_vertices(mesh);
		}));

		loader_data.indexes.push_back(loader_data.thread_pool->execute([mesh]() {
			return load_indexes(mesh);
		}));

		auto& images = loader_data.images.emplace_back();

		if (mesh->mMaterialIndex >= 0)
		{
			const aiMaterial* material = loader_data.scene->mMaterials[mesh->mMaterialIndex];

			images.diffuse_images = load_images(material, aiTextureType_DIFFUSE,
				gl::TextureType::Diffuse, loader_data.directory, *loader_data.thread_pool);

			images.specular_images = load_images(material, aiTextureType_SPECULAR,
				gl::TextureType::Specular, loader_data.directory, *loader_data.thread_pool);

			images.normal_map_images = load_images(material, aiTextureType_HEIGHT,
				gl::TextureType::NormalMap, loader_data.directory, *loader_data.thread_pool);

			images.height_map_images = load_images(material, aiTextureType_AMBIENT,
				gl::TextureType::HeightMap, loader_data.directory, *loader_data.thread_pool);
		}
	}

	void process_node(const aiNode* node, ModelLoaderData& loader_data)
	{
		for (unsigned int i = 0; i != node->mNumMeshes; ++i)
		{
			const aiMesh* mesh = loader_data.scene->mMeshes[node->mMeshes[i]];
			process_mesh(mesh, loader_data);
		}

		for (unsigned int i = 0; i != node->mNumChildren; ++i)
		{
			process_node(node->mChildren[i], loader_data);
		}
	}

	ModelBuilder load_model(std::string_view path, ThreadPool& thread_pool)
	{
		Assimp::Importer importer;
		unsigned int importer_flags =
			aiProcess_Triangulate |
			aiProcess_GenNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_OptimizeMeshes;

		const aiScene* scene = importer.ReadFile(path.data(), importer_flags);

		if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			throw AssimpError(importer.GetErrorString());
		}

		ModelLoaderData loader_data(scene, fs::path(path).parent_path().string(), thread_pool);
		process_node(scene->mRootNode, loader_data);

		std::vector<MeshBuilder> mesh_builders;
		mesh_builders.reserve(scene->mNumMeshes);

		for (size_t i = 0; i != loader_data.vertices.size(); ++i)
		{
			auto& mesh_builder = mesh_builders.emplace_back();
			mesh_builder.vertices = loader_data.vertices[i].get();
			mesh_builder.indexes = loader_data.indexes[i].get();
			
			auto& images = loader_data.images[i];

			mesh_builder.diffuse_images.reserve(images.diffuse_images.size());
			mesh_builder.specular_images.reserve(images.specular_images.size());
			mesh_builder.normal_map_images.reserve(images.normal_map_images.size());
			mesh_builder.height_map_images.reserve(images.height_map_images.size());

			for (auto& image : images.diffuse_images)
				mesh_builder.diffuse_images.push_back(image.get());

			for (auto& image : images.specular_images)
				mesh_builder.specular_images.push_back(image.get());

			for (auto& image : images.normal_map_images)
				mesh_builder.normal_map_images.push_back(image.get());

			for (auto& image : images.height_map_images)
				mesh_builder.height_map_images.push_back(image.get());
		}

		return ModelBuilder(std::move(mesh_builders));
	}

	/*
		ModelBuilder
	*/
	ModelBuilder::ModelBuilder(std::vector<MeshBuilder>&& mesh_builders) :
		m_mesh_builders{ std::move(mesh_builders) }
	{}

	Model ModelBuilder::build() const
	{
		return Model(m_mesh_builders);
	}

	/*
		AssimpError
	*/
	AssimpError::AssimpError(const char* message) :
		std::runtime_error{ message }
	{}
}