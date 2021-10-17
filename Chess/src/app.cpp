#include "app.h"

#include <meteor/graphics/opengl/drawing.h>
#include <meteor/graphics/model_loader.h>
#include <meteor/graphics/opengl/shader_sources.h>
#include <meteor/core/utils.h>

#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"

App::App(const mtr::Config& config) :
	mtr::Application{ config },
	m_camera_2d{ static_cast<float>(window_width()), static_cast<float>(window_height()) },
	m_scene{ window_width(), window_height() }
{
	set_mouse_tracking_enabled(true);

	load_board_texture();
	load_framebuffer();
	load_shaders();
	load_cubemaps();
	load_models();

	init_game();
	init_lights();
}

void App::on_key_press(const mtr::KeyPressEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::F1:
		set_cursor_mode(mtr::CursorMode::Normal);
		break;

	case mtr::KeyCode::F2:
		set_cursor_mode(mtr::CursorMode::Disabled);
		break;
	}
}

void App::on_key_release(const mtr::KeyReleaseEvent& event)
{
	switch (event.key_code)
	{
	case mtr::KeyCode::Escape:
		quit();
		break;
	}
}

void App::on_frame_resize(const mtr::FrameResizeEvent& event)
{
	m_camera_2d.set_size(event.size);
	glViewport(0, 0, event.size.x, event.size.y);

	mtr::gl::FramebufferBinder framebuffer_binder(m_framebuffer);
	framebuffer_binder.resize(event.size.x, event.size.y);
}

void App::on_button_press(const mtr::ButtonPressEvent& event)
{
	process_selection(static_cast<int>(event.position.x), static_cast<int>(event.position.y));
}

void App::update()
{
	for (size_t i = 0; i < m_scene.size(); ++i)
	{
		PieceManager& piece = m_scene[i];

		if (piece.is_movement_requested() || piece.is_jump_movement_requested() || piece.is_eject_movement_requested())
			piece.update();
	}
}

void App::draw()
{
	glm::mat4 projection = glm::perspective(game::FOV,
		static_cast<float>(window_width()) / static_cast<float>(window_height()), 0.1f, 10000.0f);
	glm::mat4 view = m_scene.camera().view_matrix();
	glm::mat4 cubemap_view = glm::mat4(glm::mat3(view));

	{
		mtr::gl::FramebufferBinder binder(m_framebuffer);

		mtr::gl::clear_color_and_depth(mtr::COLOR_BLACK);

		glDepthMask(false);
		switch (m_game.current_cubemap())
		{
		case 0:
			m_cubemap_renderer.render(m_default_cubemap_texture, projection, cubemap_view);
			break;
		case 1:
			m_cubemap_renderer.render(m_yellow_cubemap_texture, projection, cubemap_view);
			break;
		case 2:
			m_cubemap_renderer.render(m_red_cubemap_texture, projection, cubemap_view);
			break;
		}
		glDepthMask(true);

		render_models(view, projection, m_program);
	}

	mtr::gl::clear_color_and_depth();

	m_texture_renderer_2d.clear();
	m_texture_renderer_2d.draw(m_framebuffer.color_texture(),
							   mtr::FloatRect::with_size({ window_width(), window_height() }), mtr::COLOR_WHITE);
	m_texture_renderer_2d.render(m_camera_2d);

	swap_buffers();
}

void App::load_framebuffer()
{
	mtr::gl::TextureSettings texture_settings;
	texture_settings.format = mtr::gl::TextureFormat::Rgb16F;
	texture_settings.min_filter = mtr::gl::TextureMinFilter::Linear;
	texture_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_framebuffer = mtr::gl::Framebuffer(window_width(), window_height(), texture_settings);
}

void App::load_shaders()
{
	auto vertex_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/shader.vs.glsl");
		});

	auto fragment_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/shader.fs.glsl");
		});

	mtr::gl::ShaderSources sources;
	sources.vertex = vertex_source.get();
	sources.fragment = fragment_source.get();

	m_program = mtr::gl::Program(sources);

	// Board program
	auto board_vertex_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/board_shader.vs.glsl");
		});

	auto board_fragment_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/board_shader.fs.glsl");
		});

	sources.vertex = board_vertex_source.get();
	sources.fragment = board_fragment_source.get();

	m_board_program = mtr::gl::Program(sources);

	// Selection Program
	auto selection_vertex_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/selection_shader.vs.glsl");
		});

	auto selection_fragment_source = m_thread_pool.execute([]() {
		return mtr::read_to_string("chess_assets/shaders/selection_shader.fs.glsl");
		});

	sources.vertex = selection_vertex_source.get();
	sources.fragment = selection_fragment_source.get();

	m_selection_program = mtr::gl::Program(sources);
}

void App::load_default_cubemap()
{
	mtr::gl::CubemapImages images;
	images.left_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/left.jpg");
	images.right_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/right.jpg");
	images.top_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/top.jpg");
	images.bottom_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/bottom.jpg");
	images.back_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/back.jpg");
	images.front_image = mtr::Image("chess_assets/textures/skyboxes/default_skybox/front.jpg");

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_default_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void App::load_yellow_cubemap()
{
	mtr::gl::CubemapImages images;
	images.left_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/left.jpg");
	images.right_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/right.jpg");
	images.top_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/top.jpg");
	images.bottom_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/bottom.jpg");
	images.back_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/back.jpg");
	images.front_image = mtr::Image("chess_assets/textures/skyboxes/yellow_skybox/front.jpg");

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_yellow_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void App::load_red_cubemap()
{
	mtr::gl::CubemapImages images;
	images.left_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/left.jpg");
	images.right_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/right.jpg");
	images.top_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/top.jpg");
	images.bottom_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/bottom.jpg");
	images.back_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/back.jpg");
	images.front_image = mtr::Image("chess_assets/textures/skyboxes/red_skybox/front.jpg");

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_red_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void App::load_white_cubemap()
{
	mtr::gl::CubemapImages images;
	images.left_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/left.jpg");
	images.right_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/right.jpg");
	images.top_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/top.jpg");
	images.bottom_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/bottom.jpg");
	images.back_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/back.jpg");
	images.front_image = mtr::Image("chess_assets/textures/skyboxes/white_skybox/front.jpg");

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_white_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void App::load_space_cubemap()
{
	mtr::gl::CubemapImages images;
	images.left_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/left.jpg");
	images.right_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/right.jpg");
	images.top_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/top.jpg");
	images.bottom_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/bottom.jpg");
	images.back_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/back.jpg");
	images.front_image = mtr::Image("chess_assets/textures/skyboxes/space_skybox/front.jpg");

	mtr::gl::TextureSettings cubemap_settings;
	cubemap_settings.format = mtr::gl::TextureFormat::SRgb;
	cubemap_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	cubemap_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	cubemap_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_space_cubemap_texture = mtr::gl::CubemapTexture(images, cubemap_settings);
}

void App::load_cubemaps()
{
	load_default_cubemap();
	load_yellow_cubemap();
	load_red_cubemap();
	load_white_cubemap();
	load_space_cubemap();
}

void App::load_models()
{
	m_bishop_model = mtr::load_model("chess_assets/pieces/fou.obj", m_thread_pool).build();
	m_knight_model = mtr::load_model("chess_assets/pieces/cavalier.obj", m_thread_pool).build();
	m_king_model = mtr::load_model("chess_assets/pieces/roi.obj", m_thread_pool).build();
	m_pawn_model = mtr::load_model("chess_assets/pieces/pion.obj", m_thread_pool).build();
	m_queen_model = mtr::load_model("chess_assets/pieces/dame.obj", m_thread_pool).build();
	m_rook_model = mtr::load_model("chess_assets/pieces/tour.obj", m_thread_pool).build();
	m_board_model = mtr::load_model("chess_assets/pieces/plane.obj", m_thread_pool).build();
}

void App::load_board_texture()
{
	mtr::Image image = mtr::Image("chess_assets/textures/board/board.tga");

	mtr::gl::TextureSettings board_settings;
	board_settings.format = mtr::gl::TextureFormat::SRgb;
	board_settings.wrap_s = mtr::gl::TextureWrap::ClampToEdge;
	board_settings.wrap_t = mtr::gl::TextureWrap::ClampToEdge;
	board_settings.wrap_r = mtr::gl::TextureWrap::ClampToEdge;
	board_settings.min_filter = mtr::gl::TextureMinFilter::LinearMipmapLinear;
	board_settings.mag_filter = mtr::gl::TextureMagFilter::Linear;

	m_board_texture = mtr::gl::Texture2D(image, board_settings);
}

void App::init_game()
{
	m_game.init_classic_game(&m_scene);
}

void App::init_lights()
{
	std::array<glm::vec3, 2> positions = {
		glm::vec3( 400.f, 400.f,  400.f),
		glm::vec3(-400.f, 400.f, -400.f)
	};

	for (int i = 0; i < m_nr_point_lights; ++i)
	{
		m_point_light[i].position = positions[i];
		m_point_light[i].color = m_point_light[i].diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		m_point_light[i].ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		m_point_light[i].specular = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	mtr::gl::ProgramBinder program_binder(m_program);

	float ecuation_const = m_point_light[0].ecuation_consts[0] * 0.000015f;
	float ecuation_linear = m_point_light[0].ecuation_consts[1] * 0.00002f;
	float ecuation_quadratic = m_point_light[0].ecuation_consts[2] * 0.00002f;

	for (int i = 0; i < m_nr_point_lights; ++i)
	{
		program_binder["point_light[" + std::to_string(i) + "].position"] = m_point_light[i].position;
		program_binder["point_light[" + std::to_string(i) + "].ambient"] = m_point_light[i].ambient;
		program_binder["point_light[" + std::to_string(i) + "].diffuse"] = m_point_light[i].diffuse;
		program_binder["point_light[" + std::to_string(i) + "].specular"] = m_point_light[i].specular;
		program_binder["point_light[" + std::to_string(i) + "].constant"] = ecuation_const;
		program_binder["point_light[" + std::to_string(i) + "].linear"] = ecuation_linear;
		program_binder["point_light[" + std::to_string(i) + "].quadratic"] = ecuation_quadratic;
	}

	mtr::gl::CONTEXT->unbind_program();

	mtr::gl::ProgramBinder board_program_binder(m_board_program);

	for (int i = 0; i < m_nr_point_lights; ++i)
	{
		board_program_binder["point_light[" + std::to_string(i) + "].position"] = m_point_light[i].position;
		board_program_binder["point_light[" + std::to_string(i) + "].ambient"] = m_point_light[i].ambient;
		board_program_binder["point_light[" + std::to_string(i) + "].diffuse"] = m_point_light[i].diffuse;
		board_program_binder["point_light[" + std::to_string(i) + "].specular"] = m_point_light[i].specular;
		board_program_binder["point_light[" + std::to_string(i) + "].constant"] = ecuation_const;
		board_program_binder["point_light[" + std::to_string(i) + "].linear"] = ecuation_linear;
		board_program_binder["point_light[" + std::to_string(i) + "].quadratic"] = ecuation_quadratic;
	}
}

void App::render_models(const glm::mat4& view, const glm::mat4& projection, mtr::gl::Program& program)
{
	glm::mat4 model = glm::mat4(1.f);

	for (size_t i = 0; i < m_scene.size(); ++i)
	{
		PieceManager& piece = m_scene[i];

		model = piece.get_model_matrix();

		if (piece.name() == "Plane")
		{
			glm::mat4 inv_view = glm::inverse(view);
			mtr::gl::ProgramBinder binder(m_board_program);
			mtr::gl::Texture2DBinder texture_binder(m_board_texture);
			binder["model"] = model;

			game::draw_model(m_board_model, m_board_program, projection, view, m_scene.camera().position(), true);
		}

		else
		{
			mtr::gl::ProgramBinder binder(program);
			binder["model"] = model;

			if (m_scene.selected() && m_scene.get_selected() == i)
				binder["diffuse_color"] = glm::vec3(1.f, 1.f, 0.f);
			else
				binder["diffuse_color"] = piece.color();

			if (piece.name() == "Bishop")
			{
				game::draw_model(m_bishop_model, program, projection, view, m_scene.camera().position(), true);
			}

			if (piece.name() == "Knight")
			{
				game::draw_model(m_knight_model, program, projection, view, m_scene.camera().position(), true);
			}

			if (piece.name() == "King")
			{
				game::draw_model(m_king_model, program, projection, view, m_scene.camera().position(), true);
			}

			if (piece.name() == "Pawn")
			{
				game::draw_model(m_pawn_model, program, projection, view, m_scene.camera().position(), true);
			}

			if (piece.name() == "Queen")
			{
				game::draw_model(m_queen_model, program, projection, view, m_scene.camera().position(), true);
			}

			if (piece.name() == "Rook")
			{
				game::draw_model(m_rook_model, program, projection, view, m_scene.camera().position(), true);
			}
		}
	}
}

void App::render_selection()
{
	glm::mat4 projection = glm::perspective(game::FOV,
		static_cast<float>(window_width()) / static_cast<float>(window_height()), 0.1f, 100000.0f);
	glm::mat4 view = m_scene.camera().view_matrix();

	mtr::gl::clear_color_and_depth(mtr::COLOR_BLACK);

	glm::mat4 model = glm::mat4(1.f);

	for (size_t i = 0; i < m_scene.size(); ++i)
	{
		PieceManager& piece = m_scene[i];

		model = piece.get_model_matrix();
		mtr::gl::ProgramBinder binder(m_selection_program);

		if (piece.name() == "Plane")
		{
			binder["model"] = model;
			binder["texture_enabled"] = true;

			game::draw_model(m_board_model, m_selection_program, projection, view, m_scene.camera().position());
		}

		else
		{
			binder["model"] = model;
			binder["texture_enabled"] = false;
			binder["code"] = piece.get_id();

			if (piece.name() == "Bishop")
			{
				game::draw_model(m_bishop_model, m_selection_program, projection, view, m_scene.camera().position());
			}

			if (piece.name() == "Knight")
			{
				game::draw_model(m_knight_model, m_selection_program, projection, view, m_scene.camera().position());
			}

			if (piece.name() == "King")
			{
				game::draw_model(m_king_model, m_selection_program, projection, view, m_scene.camera().position());
			}

			if (piece.name() == "Pawn")
			{
				game::draw_model(m_pawn_model, m_selection_program, projection, view, m_scene.camera().position());
			}

			if (piece.name() == "Queen")
			{
				game::draw_model(m_queen_model, m_selection_program, projection, view, m_scene.camera().position());
			}

			if (piece.name() == "Rook")
			{
				game::draw_model(m_rook_model, m_selection_program, projection, view, m_scene.camera().position());
			}
		}
	}
}

void App::process_selection(int x, int y)
{
	unsigned char res[4];
	GLint viewport[4];

	render_selection();

	float x_scale = 1.f;
	float y_scale = 1.f;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(static_cast<GLint>(x * x_scale), viewport[3] - static_cast<GLint>(y * y_scale), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	int selected = static_cast<int>(res[0]);

	if (selected < 100 && selected >= 0)
	{
		if (m_scene.selected() && ((m_game.get_player_id() == 1 && selected > 16) || (m_game.get_player_id() == 2 && selected < 17)))
			m_game.try_movement(m_scene.get_selected() + 1, selected + 1);

		else if ((m_game.get_player_id() == 1 && selected <= 16) || (m_game.get_player_id() == 2 && selected > 16))
			m_scene.select_model(selected);
	}

	else
	{
		selected -= 100;
		int caseY = selected % 8;
		int caseX = 7 - (selected / 8);

		if (m_scene.selected())
			m_game.try_movement(m_scene.get_selected() + 1, caseX, caseY);
	}
}
