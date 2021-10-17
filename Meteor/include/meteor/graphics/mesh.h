#pragma once

#include "meteor/core/non_copyable.h"
#include "meteor/graphics/image.h"
#include "meteor/graphics/opengl/index_buffer.h"
#include "meteor/graphics/opengl/program.h"
#include "meteor/graphics/opengl/texture_2d.h"
#include "meteor/graphics/opengl/vertex_array.h"
#include "meteor/graphics/opengl/vertex_buffer.h"

#include <vector>

namespace mtr
{
	struct MeshBuilder;

	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec3 normals;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};
	
	class Mesh : NonCopyable
	{
		friend struct MeshBuilder;

	public:
		Mesh() = default;
		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(Mesh&& other) noexcept;

		const gl::VertexArray& vertex_array() const;
		const gl::IndexBuffer& index_buffer() const;
		const std::vector<gl::Texture2D>& diffuse_textures() const;
		const std::vector<gl::Texture2D>& specular_textures() const;
		const std::vector<gl::Texture2D>& normal_map_textures() const;
		const std::vector<gl::Texture2D>& height_map_textures() const;

	private:
		explicit Mesh(const MeshBuilder& mesh_builder);

		gl::VertexArray m_vao;
		gl::VertexBuffer<MeshVertex> m_vbo;
		gl::IndexBuffer m_ebo;

		std::vector<gl::Texture2D> m_diffuse_textures;
		std::vector<gl::Texture2D> m_specular_textures;
		std::vector<gl::Texture2D> m_normal_map_textures;
		std::vector<gl::Texture2D> m_height_map_textures;
	};

	struct MeshBuilder
	{
		std::vector<MeshVertex> vertices;
		std::vector<GLuint> indexes;
		std::vector<Image> diffuse_images;
		std::vector<Image> specular_images;
		std::vector<Image> normal_map_images;
		std::vector<Image> height_map_images;

		Mesh build() const;
	};
}