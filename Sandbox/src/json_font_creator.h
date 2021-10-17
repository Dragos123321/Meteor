#pragma once

#include <nlohmann/json.hpp>

#include <meteor/core/utils.h>

#include <sstream>

nlohmann::json create_json_font(std::string_view path)
{
	std::istringstream input;
	input.str(mtr::read_to_string(path));

	auto next_int = [&input]() {
		int value = 0;
		input >> value;
		return value;
	};

	auto next_string = [&input]() {
		std::string value;
		input >> value;
		return value;
	};

	nlohmann::json output;
	output["image"] = next_string();
	output["size"] = next_int();
	output["base"] = next_int();
	output["line_height"] = next_int();
	output["padding"] = next_int();

	auto& glyphs = output["glyphs"] = nlohmann::json::array();
	int glyph_count = next_int();

	for (int i = 0; i != glyph_count; ++i)
	{
		glyphs.push_back({
			{ "char_id", next_int() },
			{ "bounds", { next_int(), next_int(), next_int(), next_int() } },
			{ "offset", { next_int(), next_int() } },
			{ "advance", next_int() }
		});
	}

	auto& kernings = output["kernings"] = nlohmann::json::array();
	int kerning_count = next_int();

	for (int i = 0; i != kerning_count; ++i)
	{
		kernings.push_back({
			{ "first", next_int() },
			{ "second", next_int() },
			{ "amount", next_int() }
		});
	}

	return output;
}