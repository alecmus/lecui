/*
** formatted_text_editor.cpp - formatted text editor implementation
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

#include "formatted_text_editor.h"
#include <iostream>

void formatted_text_editor::insert_character(const char c, unsigned long position,
	unsigned long tag_number, std::string& xml_text) {
    try {
        // skip all html tags and find the position we need.
		size_t characters = 0;
		size_t tag_characters = 0;

		auto insert = [&]() {
			// we have found the position ... do the insertion
			const auto xml_position = position + tag_characters;

			std::string s;
			s += c;
			xml_text.insert(xml_position, s);
		};

		do {
			const auto open = xml_text.find("<", characters + tag_characters);

			if (open != std::string::npos) {
				characters = open - tag_characters;

				if (tag_number == 0 && position <= characters) {
					// position is before the next tag
					insert();
					break;
				}

				auto close = xml_text.find(">", characters + tag_characters);

				if (close != std::string::npos) {
					if (tag_number > 0)
						tag_number--;

					tag_characters += (close + 1 - open);
				}
				else
					break;
			}
			else {
				characters = xml_text.length() - tag_characters;

				if (position <= characters) {
					// position is after all tags
					insert();
					break;
				}

				break;
			}
		} while (true);
    }
    catch (const std::exception&) {}
}

unsigned long formatted_text_editor::erase(unsigned long start, unsigned long end, std::string& xml_text) {
	unsigned long tag_number = 0;

	// skip all html tags and find the position we need.
	size_t characters = 0;
	size_t tag_characters = 0;

	try {
		do {
			auto open = xml_text.find("<", characters + tag_characters);

			if (open != std::string::npos) {
				characters = open - tag_characters;

				bool advance_tag_number = true;

				if (characters > 0 && start <= characters) {
					if (end <= characters) {
						// entire selection is before the next tag
						const auto xml_start = start + tag_characters;
						const auto xml_end = end + tag_characters;
						xml_text.erase(xml_start, xml_end - xml_start);
						break;
					}
					else {
						// part of the text is to the right of this tag

						// erase that part of the text that is within this tag
						const auto xml_start = start + tag_characters;
						const auto xml_end = characters + tag_characters;
						xml_text.erase(xml_start, xml_end - xml_start);

						const unsigned long erased = static_cast<unsigned long>(xml_end - xml_start);

						// advance start position (this will move just past current tag)
						characters -= erased;
						end -= static_cast<unsigned long>(erased);
						open -= erased;
					}

					advance_tag_number = false;
				}

				auto close = xml_text.find(">", characters + tag_characters);

				if (close != std::string::npos) {
					if (advance_tag_number)
						tag_number++;

					tag_characters += (close + 1 - open);
				}
				else
					break;
			}
			else {
				characters = xml_text.length() - tag_characters;

				if (start <= characters) {
					if (end <= characters) {
						// entire selection is after the last tag
						const auto xml_start = start + tag_characters;
						const auto xml_end = end + tag_characters;
						xml_text.erase(xml_start, xml_end - xml_start);
						break;
					}
				}

				break;
			}
		} while (true);
	}
	catch (const std::exception&) {}

	// do cleanup
	cleanup(xml_text);
	return tag_number;
}

/// assumes there is no whitespace in empty tags
void formatted_text_editor::cleanup(std::string& xml_text) {
	/// this identifier indicates the presence of an empty tag. This same identifier though
	/// can be found between two consecutive closing tags, e.g. <a><b>Just before next 'a'.</b></a>
	const std::string identifier = "></";
	size_t pos = 0;
	size_t mem = 0;

	while ((pos = xml_text.find(identifier, mem)) != std::string::npos) {
		// start marks where the opening tag begins <tag>
		const auto start = xml_text.rfind("<", pos);

		// close marks where the closing tag ends </tag>
		const auto close = xml_text.find(">", pos + identifier.length());

		if (start != std::string::npos && close != std::string::npos) {
			// check if this is indeed between two tags

			const auto len_open = pos + 1 - start;	// length of <tag>
			const auto len_close = close - pos;		// length of </tag>

			// trivial case
			if (len_open + 1 != len_close) {
				mem = close;	// don't search before this position again else stuck in loop
				continue;
			}

			const auto open_tag = xml_text.substr(start + 1, len_open - 2);	// tag
			const auto close_tag = xml_text.substr(pos + 3, len_close - 3);	// tag

			if (open_tag != close_tag) {
				mem = close;	// don't search before this position again else stuck in loop
				continue;
			}

			xml_text.erase(start, close + 1 - start);	// erase the entire tag
		}
		else
			break;
	}
}
