/*
** formatted_text_parser.h - formatted text parser interface
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

#pragma once

#include <dwrite.h>
#include <string>
#include <d2d1.h>

#include "../xml_parser/xml_parser.h"

/// <summary>Parser for handling formatted text.</summary>
/// <remarks>Uses HTML syntax and CSS style tags. Refer to the in-class documentation for details
/// on which tags are supported.</remarks>
class formatted_text_parser {
public:
	struct text_range_properties {
		DWRITE_TEXT_RANGE text_range = { 0, 0 };
		std::string font;
		float size = .0f;
		bool bold = false;
		bool italic = false;
		bool underline = false;
		bool strikethrough = false;
		D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f };
	};

	struct formatted_text_properties {
		xml_parser::xml xml;
		std::vector<text_range_properties> dwrite;
	};

	formatted_text_properties read(std::string formatted_text,
		D2D1_COLOR_F default_color);
};
