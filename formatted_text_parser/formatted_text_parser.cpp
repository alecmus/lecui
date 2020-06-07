/*
** formatted_text_parser.cpp - formatted text parser implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "formatted_text_parser.h"
#include "../css_parser/css_parser.h"
#include <sstream>
#include <boost/lexical_cast.hpp>

formatted_text_parser::formatted_text_properties
formatted_text_parser::read(std::string formatted_text,
	D2D1_COLOR_F default_color) {
    formatted_text_properties properties;

    // parse the formatted text into xml
    properties.xml = xml_parser().read(formatted_text);

	auto parse_css_font_size = [](const std::string& text) {
		float font_size = 0.f;

		try {
			if (text.find("pt") != std::string::npos) {
				// font size is described in points
				std::stringstream ss;
				ss << text;
				ss >> font_size;
			}
			else
				if (text.find("px") != std::string::npos) {
					float pixels = 0.f;
					// font size is described in pixels
					std::stringstream ss;
					ss << text;
					ss >> pixels;

					// to-do: convert from pixels to points
				}
				else {
					// to-do: handle sizes that are described in neither points nor pixels
				}
		}
		catch (const std::exception&) {}

		return font_size;
	};

	auto split_line = [](const std::string & line,
		std::string seperator) {
		std::vector<std::string> data;

		auto add = [&](std::string s) {
			// trim leading whitespace
			s.erase(0, s.find_first_not_of(" "));

			// trim trailing whitespace
			s.erase(s.find_last_not_of(" ") + 1);

			if (!s.empty())
				data.push_back(s);
		};

		try {
			data.clear();

			std::string line_(line);

			while (true) {
				auto idx = line_.find(seperator);

				if (idx == std::string::npos) {
					add(line_);
					break;
				}

				auto s = line_.substr(0, idx);
				line_.erase(0, idx + 1);
				add(s);
			}
		}
		catch (const std::exception&) {}

		return data;
	};

	auto parse_css_color = [&](std::string text) {
		// make all text lower caps
		for (auto& c : text)
			c = tolower(c);

		D2D1_COLOR_F color = default_color;

		try {
			if (text.find("rgb") != std::string::npos) {
				// color is described in rgb
				
				// trim opening brace
				text.erase(0, text.find("(") + 1);

				// trim trailing brace
				text.erase(text.find(")"));

				const auto rgb = split_line(text, ",");

				if (rgb.size() == 3) {
					float r = 0.f, g = 0.f, b = 0.f;

					std::stringstream ss;
					ss << rgb[0];
					ss >> r;
					ss.clear();

					ss << rgb[1];
					ss >> g;
					ss.clear();

					ss << rgb[2];
					ss >> b;
					ss.clear();

					color.r = r / 255.f;
					color.g = g / 255.f;
					color.b = b / 255.f;
					color.a = 1.f;
				}
			}
			else
				if (text.find("0x") != std::string::npos ||
					text.find("#") != std::string::npos) {
					// append missing
					if (text[0] == '#') {
						text.erase(0, 1);
						text = "0x" + text;
					}

					for (size_t i = text.length(); i < 10; i++)
						text += "f";

					// color is described in hexadecimal
					std::stringstream ss;
					ss << text;

					unsigned long hex = 0;
					ss >> std::hex >> hex;

					const auto r = boost::lexical_cast<float>((hex >> 24) & 0xff);
					const auto g = boost::lexical_cast<float>((hex >> 16) & 0xff);
					const auto b = boost::lexical_cast<float>((hex >> 8) & 0xff);
					const auto a = boost::lexical_cast<float>((hex >> 0) & 0xff);

					color.r = static_cast<float>(r) / 255.f;
					color.g = static_cast<float>(g) / 255.f;
					color.b = static_cast<float>(b) / 255.f;
					color.a = static_cast<float>(a) / 255.f;
				}
				else {
					// to-do: handle color that is described using a word, e.g. "red"
				}
		}
		catch (const std::exception&) {}

		return color;
	};

	// make text_range_properties from the xml
	for (auto& tag : properties.xml.tags) {
		text_range_properties props;
		props.text_range.startPosition = tag.start_position;
		props.text_range.length = tag.length;

		// handle strong tag
		if (tag.name == "strong") {
			props.bold = true;
			properties.dwrite.push_back(props);
			continue;
		}

		// handle emphasis tag
		if (tag.name == "em") {
			props.italic = true;
			properties.dwrite.push_back(props);
			continue;
		}

		// handle underline tag
		if (tag.name == "u") {
			props.underline = true;
			properties.dwrite.push_back(props);
			continue;
		}

		// handle strikethrough tag
		if (tag.name == "s") {
			props.strikethrough = true;
			properties.dwrite.push_back(props);
			continue;
		}

		// handle span tag
		if (tag.name == "span") {
			for (const auto& tag_attribute : tag.attributes) {
				// handle CSS style attribute
				if (tag_attribute.name == "style") {
					const auto properties = css_parser().read(tag_attribute.value);

					for (const auto& css_property : properties) {
						// handle CSS style properties
						if (css_property.property == "font-family") {
							props.font = css_property.value;
							continue;
						}

						if (css_property.property == "font-size") {
							props.font_size = parse_css_font_size(css_property.value);
							continue;
						}

						if (css_property.property == "font-weight") {
							props.bold = css_property.value == "bold";
							continue;
						}

						if (css_property.property == "font-style") {
							props.italic = css_property.value == "italic";
							continue;
						}

						if (css_property.property == "text-decoration") {
							if (css_property.value == "underline") {
								props.underline = true;
								continue;
							}
							if (css_property.value == "line-through") {
								props.strikethrough = true;
								continue;
							}
							continue;
						}

						if (css_property.property == "color") {
							props.color = parse_css_color(css_property.value);
							continue;
						}
					}
				}
			}
			properties.dwrite.push_back(props);
			continue;
		}

		// handle our own in-house text tag
		if (tag.name == "text") {
			for (const auto& [key, value] : tag.attributes) {
				if (key == "bold")
					props.bold = value == "true";
				else
					if (key == "italic")
						props.italic = value == "true";
					else
						if (key == "underline")
							props.underline = value == "true";
						else
							if (key == "strikethrough")
								props.strikethrough = value == "true";
							else
								if (key == "font")
									props.font = value;
								else
									if (key == "size") {
										try { props.font_size = boost::lexical_cast<float>(value); }
										catch (const std::exception&) {}
									}
									else
										if (key == "color")
											props.color = parse_css_color(value);
			}

			properties.dwrite.push_back(props);
			continue;
		}
	}

    return properties;
}
