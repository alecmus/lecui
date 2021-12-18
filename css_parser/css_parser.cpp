//
// css_parser.cpp - css parser implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "css_parser.h"

std::vector<std::string> split_line(const std::string& line,
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

		std::string _line(line);

		while (true) {
			auto idx = _line.find(seperator);

			if (idx == std::string::npos) {
				add(_line);
				break;
			}

			auto s = _line.substr(0, idx);
			_line.erase(0, idx + 1);
			add(s);
		}
	}
	catch (const std::exception&) {}

	return data;
}

std::vector<css_parser::css_property>
css_parser::read(const std::string style_string) {
	std::vector<css_property> css_properties;

	// get the segments
	const auto segments = split_line(style_string, ";");

	for (const auto& segment : segments) {
		// get the segment parts
		const auto parts = split_line(segment, ":");

		if (parts.size() == 2) {
			// get the property
			css_properties.push_back(css_property{ parts[0], parts[1] });
		}
	}

	return css_properties;
}

std::string
css_parser::write(const std::vector<css_property>& css_properties) {
	std::string style_string;

	for (const auto& css_property : css_properties) {
		const auto gap = style_string.empty() ? std::string("") : std::string(" ");
		if (!css_property.property.empty())
			style_string += (gap + css_property.property + ": " + css_property.value + ";");
	}

	return style_string;
}
