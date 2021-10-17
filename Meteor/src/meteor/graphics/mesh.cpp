#include "meteor/graphics/mesh.h"

namespace mtr
{
	/*
		Mesh
	*/
	Mesh::Mesh(Mesh&& other) noexcept :
		m_vao{ std::move(other.m_vao) },
		m_vbo{ std::move(other.m_vbo) },
		m_ebo{ std::move(other.m_ebo) },
		m_diffuse_textures{ std::move(other.m_diffuse_textures) },
		m_specular_textures{ std::move(other.m_specular_textures) },
		m_normal_map_textures{ std::move(other.m_normal_map_textures) },
		m_height_map_textures{ std::move(other.m_height_map_textures) }
	{}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		m_vao = std::move(other.m_vao);
		m_vbo = std::move(other.m_vbo);
		m_ebo = std::move(other.m_ebo);
		m_diffuse_textures = std::move(other.m_diffuse_textures);
		m_specular_textures = std::move(other.m_specular_textures);
		m_normal_map_textures = std::move(other.m_normal_map_textures);
		m_height_map_textures = std::move(other.m_height_map_textures);

		return *this;
	}

	const gl::VertexArray& Mesh::vertex_array() const
	{
		return m_vao;
	}

	const gl::IndexBuffer& Mesh::index_buffer() const
	{
		return m_ebo;
	}

	const std::vector<gl::Texture2D>& Mesh::diffuse_textures() const
	{
		return m_diffuse_textures;
	}

	const std::vector<gl::Texture2D>& Mesh::specular_textures() const
	{
		return m_specular_textures;
	}

	const std::vector<gl::Texture2D>& Mesh::normal_map_textures() const
	{
		return m_normal_map_textures;
	}

	const std::vector<gl::Texture2D>& Mesh::height_map_textures() const
	{
		return m_height_map_textures;
	}

	Mesh::Mesh(const MeshBuilder& mesh_builder)
	{
		gl::VertexLayout layout(sizeof(MeshVertex));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(MeshVertex, position));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(MeshVertex, normals));
		layout.add_attribute(gl::VertexAttributeType::FloatVec2, offsetof(MeshVertex, tex_coords));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(MeshVertex, tangent));
		layout.add_attribute(gl::VertexAttributeType::FloatVec3, offsetof(MeshVertex, bitangent));

		m_vao = gl::VertexArray(layout, m_vbo);

		gl::VertexBufferBinder<MeshVertex> vbo_binder(m_vbo);
		vbo_binder.upload_data(mesh_builder.vertices.data(), mesh_builder.vertices.size());

		gl::IndexBufferBinder ebo_binder(m_ebo);
		ebo_binder.upload_data(mesh_builder.indexes.data(), mesh_builder.indexes.size());

		gl::TextureSettings texture_settings;
		texture_settings.format = gl::TextureFormat::SRgb;
		texture_settings.wrap_s = gl::TextureWrap::Repeat;
		texture_settings.wrap_t = gl::TextureWrap::Repeat;
		texture_settings.min_filter = gl::TextureMinFilter::LinearMipmapLinear;
		texture_settings.mag_filter = gl::TextureMagFilter::Linear;

		m_diffuse_textures.reserve(mesh_builder.diffuse_images.size());
		m_specular_textures.reserve(mesh_builder.specular_images.size());
		m_normal_map_textures.reserve(mesh_builder.normal_map_images.size());
		m_height_map_textures.reserve(mesh_builder.height_map_images.size());

		for (const auto& image : mesh_builder.diffuse_images)
			m_diffuse_textures.emplace_back(image, texture_settings);

		for (const auto& image : mesh_builder.specular_images)
			m_specular_textures.emplace_back(image, texture_settings);

		for (const auto& image : mesh_builder.normal_map_images)
			m_normal_map_textures.emplace_back(image, texture_settings);

		for (const auto& image : mesh_builder.height_map_images)
			m_height_map_textures.emplace_back(image, texture_settings);
	}

	/*
		MeshBuilder
	*/
	Mesh MeshBuilder::build() const
	{
		return Mesh(*this);
	}
}