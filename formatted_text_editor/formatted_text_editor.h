//
// formatted_text_editor.h - formatted text editor interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include <string>
#include "../css_parser/css_parser.h"
#include "../xml_parser/xml_parser.h"

/// <summary>Class editing formatted text using position references in the plaintext.</summary>
class formatted_text_editor {
public:
	/// <summary>Insert a character at a given position.</summary>
	/// <param name="c">The character to insert.</param>
	/// <param name="position">The position to insert the character (in reference to the
	/// plaintext version).</param>
	/// <param name="tag_number">The tag number after which to do the insertion.</param>
	/// <param name="xml_text">The XML formatted text. The modifications are written back.</param>
	void insert_character(const char c, unsigned long position, unsigned long tag_number, std::string& xml_text);

	/// <summary>Erase some characters from a formatted string.</summary>
	/// <param name="start">The start position, with reference to the plaintext.</param>
	/// <param name="end">The end position, with reference to the plaintext. This marks the
	/// position of 'start' after the erasure. This is useful for immediate insertion of text in a
	/// way that the user expects, continuing with the formatting of the left-most position.
	/// </param>
	/// <param name="xml_text">The XML formatted text. The modifications are written back.</param>
	/// <returns>The tag number after which any new text can immediately be inserted after the
	/// erasure.</returns>
	[[nodiscard]]
	unsigned long erase(unsigned long start, unsigned long end, std::string& xml_text);

	/// <summary>Cleanup an XML formatted string (removed any dangling empty tags that are
	/// typically left behind after erasure that spans entire tags.</summary>
	/// <param name="xml_text">The XML formatted text. Modification are written back.</param>
	void cleanup(std::string& xml_text);

	/// <summary>For toggling tags, e.g. 'strong', 'em'.</summary>
	/// <param name="xml_text">The XML formatted text.</param>
	/// <param name="tag_name">The tag name, e.g. "span".</param>
	/// <param name="tag_attributes">The tag attributes.</param>
	/// <param name="start">The start position within the plain text.</param>
	/// <param name="end">The end position within the plain text.</param>
	void toggle_tag(std::string& xml_text, const std::string& tag_name,
		const std::vector<xml_parser::tag_attribute>& tag_attributes,
		unsigned long start, unsigned long end);

	/// <summary>For toggling basic tags, e.g. 'strong', 'em'.</summary>
	/// <param name="xml_text">The XML formatted text.</param>
	/// <param name="tag_name">The tag name, e.g. "strong".</param>
	/// <param name="start">The start position within the plain text.</param>
	/// <param name="end">The end position within the plain text.</param>
	void toggle_tag(std::string& xml_text, const std::string& tag_name,
		unsigned long start, unsigned long end);
};
