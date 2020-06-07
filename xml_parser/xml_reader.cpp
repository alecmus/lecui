/*
** xml_reader.cpp - xml reader implementation
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

#include "xml_parser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <algorithm>

std::string get_plain(std::string xml_text) {
	do {
		const auto open = xml_text.find("<");

		if (open != std::string::npos) {
			auto close = xml_text.find(">");

			if (close != std::string::npos)
				xml_text.erase(open, close + 1 - open);
			else
				break;
		}
		else
			break;
	} while (true);

	return xml_text;
}

std::string do_parse(const std::string xml_text, size_t position, long level, std::vector<xml_parser::tag>& tags) {
	// find a tag
	std::string start_tag, end_tag;
	size_t start_pos = 0, end_pos = 0;

	auto open = xml_text.find("<");

	if (open != std::string::npos) {
		auto close = xml_text.find(">");

		if (close != std::string::npos) {
			// capture start tag
			start_tag = xml_text.substr(open, close + 1 - open);

			// isf there are spaces, break at the first occurrence of one
			auto space_idx = start_tag.find(" ");

			if (space_idx != std::string::npos) {
				start_tag.erase(space_idx);
				start_tag += ">";
			}

			// capture start position (actual index in plain string)
			start_pos = open;

			// find matching close tag
			end_tag = start_tag;
			end_tag.insert(1, "/");

			auto idx = xml_text.find(end_tag);

			// check if this is indeed the matching tag ... there could be another tag of the same name a level deeper
			if (idx != std::string::npos) {
				// check how many opening tags of the same nature there are between open and idx
				size_t count = 0;

				auto to_find = start_tag.substr(0, start_tag.length() - 1);

				size_t pos = open + 1;

				do {
					auto idx_a = xml_text.find(to_find, pos);

					if (idx_a != std::string::npos && idx_a < idx) {
						// check if indeed it is a match ... next character has to be either a space or a closing chevron
						const auto c = xml_text[idx_a + to_find.length()];

						if (c == ' ' || c == '>')
							count++;

						// advance
						pos = idx_a + 1;
					}
					else
						break;
				} while (true);

				// skip as many as 'count - 1' closing tags
				pos = idx + 1;
				for (size_t i = 0; i < count; i++) {
					idx = xml_text.find(end_tag, pos);

					if (idx == std::string::npos)
						break;
					else
						pos = idx + 1;
				}
			}

			if (idx != std::string::npos) {
				// matching closing tag found
				std::vector<xml_parser::tag_attribute> attributes;

				if (space_idx != std::string::npos) {
					try {
						// get the xml
						const auto xml = xml_text.substr(open, idx + end_tag.length() - open);

						// create an empty property tree object
						boost::property_tree::ptree tree;

						// parse the xml into the property tree
						std::stringstream ss;
						ss << xml;
						boost::property_tree::read_xml(ss, tree);

						// get attributes
						for (auto& it : tree) {
							for (auto& m_it : it.second) {
								if (m_it.first == "<xmlattr>") {
									for (auto& [attribute_name, attribute_node] : m_it.second)
										attributes.push_back({ attribute_name, attribute_node.data() });
								}
							}
						}
					}
					catch (const std::exception&) {}
				}

				// parse one level deeper between the start tag and the end tag
				const auto sub_plain = do_parse(xml_text.substr(close + 1, idx - (close + 1)),
					position + start_pos, level + 1, tags);	// recursion

				end_pos = start_pos + sub_plain.length() - 1;

				// capture tag info
				tags.push_back({ level, start_tag.substr(1, start_tag.length() - 2), sub_plain, position + start_pos, position + end_pos + 1, attributes });

				// parse the rest of the string (same level)
				do_parse(xml_text.substr(idx + end_tag.length()), position + end_pos + 1, level, tags);
			}
			else {
				// matching closing tag NOT found

				// parse the rest of the string (same level)
				do_parse(xml_text.substr(close + 1), position + open, level, tags);
			}
		}
	}

	return get_plain(xml_text);
}

xml_parser::xml xml_parser::read(std::string xml_text) {
	xml info;

	// parse the xml, capturing the tags
	info.plain_text = do_parse(xml_text, 0, 0, info.tags);

	auto compare = [](xml_parser::tag& tag1, xml_parser::tag& tag2)->bool {
		if (tag1.level < tag2.level)
			return true;
		else
			return false;
	};

	// sort tags according to level (critically important!)
	std::sort(info.tags.begin(), info.tags.end(), compare);

	return info;
}
