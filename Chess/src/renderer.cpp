#include "renderer.h"

#include <meteor/graphics/model.h>
#include <meteor/graphics/opengl/drawing.h>
#include <meteor/graphics/opengl/render_settings.h>
#include <meteor/graphics/opengl/program.h>

namespace game
{
	void draw_model(const mtr::Model& model, mtr::gl::Program& program,
		const glm::mat4& projection, const glm::mat4& view, const glm::vec3 view_pos, bool use_view_pos, size_t instance_count)
	{
		mtr::gl::ProgramBinder program_binder(program);
		program_binder["projection"] = projection;
		program_binder["view"] = view;

		if (use_view_pos)
			program_binder["view_position"] = view_pos;

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
		mtr::gl::draw_elements_instanced(mesh.vertex_array(), mesh.index_buffer(), instance_count);
	}
}