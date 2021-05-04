/*
** css_parser.h - css parser interface
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#pragma once

#include <string>
#include <vector>

/// <summary>A custom CSS parser that describes CSS styles in a data structure.</summary>
class css_parser {
public:
	/// <summary>A CSS property.</summary>
	struct css_property {
		/// <summary>The name of the property, e.g. color.</summary>
		std::string property;

		/// <summary>The value of the property, e.g. rgb(12, 45, 125).</summary>
		std::string value;
	};

	/// <summary>Read CSS properties from a CSS style string.</summary>
	/// <param name="style_string">The CSS style string, e.g.
	/// "font-family: Segoe UI; color: rgb(0, 150, 0);"</param>
	/// <returns>The CSS properties.</returns>
	std::vector<css_property> read(const std::string style_string);

	/// <summary>Write CSS properties to a CSS style string.</summary>
	/// <param name="css_properties">The CSS properties.</param>
	/// <returns>The CSS style string.</returns>
	std::string write(const std::vector<css_property>& css_properties);
};
