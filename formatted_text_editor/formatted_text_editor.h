/*
** formatted_text_editor.h - formatted text editor interface
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

#include <string>

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
};
