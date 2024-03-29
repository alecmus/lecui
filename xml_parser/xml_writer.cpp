//
// xml_writer.cpp - xml writer implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

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
		if (!(tag.start_position >= position && (tag.start_position + tag.length) <= end && tag.level == level))
			continue;

		const auto tag_id = tag.name + std::to_string(tag.start_position) + std::to_string(tag.start_position + tag.length);

		if (!handled_tags.insert(tag_id).second)
			continue;

		std::string before = plain.substr(0, tag.start_position - position);
		std::string after = plain.substr(tag.start_position + tag.length - position);

		// insert tags to original plain text
		auto start_tag = tag.name;

		if (!tag.attributes.empty()) {
			// add attribute string
			std::string attribute_string;

			for (const auto& attribute : tag.attributes) {
				if (!attribute.value.empty())
					attribute_string += " " + attribute.name + "='" + attribute.value + "'";
			}

			start_tag += attribute_string;
		}

		start_tag = "<" + start_tag + ">";
		auto end_tag = "</" + tag.name + ">";

		for (auto sub_tag : tags) {
			if (sub_tag.start_position >= tag.start_position && (sub_tag.start_position + sub_tag.length) <= (tag.start_position + tag.length) && sub_tag.level == tag.level + 1) {
				// this is a direct sub tag, get it's subelements
				auto sub_elements = do_build(tag.text, tag.start_position, (tag.start_position + tag.length), sub_tag.level, tags, handled_tags);	// recursion

				for (auto& element : sub_elements) {
					element.start -= tag.start_position;
					element.end -= tag.start_position;
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
		element.start = tag.start_position;
		element.end = tag.start_position + tag.length;
		elements.push_back(element);
	}

	return elements;
}

std::string xml_parser::write(const xml& info) {
	std::set<std::string> handled_tags;
	const auto xml_elements = do_build(info.plain_text, 0, info.plain_text.length(), 0, info.tags, handled_tags);
	return consolidate(info.plain_text, xml_elements);
}
