#include "renderer.h"

#include <meteor/graphics/model.h>
#include <meteor/graphics/opengl/drawing.h>
#include <meteor/graphics/opengl/render_settings.h>
#include <meteor/graphics/opengl/program.h>

namespace game
{
	void draw_model(const mtr::Model& model, mtr::gl::Program& program, 
		const glm::mat4& projection, const glm::mat4& view, const glm::vec3 view_pos, size_t instance_count)
	{
		mtr::gl::ProgramBinder program_binder(program);
		program_binder["projection"] = projection;
		program_binder["view"] = view;

		mtr::gl::RenderSettings render_settings;
		render_settings.depth_test = true;
		render_settings.depth_function = mtr::gl::DepthFunction::LessOrEqual;
		render_settings.apply();

		for (const auto& mesh : model.meshes())
		{
			draw_mesh(mesh, program_binder, instance_count);
		}
	}

	void draw_mesh(const mtr::Mesh& mesh, mtr::gl::ProgramBinder& program_binder, size_t instance_count)
	{
		GLuint diffuse_count = static_cast<GLuint>(mesh.diffuse_textures().size());
		GLuint specular_count = static_cast<GLuint>(mesh.specular_textures().size());
		GLuint normal_count = static_cast<GLuint>(mesh.normal_map_textures().size());
		GLuint height_count = static_cast<GLuint>(mesh.height_map_textures().size());

		program_binder["number_of_diffuse_textures"] = static_cast<GLint>(diffuse_count);

		/*program_binder.uniform("number_of_specular_textures").set(static_cast<int>(specular_count));
		program_binder.uniform("number_of_normal_maps").set(static_cast<int>(normal_count));*/
		//program_binder.uniform("number_of_height_maps").set(static_cast<int>(height_count));

		GLuint texture_unit = 0;

		for (GLuint i = 0; i != diffuse_count; ++i)
		{
			mtr::gl::ConstTexture2DBinder texture_binder(mesh.diffuse_textures()[i], texture_unit);
			program_binder["texture_diffuse"][i] = static_cast<GLint>(texture_unit);
			++texture_unit;
		}

		/*for (GLuint i = 0; i != specular_count; ++i)
		{
			mtr::gl::Texture2DBinder texture_binder(mesh.specular_textures()[i], texture_unit);
			program_binder.uniform("texture_specular", i).set(mtr::gl::Sampler2D(texture_unit));
			++texture_unit;
		}


		for (GLuint i = 0; i != normal_count; ++i)
		{
			mtr::gl::Texture2DBinder texture_binder(mesh.normal_map_textures()[i], texture_unit);
			program_binder.uniform("texture_normal", i).set(mtr::gl::Sampler2D(texture_unit));
			++texture_unit;
		}

		program_binder.uniform("shininess").set(32.0f);*/

		/*for (GLuint i = 0; i != height_count; ++i)
		{
			mtr::gl::Texture2DBinder texture_binder(mesh.height_map_textures()[i], texture_unit);
			program_binder.uniform("texture_height", i).set(mtr::gl::Sampler2D(texture_unit));
			++texture_unit;
		}*/

		mtr::gl::draw_elements_instanced(mesh.vertex_array(), mesh.index_buffer(), instance_count);
	}
}