#include "meteor/core/utils.h"
#include "meteor/graphics/image.h"
#include "meteor/gui/font.h"

#include <nlohmann/json.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace mtr
{
	/*
		Font
	*/
	Font::Font(std::string_view path)
	{
		const nlohmann::json document = nlohmann::json::parse(read_to_string(path));
		float padding = document["padding"].get<float>() * 2.f;

		// Metrics
		m_metrics.size = document["size"].get<float>();
		m_metrics.base = document["base"].get<float>();
		m_metrics.line_height = document["line_height"].get<float>() - padding;

		// Texture
		std::string image_path = fs::path(path).parent_path().append(document["image"].get<std::string>()).string();

		gl::TextureSettings texture_settings;
		texture_settings.min_filter = gl::TextureMinFilter::LinearMipmapLinear;
		texture_settings.mag_filter = gl::TextureMagFilter::Linear;

		m_texture = gl::Texture2D(Image(image_path), texture_settings);

		const auto& glyphs = document["glyphs"];
		m_glyphs.reserve(glyphs.size());

		// Glyphs
		for (const auto& glyph : glyphs)
		{
			uint32_t char_id = glyph["char_id"].get<uint32_t>();
			const auto& bounds = glyph["bounds"];
			const auto& offset = glyph["offset"];
			float advance = glyph["advance"].get<float>() - padding;

			auto& curr_glyph = m_glyphs.insert({ char_id, Glyph() }).first->second;

			curr_glyph.tex_coords.x = bounds[0].get<float>();
			curr_glyph.tex_coords.y = bounds[1].get<float>();
			curr_glyph.tex_coords.w = bounds[2].get<float>();
			curr_glyph.tex_coords.h = bounds[3].get<float>();

			curr_glyph.offset.x = offset[0].get<float>();
			curr_glyph.offset.y = offset[1].get<float>();

			curr_glyph.advance = advance;
		}

		// Kernings
		const auto& kernings = document["kernings"];
		m_kernings.reserve(kernings.size());

		for (const auto& kerning : kernings)
		{
			KerningPair key(
				kerning["first"].get<uint32_t>(), 
				kerning["second"].get<uint32_t>()
			);

			m_kernings[key] = kerning["amount"].get<float>();
		}
	}

	const gl::Texture2D& Font::texture() const
	{
		return m_texture;
	}

	const FontMetrics& Font::metrics() const
	{
		return m_metrics;
	}

	const Font::GlyphMap& Font::glyphs() const
	{
		return m_glyphs;
	}

	const Font::KerningMap& Font::kernings() const
	{
		return m_kernings;
	}

	/*
		Glyph
	*/
	Glyph::Glyph(const FloatRect& tex_coords, const glm::vec2& offset, float advance) :
		tex_coords{ tex_coords },
		offset{ offset },
		advance{ advance }
	{}
}