#pragma once

#include <glm/mat4x4.hpp>

namespace mtr
{
	class Mesh;
	class Model;
}

namespace mtr::gl
{
	class Program;
	class ProgramBinder;
}

namespace game
{
	extern void draw_model(const mtr::Model& model, mtr::gl::Program& program,
		const glm::mat4& projection, const glm::mat4& view, const glm::vec3 view_pos, bool use_view_pos = false, size_t instance_count = 1);

	extern void draw_mesh(const mtr::Mesh& mesh, mtr::gl::ProgramBinder& program_binder, size_t instance_count = 1);
}

