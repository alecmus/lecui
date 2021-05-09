//
// formatted_text_editor.cpp - formatted text editor implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "formatted_text_editor.h"
#include "../xml_parser/xml_parser.h"
#include "../form_common.h"
#include <algorithm>

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

			auto len_open = pos + 1 - start - 2;	// length of <tag ...>
			auto len_close = close - pos - 3;		// length of </tag>

			// check if tag has attributes
			auto space_idx = xml_text.find(" ", start + 1);
			if (space_idx != std::string::npos && space_idx < close) {
				// there are attributes ... factor them in
				len_open = space_idx - (start + 1);
			}

			const auto open_tag = xml_text.substr(start + 1, len_open);	// tag
			const auto close_tag = xml_text.substr(pos + 3, len_close);	// tag

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

void formatted_text_editor::toggle_tag(std::string& xml_text, const std::string& tag_name,
	const std::vector<xml_parser::tag_attribute>& tag_attributes,
	unsigned long start, unsigned long end) {
	if (start == end)
		return;	// do nothing

	if (start > end) {
		auto temp = start;
		start = end;
		end = temp;
	}

	try {
		// parse the xml text
		auto xml = xml_parser().read(xml_text);

		// do some tag switches
		for (auto& tag : xml.tags) {
			if (tag.name == "strong") {
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "font-weight: bold;";

				// change to a CSS style
				tag.name = "span";
				tag.attributes.clear();
				tag.attributes.push_back(tag_attribute);
				continue;
			}
			if (tag.name == "em") {
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "font-style: italic;";

				// change to a CSS style
				tag.name = "span";
				tag.attributes.clear();
				tag.attributes.push_back(tag_attribute);
				continue;
			}
			if (tag.name == "u") {
				// to-do: make it possible for this to work together with text-decoration: line-through
				continue;
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "text-decoration: underline;";

				// change to a CSS style
				tag.name = "span";
				tag.attributes.clear();
				tag.attributes.push_back(tag_attribute);
				continue;
			}
			if (tag.name == "s") {
				xml_parser::tag_attribute tag_attribute;
				tag_attribute.name = "style";
				tag_attribute.value = "text-decoration: line-through;";

				// change to a CSS style
				tag.name = "span";
				tag.attributes.clear();
				tag.attributes.push_back(tag_attribute);
				continue;
			}
		}

		std::vector<unsigned long> tag_positions;

		// check if any part of this selection is within another tag
		for (const auto& tag : xml.tags) {
			const auto a = tag.start_position;
			const auto b = a + tag.length;

			if (a >= start && b <= end)
				continue;	// fully contained within range

			if (a > start && a < end) {
				// a is a breaking point
				tag_positions.push_back(a);
			}

			if (b > start && b < end) {
				// b is a breaking point
				tag_positions.push_back(b);
			}
		}

		// sort breaking points in ascending order
		auto compare = [](unsigned long& left, unsigned long& right) {
			return left < right;
		};

		std::sort(tag_positions.begin(), tag_positions.end(), compare);

		struct portion {
			unsigned long begin = 0;
			unsigned long end = 0;
			bool skip = false;
			long tag_number = 0;
		};

		std::vector<portion> portions;

		if (tag_positions.empty()) {
			// place the tag directly to the entire range, there is nothing hindering it.
			portion p;
			p.begin = start;
			p.end = end;
			portions.push_back(p);
		}
		else {
			// break the selection into portions using the breaking points and apply the tag to
			// each portion

			unsigned long prev = start;
			for (const auto& position : tag_positions) {
				portion p;
				p.begin = prev;
				p.end = position;
				portions.push_back(p);

				prev = position;
			}
			portion p;
			p.begin = prev;
			p.end = end;
			portions.push_back(p);
		}

		// figure out portion levels
		for (auto& portion : portions) {
			for (auto& tag : xml.tags) {
				const auto a = tag.start_position;
				const auto b = a + tag.length;

				if (portion.begin >= a && portion.end <= b) {
					// portion is within this tag
					if (portion.tag_number < tag.level + 1)
						portion.tag_number = tag.level + 1;
				}
			}
		}

		for (auto& portion : portions) {
			// remove all tags of this name within this portion
			liblec::lecui::log("[" + std::to_string(portion.begin) + "-" + std::to_string(portion.end) + "]");

			std::vector<xml_parser::tag> eliminate;

			// check if there are any tags between begin and end
			for (auto& tag : xml.tags) {
				const auto a = tag.start_position;
				const auto b = a + tag.length;
				const bool equal_to_selection = a == portion.begin && b == portion.end;

				// check if any tag at lower levels is scheduled for elimination. If yes
				// lower the tag number
				auto check_to_lower_tag_number = [&]() {
					for (const auto& it : eliminate) {
						const auto x = it.start_position;
						const auto y = x + it.length;

						if (a >= x && b <= y) {
							// lower tag number
							liblec::lecui::log("Within a tag scheduled for elimination. Lower tag number.");
							tag.level--;
						}
					}
				};

				if (a >= portion.begin && b <= portion.end) {
					// if this tag is the same, eliminate it
					// if this tag is different, lower it's tag number

					liblec::lecui::log(tag.name + " within selection");

					if (equal_to_selection) {
						liblec::lecui::log(tag.name + " equal to selection");

						if (tag.name == tag_name) {
							// add to skip list ... the portion will not be tagged
							portion.skip = true;
						}
					}

					if (tag.name == tag_name) {
						if (tag.attributes.empty() && tag_attributes.empty()) {
							liblec::lecui::log("Schedule tag for elimination");
							eliminate.push_back(tag);
						}
						else {
							for (const auto& incoming_attribute : tag_attributes) {
								// check if incoming attribute exists in the existing tag attributes
								bool exists = false;
								for (auto& existing_attribute : tag.attributes) {
									if (existing_attribute.name == incoming_attribute.name) {
										exists = true;

										if (existing_attribute.name == "style") {
											// handle CSS styles
											auto existing_properties = css_parser().read(existing_attribute.value);
											auto incoming_properties = css_parser().read(incoming_attribute.value);

											for (const auto& incoming_property : incoming_properties) {
												// check if incoming property exists in the existing properties
												bool exists_ = false;

												std::vector<css_parser::css_property> eliminate_property;

												for (auto& existing_property : existing_properties) {
													if (existing_property.property == incoming_property.property) {
														exists_ = true;

														if (!equal_to_selection) {
															// schedule this property for deletion
															eliminate_property.push_back(existing_property);
															break;
														}

														/// modify the property
														
														if (existing_property.property == "font-family") {
															existing_property.value = incoming_property.value;
															break;
														}

														if (existing_property.property == "font-size") {
															existing_property.value = incoming_property.value;
															break;
														}

														if (existing_property.property == "font-weight") {
															if (existing_property.value == "bold")
																existing_property.value = "normal";
															else
																existing_property.value = "bold";

															break;
														}

														if (existing_property.property == "font-style") {
															if (existing_property.value == "italic")
																existing_property.value = "normal";
															else
																existing_property.value = "italic";

															break;
														}

														if (existing_property.property == "text-decoration") {
															if (incoming_property.value == "underline") {
																if (existing_property.value == "underline")
																	existing_property.value = "none";
																else
																	existing_property.value = "underline";

																break;
															}

															if (incoming_property.value == "line-through") {
																if (existing_property.value == "line-through")
																	existing_property.value = "none";
																else
																	existing_property.value = "line-through";

																break;
															}
														}

														if (existing_property.property == "color") {
															existing_property.value = incoming_property.value;
															break;
														}

														break;
													}
												}

												// do property eliminations
												if (!eliminate_property.empty()) {
													std::vector<css_parser::css_property> properties_new;
													for (auto& property : existing_properties) {
														bool skip = false;

														for (auto& property_to_skip : eliminate_property) {
															if (property.property == property_to_skip.property) {
																skip = true;
																break;
															}
														}

														if (!skip)
															properties_new.push_back(property);
													}
													existing_properties = properties_new;
												}

												if (!exists_ && equal_to_selection) {
													// add the property
													existing_properties.push_back(incoming_property);
												}

												// write the new CSS properties into the attribute value
												existing_attribute.value = css_parser().write(existing_properties);
											}
										}

										break;
									}
								}

								if (!exists && equal_to_selection) {
									// add the tag attribute
									tag.attributes.push_back(incoming_attribute);
								}
							}

							check_to_lower_tag_number();

							// cleanup tag
							long count = 0;
							for (auto& attribute : tag.attributes) {
								if (!attribute.value.empty())
									count++;
							}

							if (!count) {
								liblec::lecui::log("Tag now empty");
								liblec::lecui::log("Schedule tag for elimination");
								eliminate.push_back(tag);
							}
						}
					}
					else
						check_to_lower_tag_number();
				}
			}

			// do eliminations
			if (!eliminate.empty()) {
				std::vector<xml_parser::tag> tags_new;
				for (auto& tag : xml.tags) {
					bool skip = false;

					for (auto& tag_to_skip : eliminate) {
						if (tag == tag_to_skip) {
							skip = true;
							break;
						}
					}

					if (!skip)
						tags_new.push_back(tag);
				}
				xml.tags = tags_new;
			}

			// write back
			liblec::lecui::log("Before: " + xml_text);
			xml_text = xml_parser().write(xml);
			liblec::lecui::log("After : " + xml_text);
		}

		// reverse the portions vector
		std::reverse(portions.begin(), portions.end());

		auto insert_tag = [](const std::string s, unsigned long position,
			long tag_number, std::string& xml_text)->void {
			try {
				// skip all html tags and find the position we need.
				size_t characters = 0;
				size_t tag_characters = 0;

				auto insert = [&]() {
					// we have found the position ... do the insertion
					const auto xml_position = position + tag_characters;
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
							// is this a closing tag or an opening tag
							if (xml_text[open + 1] == '/') {
								// this is a closing tag ... a decrement has already been done for the
								// opening tag that matches it ... cancel out it's effect by increasing
								tag_number++;
							}
							else {
								// this is an opening tag ... advance
								tag_number--;
							}

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
		};

		for (const auto& portion : portions) {
			if (portion.skip)
				continue;

			// place the tag into this portion
			liblec::lecui::log("[" + std::to_string(portion.begin) + "-" + std::to_string(portion.end) + "]");

			// insert tags to xml text
			auto start_tag = tag_name;

			if (!tag_attributes.empty()) {
				// add attribute string
				std::string attribute_string;

				for (const auto& attribute : tag_attributes)
					attribute_string += " " + attribute.name + "='" + attribute.value + "'";

				start_tag += attribute_string;
			}

			start_tag = "<" + start_tag + ">";
			auto end_tag = "</" + tag_name + ">";

			insert_tag(end_tag, portion.end, portion.tag_number, xml_text);
			insert_tag(start_tag, portion.begin, portion.tag_number, xml_text);
		}

		liblec::lecui::log("After : " + xml_text);
	}
	catch (const std::exception&) {}
}

void formatted_text_editor::toggle_tag(std::string& xml_text, const std::string& tag_name,
	unsigned long start, unsigned long end) {
	if (start == end)
		return;	// do nothing

	std::vector<xml_parser::tag_attribute> tag_attributes;	// basic tags have no attributes
	toggle_tag(xml_text, tag_name, tag_attributes, start, end);
}
