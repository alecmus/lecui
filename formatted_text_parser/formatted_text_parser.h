//
// formatted_text_parser.h - formatted text parser interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

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
	/// <summary>Direct Write text range properties.</summary>
	struct text_range_properties {
		DWRITE_TEXT_RANGE text_range = { 0, 0 };
		std::string font;
		float font_size = .0f;
		bool bold = false;
		bool italic = false;
		bool underline = false;
		bool strikethrough = false;
		D2D1_COLOR_F color = { 0.f, 0.f, 0.f, -1.f };	// alpha of -1.f indicates color hasn't been set
	};

	/// <summary>Formatted text properties.</summary>
	struct formatted_text_properties {
		/// <summary>And XML representation of the formatted text.</summary>
		xml_parser::xml xml;

		/// <summary>Direct Write text range properties representing the formatting.</summary>
		std::vector<text_range_properties> dwrite;
	};

	/// <summary>Read formatted text.</summary>
	/// <param name="formatted_text">The XML or HTML formatted text.</param>
	/// <param name="default_color">The default color to use for the text.</param>
	/// <returns>The formatted text properties.</returns>
	[[nodiscard]]
	formatted_text_properties read(std::string formatted_text,
		D2D1_COLOR_F default_color);
};
