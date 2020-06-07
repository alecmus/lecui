/*
** xml_writer.cpp - xml writer implementation
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
#include <set>

struct xml_element {
	std::string xml;
	size_t start = 0;
	size_t end = 0;
};

std::string consolidate(std::string plain, const std::vector<xml_element>& elements) {
	std::string xml = plain;

	// loop backwards
	for (size_t i = elements.size(); i > 0; i--) {
		auto& element = elements[i - 1];

		std::string before = xml.substr(0, element.start);
		std::string after = xml.substr(element.end);

		xml = before + element.xml + after;
	}

	return xml;
}

std::vector<xml_element> do_build(std::string plain,
	size_t position, size_t end, long level, std::vector<xml_parser::tag> tags,
	std::set<std::string>& handled_tags) {
	std::vector<xml_element> elements;

	for (auto& tag : tags) {
		if (!(tag.start >= position && tag.end <= end && tag.level == level))
			continue;

		const auto tag_id = tag.name + std::to_string(tag.start) + std::to_string(tag.end);

		if (!handled_tags.insert(tag_id).second)
			continue;

		std::string before = plain.substr(0, tag.start - position);
		std::string after = plain.substr(tag.end - position);

		// insert tags to original plain text
		auto start_tag = tag.name;

		if (!tag.attributes.empty()) {
			// add attribute string
			std::string attribute_string;

			for (const auto& attribute : tag.attributes)
				attribute_string += " " + attribute.name + "=\"" + attribute.value + "\"";

			start_tag += attribute_string;
		}

		start_tag = "<" + start_tag + ">";
		auto end_tag = "</" + tag.name + ">";

		for (auto sub_tag : tags) {
			if (sub_tag.start >= tag.start && sub_tag.end <= tag.end && sub_tag.level == tag.level + 1) {
				// this is a direct sub tag, get it's subelements
				auto sub_elements = do_build(tag.text, tag.start, tag.end, sub_tag.level, tags, handled_tags);	// recursion

				for (auto& element : sub_elements) {
					element.start -= tag.start;
					element.end -= tag.start;
				}

				auto sub_xml = consolidate(tag.text, sub_elements);

				if (sub_xml.length() != tag.text.length()) {
					// do the replacement
					tag.text = sub_xml;
				}
			}
		}

		xml_element element;
		element.xml = start_tag + tag.text + end_tag;
		element.start = tag.start;
		element.end = tag.end;
		elements.push_back(element);
	}

	return elements;
}

std::string xml_parser::write(const xml& info) {
	std::set<std::string> handled_tags;
	const auto xml_elements = do_build(info.plain_text, 0, info.plain_text.length(), 0, info.tags, handled_tags);
	return consolidate(info.plain_text, xml_elements);
}
