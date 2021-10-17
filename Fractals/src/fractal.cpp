#include "fractal.h"

#include <meteor/core/thread_pool.h>
#include <meteor/core/utils.h>
#include <meteor/graphics/opengl/shader_sources.h>
#include <meteor/graphics/opengl/drawing.h>

Fractal::Fractal(const mtr::Config& config) :
	Application{ config },
	m_camera_2d{ static_cast<float>(window_width()), static_cast<float>(window_height()) }
{
	m_data.x = 0.f;
	m_data.y = 0.f;
	m_data.scale = 1.f;
	m_data.max_iterations = 30;

	setup_color_maps();
	load_shaders();
}

void Fractal::on_key_press(const mtr::KeyPressEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::W:
		m_vertical_axis.press_positive_axis();
		break;

	case mtr::KeyCode::S:
		m_vertical_axis.press_negative_axis();
		break;

	case mtr::KeyCode::A:
		m_horizontal_axis.press_negative_axis();
		break;

	case mtr::KeyCode::D:
		m_horizontal_axis.press_positive_axis();
		break;

	case mtr::KeyCode::Z:
		m_scale.press_negative_axis();
		break;

	case mtr::KeyCode::X:
		m_scale.press_positive_axis();
		break;

	case mtr::KeyCode::Q:
		m_iterations.press_negative_axis();
		break;

	case mtr::KeyCode::E:
		m_iterations.press_positive_axis();
		break;

	case mtr::KeyCode::P:
		m_color_map_index = (m_color_map_index + 1) % 4;

		mtr::gl::ProgramBinder program_binder(m_program);

		for (size_t i = 0; i < m_color_maps[m_color_map_index].size(); ++i)
		{
			program_binder["color_map"][i] = m_color_maps[m_color_map_index][i];
		}		

		break;
	}
}

void Fractal::on_key_release(const mtr::KeyReleaseEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::Escape:
		quit();
		break;

	case mtr::KeyCode::W:
		m_vertical_axis.release_positive_axis();
		break;

	case mtr::KeyCode::S:
		m_vertical_axis.release_negative_axis();
		break;

	case mtr::KeyCode::A:
		m_horizontal_axis.release_negative_axis();
		break;

	case mtr::KeyCode::D:
		m_horizontal_axis.release_positive_axis();
		break;

	case mtr::KeyCode::Z:
		m_scale.release_negative_axis();
		break;

	case mtr::KeyCode::X:
		m_scale.release_positive_axis();
		break;

	case mtr::KeyCode::Q:
		m_iterations.release_negative_axis();
		break;

	case mtr::KeyCode::E:
		m_iterations.release_positive_axis();
		break;
	}
}

void Fractal::on_frame_resize(const mtr::FrameResizeEvent& event)
{
	m_camera_2d.set_size(event.size);
	glViewport(0, 0, event.size.x, event.size.y);
}

void Fractal::update()
{
	const float scale_per = 0.02f * m_data.scale;

	switch (m_vertical_axis.value())
	{
	case mtr::InputAxisValue::Negative:
		m_data.y -= scale_per;
		break;

	case mtr::InputAxisValue::Positive:
		m_data.y += scale_per;
		break;
	}

	switch (m_horizontal_axis.value())
	{
	case mtr::InputAxisValue::Negative:
		m_data.x -= scale_per;
		break;

	case mtr::InputAxisValue::Positive:
		m_data.x += scale_per;
		break;
	}

	switch (m_scale.value())
	{
	case mtr::InputAxisValue::Negative:
		m_data.scale -= scale_per;
		break;

	case mtr::InputAxisValue::Positive:
		m_data.scale += scale_per;
		break;
	}

	switch (m_iterations.value())
	{
	case mtr::InputAxisValue::Negative:
		neg_iterations++;
		if (m_data.max_iterations > 0 && neg_iterations == 5)
		{
			m_data.max_iterations -= 1;
			neg_iterations = 0;
		}
		break;

	case mtr::InputAxisValue::Positive:
		pos_iterations++;
		if (pos_iterations == 5)
		{
			m_data.max_iterations += 1;
			pos_iterations = 0;
		}
		break;
	}
}

void Fractal::draw()
{
	mtr::gl::clear_color_and_depth();

	mtr::gl::ProgramBinder binder(m_program);
	binder["rect_width"] = static_cast<float>(window_width());
	binder["rect_height"] = static_cast<float>(window_height());
	binder["area_w"] = glm::vec2(-2.0f * m_data.scale + m_data.x, 1.0f * m_data.scale + m_data.x);
	binder["area_h"] = glm::vec2(-1.0f * m_data.scale + m_data.y, 1.0f * m_data.scale + m_data.y);
	binder["max_iterations"] = m_data.max_iterations;

	m_renderer.render(m_program);
	swap_buffers();
}

void Fractal::load_shaders()
{
	auto vertex_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("fractals_assets/shaders/vertex_shader.glsl");
	});

	auto fragment_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("fractals_assets/shaders/fragment_shader.glsl");
	});

	m_program = mtr::gl::Program(vertex_source.get(), fragment_source.get());

	mtr::gl::ProgramBinder binder(m_program);

	for (int i = 0; i < m_color_maps[0].size(); ++i)
	{
		binder["color_map"][i] = m_color_maps[0][i];
	}
}

void Fractal::setup_color_maps()
{
	auto m_color_map1 = std::array<glm::vec3, 17>
	{
		glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.26, 0.18, 0.06),
			glm::vec3(0.1, 0.03, 0.1),
			glm::vec3(0.04, 0.0, 0.18),
			glm::vec3(0.02, 0.02, 0.29),
			glm::vec3(0.0, 0.03, 0.39),
			glm::vec3(0.05, 0.17, 0.54),
			glm::vec3(0.09, 0.32, 0.69),
			glm::vec3(0.22, 0.49, 0.82),
			glm::vec3(0.52, 0.71, 0.9),
			glm::vec3(0.82, 0.92, 0.97),
			glm::vec3(0.94, 0.91, 0.75),
			glm::vec3(0.97, 0.79, 0.37),
			glm::vec3(1.0, 0.67, 0.0),
			glm::vec3(0.8, 0.5, 0.0),
			glm::vec3(0.6, 0.34, 0.0),
			glm::vec3(0.41, 0.2, 0.01)
	};

	auto m_color_map2 = std::array<glm::vec3, 17>
	{
		glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.53, 0.09, 0.13),
			glm::vec3(0.64, 0.03, 0.1),
			glm::vec3(0.45, 0.0, 0.04),
			glm::vec3(0.58, 0.02, 0.02),
			glm::vec3(0.48, 0.03, 0.0),
			glm::vec3(0.64, 0.17, 0.05),
			glm::vec3(0.89, 0.12, 0.09),
			glm::vec3(0.92, 0.29, 0.22),
			glm::vec3(0.9, 0.51, 0.32),
			glm::vec3(0.97, 0.42, 0.42),
			glm::vec3(0.75, 0.21, 0.34),
			glm::vec3(0.83, 0.29, 0.37),
			glm::vec3(1.0, 0.37, 0.0),
			glm::vec3(0.9, 0.3, 0.0),
			glm::vec3(0.8, 0.14, 0.4),
			glm::vec3(0.7, 0.2, 0.41)
	};

	auto m_color_map3 = std::array<glm::vec3, 17>
	{
		glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.09, 0.53, 0.13),
			glm::vec3(0.03, 0.64, 0.1),
			glm::vec3(0.0, 0.45, 0.04),
			glm::vec3(0.02, 0.58, 0.02),
			glm::vec3(0.03, 0.48, 0.0),
			glm::vec3(0.17, 0.64, 0.05),
			glm::vec3(0.12, 0.89, 0.09),
			glm::vec3(0.29, 0.92, 0.22),
			glm::vec3(0.51, 0.9, 0.32),
			glm::vec3(0.42, 0.97, 0.42),
			glm::vec3(0.21, 0.75, 0.34),
			glm::vec3(0.29, 0.83, 0.37),
			glm::vec3(0.37, 1.0, 0.0),
			glm::vec3(0.3, 0.9, 0.0),
			glm::vec3(0.14, 0.8, 0.4),
			glm::vec3(0.2, 0.7, 0.41)
	};

	auto m_color_map4 = std::array<glm::vec3, 17>
	{
		glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.13, 0.09, 0.53),
			glm::vec3(0.1, 0.03, 0.64),
			glm::vec3(0.04, 0.0, 0.45),
			glm::vec3(0.02, 0.02, 0.58),
			glm::vec3(0.0, 0.03, 0.48),
			glm::vec3(0.05, 0.17, 0.64),
			glm::vec3(0.09, 0.32, 0.89),
			glm::vec3(0.22, 0.49, 0.92),
			glm::vec3(0.32, 0.51, 0.9),
			glm::vec3(0.42, 0.52, 0.97),
			glm::vec3(0.34, 0.31, 0.75),
			glm::vec3(0.37, 0.29, 0.83),
			glm::vec3(0.0, 0.47, 1.0),
			glm::vec3(0.0, 0.3, 0.9),
			glm::vec3(0.4, 0.14, 0.8),
			glm::vec3(0.41, 0.2, 0.7)
	};

	m_color_maps = std::array<std::array<glm::vec3, 17>, 4>
	{
		m_color_map1,
		m_color_map2,
		m_color_map3,
		m_color_map4
	};
}
