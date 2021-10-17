#include "game.h"
#include "renderer.h"

#include <meteor/core/benchmark.h>
#include <meteor/core/utils.h>
#include <meteor/graphics/model_loader.h>
#include <meteor/graphics/text.h>
#include <meteor/graphics/opengl/shader_sources.h>
#include <meteor/graphics/opengl/drawing.h>
#include <meteor/graphics/opengl/render_settings.h>
#include <meteor/gui/button.h>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

const float Game::CAMERA_SPEED = 0.008f;

Game::Game(const mtr::Config& config) :
	Application{ config },
	m_camera_2d{ static_cast<float>(window_width()), static_cast<float>(window_height()) },
	m_camera_3d{ glm::vec3(0.0f, 0.0f, 3.0f) },
	m_font{ "assets/fonts/arial.json" }
{
	m_models[0] = mtr::load_model("assets/objects/pieces/pawn.obj", m_thread_pool).build();   // pawn model
	m_models[1] = mtr::load_model("assets/objects/pieces/knight.obj", m_thread_pool).build(); // knight model
	m_models[2] = mtr::load_model("assets/objects/pieces/queen.obj", m_thread_pool).build();  // queen model
	m_models[3] = mtr::load_model("assets/objects/pieces/bishop.obj", m_thread_pool).build(); // bishop model
	m_models[4] = mtr::load_model("assets/objects/pieces/king.obj", m_thread_pool).build();   // king model
	m_models[5] = mtr::load_model("assets/objects/pieces/rook.obj", m_thread_pool).build();   // rook model

	load_framebuffer();
	load_shaders();
	load_cubemap();
	load_board();

	set_mouse_tracking_enabled(true);
	set_cursor_mode(mtr::CursorMode::Disabled);

	m_panel.set_padding(mtr::Padding::uniform(6.f));
	m_panel.set_row_spacing(6.f);
	m_panel.set_column_spacing(6.f);

	m_panel.add_row(mtr::GridStyle(mtr::SizeType::Absolute, 40.f));
	m_panel.add_row(mtr::GridStyle(mtr::SizeType::Absolute, 40.f));
	m_panel.add_row(mtr::GridStyle(mtr::SizeType::Absolute, 40.f));

	m_panel.add_column(mtr::GridStyle(mtr::SizeType::Absolute, 120.f));

	m_label = std::make_shared<mtr::Label>("? FPS", m_font);
	m_panel.add_widget(m_label);

	m_stream.open("assets/audio/megalovania.ogg");
}

void Game::on_key_press(const mtr::KeyPressEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::F1:
		set_cursor_mode(mtr::CursorMode::Normal);
		break;

	case mtr::KeyCode::F2:
		set_cursor_mode(mtr::CursorMode::Disabled);
		break;

	case mtr::KeyCode::F3:
		m_stream.play();
		break;

	case mtr::KeyCode::F4:
		m_stream.set_pitch(m_stream.pitch() + 0.1f);
		m_camera_2d.scale(-0.1f);
		break;

	case mtr::KeyCode::F5:
		m_stream.set_pitch(m_stream.pitch() - 0.1f);
		m_camera_2d.scale(0.1f);
		break;

	case mtr::KeyCode::F6:
		m_stream.stop();
		break;

	case mtr::KeyCode::W:
		m_vertical_axis = VerticalAxis::Up; 
		break;

	case mtr::KeyCode::S:
		m_vertical_axis = VerticalAxis::Down;
		break;

	case mtr::KeyCode::A:
		m_horizontal_axis = HorizontalAxis::Left;
		break;

	case mtr::KeyCode::D:
		m_horizontal_axis = HorizontalAxis::Right;
		break;
	}
}

void Game::on_key_release(const mtr::KeyReleaseEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::Escape:
		quit();
		break;

	case mtr::KeyCode::W:
		if (m_vertical_axis == VerticalAxis::Up)
		{
			m_vertical_axis = VerticalAxis::None;
		}
		break;

	case mtr::KeyCode::S:
		if (m_vertical_axis == VerticalAxis::Down)
		{
			m_vertical_axis = VerticalAxis::None;
		}
		break;

	case mtr::KeyCode::A:
		if (m_horizontal_axis == HorizontalAxis::Left)
		{
			m_horizontal_axis = HorizontalAxis::None;
		}
		break;

	case mtr::KeyCode::D:
		if (m_horizontal_axis == HorizontalAxis::Right)
		{
			m_horizontal_axis = HorizontalAxis::None;
		}
		break;
	}
}

void Game::on_mouse_move(const mtr::MouseMoveEvent& event)
{
	if (m_old_x == -1.f)
	{
		m_old_x = event.position.x;
		m_old_y = event.position.y;
	}
	else
	{
		float offset_x = event.position.x - m_old_x;
		float offset_y = m_old_y - event.position.y;

		m_old_x = event.position.x;
		m_old_y = event.position.y;

		m_camera_3d.rotate(offset_x, offset_y);
	}

	m_panel.on_mouse_move(event);
}

void Game::on_frame_resize(const mtr::FrameResizeEvent& event)
{
	m_camera_2d.set_size(event.size);
	glViewport(0, 0, event.size.x, event.size.y);

	mtr::gl::FramebufferBinder framebuffer_binder(m_framebuffer);
	framebuffer_binder.resize(event.size.x, event.size.y);

	m_panel.set_bounds(mtr::FloatRect::with_size(event.size));
}

void Game::update()
{
	m_label->set_text(std::to_string(m_models[0].meshes()[0].index_buffer().size()));

	switch (m_vertical_axis)
	{
	case VerticalAxis::Up:
		m_camera_3d.move_on_z(mtr::CameraDirection::Positive, CAMERA_SPEED);
		break;

	case VerticalAxis::Down:
		m_camera_3d.move_on_z(mtr::CameraDirection::Negative, CAMERA_SPEED);
		break;
	}

	switch (m_horizontal_axis)
	{
	case HorizontalAxis::Left:
		m_camera_3d.move_on_x(mtr::CameraDirection::Negative, CAMERA_SPEED);
		break;

	case HorizontalAxis::Right:
		m_camera_3d.move_on_x(mtr::CameraDirection::Positive, CAMERA_SPEED);
		break;
	}
}

void Game::draw()
{
	glm::mat4 projection = glm::perspective(glm::radians(m_camera_3d.zoom()), 
		static_cast<float>(window_width()) / static_cast<float>(window_height()), 0.1f, 1000.0f);
	glm::mat4 view = m_camera_3d.view_matrix();
	glm::mat4 cubemap_view = glm::mat4(glm::mat3(m_camera_3d.view_matrix()));

	glm::mat4 model = glm::mat4(1.f);
	model = glm::scale(model, glm::vec3(5.f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	{
		mtr::gl::FramebufferBinder framebuffer_binder(m_framebuffer);
		mtr::gl::clear_color_and_depth(mtr::COLOR_BLACK);

		glDepthMask(false);
		m_cubemap_renderer.render(m_cubemap_texture, projection, cubemap_view);
		glDepthMask(true);

		m_board_renderer.render(m_board_texture, model, projection, view);

		draw_pawns(view, projection);
		draw_bishops(view, projection);
		draw_knights(view, projection);
		draw_kings(view, projection);
		draw_queens(view, projection);
		draw_rooks(view, projection);
	}

	mtr::gl::clear_color_and_depth();

	m_texture_renderer_2d.clear();
	m_texture_renderer_2d.draw(m_framebuffer.color_texture(),
		mtr::FloatRect::with_size({ window_width(), window_height() }), mtr::COLOR_WHITE);
	m_texture_renderer_2d.render(m_camera_2d);

	m_gui_renderer.clear();
	m_panel.draw(m_gui_renderer);
	m_gui_renderer.render(m_camera_2d);

	swap_buffers();
}

void Game::load_framebuffer()
{
	mtr::gl::TextureSettings texture_settings;
	texture_settings.format = mtr::gl::TextureFormat::Rgb16F;
	texture_settings.min_filter = mtr::gl::TextureMinFilter::Linear;
	texture_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_framebuffer = mtr::gl::Framebuffer(window_width(), window_height(), texture_settings);
}

void Game::load_shaders()
{
	auto vertex_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("assets/shaders/model_test/shader.vs.glsl");
	});

	auto fragment_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("assets/shaders/model_test/shader.fs.glsl");
	});

	mtr::gl::ShaderSources sources;
	sources.vertex = vertex_source.get();
	sources.fragment = fragment_source.get();

	m_program = mtr::gl::Program(sources);

}

void Game::load_cubemap()
{
	std::string texture_paths[] = {
		"assets/textures/skybox/ocean/left.jpg",
		"assets/textures/skybox/ocean/right.jpg",
		"assets/textures/skybox/ocean/top.jpg",
		"assets/textures/skybox/ocean/bottom.jpg",
		"assets/textures/skybox/ocean/back.jpg",
		"assets/textures/skybox/ocean/front.jpg"
	};

	std::vector<std::future<mtr::Image>> image_futures;

	for (const auto& texture_path : texture_paths)
	{
		image_futures.push_back(m_thread_pool.execute([&texture_path]() {
			return mtr::Image(texture_path);
		}));
	}

	mtr::gl::CubemapImages images;
	images.left_image = image_futures[0].get();
	images.right_image = image_futures[1].get();
	images.top_image = image_futures[2].get();
	images.bottom_image = image_futures[3].get();
	images.back_image = image_futures[4].get();
	images.front_image = image_futures[5].get();

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void Game::load_board()
{
	mtr::gl::TextureSettings board_settings;
	board_settings.format = mtr::gl::TextureFormat::SRgb;
	board_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	board_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	board_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	board_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_board_texture = mtr::gl::Texture2D(mtr::Image("assets/textures/board/board_top.png"), board_settings);
}

void Game::draw_pawns(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 16> pawn_models =
	{
		glm::translate(model, glm::vec3(60.f - 4.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f - 3.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f - 2.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f - 1.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f,							   0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f + 1.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f + 2.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),
		glm::translate(model, glm::vec3(60.f + 3.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding)),

		glm::translate(rotation, glm::vec3(60.f - 4.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f - 3.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f - 2.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f - 1.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f,							  0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 1.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 2.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 3.f * m_horizontal_padding, 0.f, 0.f - m_vertical_padding + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < pawn_models.size(); ++i)
	{
		binder["model"][i] = pawn_models[i];
	}

	game::draw_model(m_models[0], m_program, projection, view, m_camera_3d.position(), 16);
}

void Game::draw_queens(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 2> queen_models =
	{
		glm::translate(model, glm::vec3(60.f - 1.f * m_horizontal_padding, 0.f, 0.f)),

		glm::translate(rotation, glm::vec3(60.f - 1.f * m_horizontal_padding, 0.f, 0.f + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < queen_models.size(); ++i)
	{
		binder["model"][i] = queen_models[i];
	}

	game::draw_model(m_models[2], m_program, projection, view, m_camera_3d.position(), 2);
}

void Game::draw_kings(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 2> king_models =
	{
		glm::translate(model, glm::vec3(60.f, 0.f, 0.f)),

		glm::translate(rotation, glm::vec3(60.f, 0.f, 0.f + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < king_models.size(); ++i)
	{
		binder["model"][i] = king_models[i];
	}

	game::draw_model(m_models[4], m_program, projection, view, m_camera_3d.position(), 2);
}

void Game::draw_rooks(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 4> rook_models =
	{
		glm::translate(model, glm::vec3(60.f - 4.f * m_horizontal_padding, 0.f, 0.f)),
		glm::translate(model, glm::vec3(60.f + 3.f * m_horizontal_padding, 0.f, 0.f)),

		glm::translate(rotation, glm::vec3(60.f - 4.f * m_horizontal_padding, 0.f, 0.f + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 3.f * m_horizontal_padding, 0.f, 0.f + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < rook_models.size(); ++i)
	{
		binder["model"][i] = rook_models[i];
	}

	game::draw_model(m_models[5], m_program, projection, view, m_camera_3d.position(), 4);
}

void Game::draw_knights(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 4> knight_models =
	{
		glm::translate(model, glm::vec3(60.f - 3.f * m_horizontal_padding, 0.f, 0.f)),
		glm::translate(model, glm::vec3(60.f + 2.f * m_horizontal_padding, 0.f, 0.f)),

		glm::translate(rotation, glm::vec3(60.f - 3.f * m_horizontal_padding, 0.f, 0.f + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 2.f * m_horizontal_padding, 0.f, 0.f + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < knight_models.size(); ++i)
	{
		binder["model"][i] = knight_models[i];
	}

	game::draw_model(m_models[1], m_program, projection, view, m_camera_3d.position(), 4);
}

void Game::draw_bishops(const glm::mat4& view, const glm::mat4& projection)
{
	glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.008f));
	glm::mat4 rotation = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	std::array<glm::mat4, 4> bishop_models =
	{
		glm::translate(model, glm::vec3(60.f - 2.f * m_horizontal_padding, 0.f, 0.f)),
		glm::translate(model, glm::vec3(60.f + 1.f * m_horizontal_padding, 0.f, 0.f)),

		glm::translate(rotation, glm::vec3(60.f - 2.f * m_horizontal_padding, 0.f, 0.f + m_side_padding)),
		glm::translate(rotation, glm::vec3(60.f + 1.f * m_horizontal_padding, 0.f, 0.f + m_side_padding))
	};

	mtr::gl::ProgramBinder binder(m_program);

	for (size_t i = 0; i < bishop_models.size(); ++i)
	{
		binder["model"][i] = bishop_models[i];
	}

	game::draw_model(m_models[3], m_program, projection, view, m_camera_3d.position(), 4);
}

void Game::setup_dir_light()
{
	m_dir_light.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	m_dir_light.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	m_dir_light.diffuse = glm::vec3(10.0f, 10.0f, 10.0f);
	m_dir_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	mtr::gl::ProgramBinder program_binder(m_program);
	program_binder["dir_light_direction"] = m_dir_light.direction;
	program_binder["dir_light_ambient"] = m_dir_light.ambient;
	program_binder["dir_light_diffuse"] = m_dir_light.diffuse;
	program_binder["dir_light_specular"] = m_dir_light.specular;
}

void Game::setup_point_lights()
{
	std::array<glm::vec3, 4> positions = {
		glm::vec3(16.f,  0.f, 0.f),
		glm::vec3(-16.f,  0.f, 0.f),
		glm::vec3(0.f,  5.f, 0.f),
		glm::vec3(0.f, -5.f, 0.f)
	};

	for (int i = 0; i < m_point_light_count; ++i)
	{
		m_point_light[i].position = positions[i];
		m_point_light[i].color = m_point_light[i].diffuse = glm::vec3(1.f, 1.f, 1.f);
		m_point_light[i].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		m_point_light[i].specular = glm::vec3(0.5f, 0.5f, 0.5f);
	}
}
