//
// xml_parser.h - xml parser interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include <string>
#include <vector>

/// <summary>A custom XML parser that describes the XML file based on positions of tags within
/// a plaintext canvas.</summary>
/// <remarks>This parser is specifically designed for use with text rendering. Use only XML text
/// that does not have whitespace between tags.</remarks>
class xml_parser {
public:
	/// <summary>Structure that describes an XML tag attribute.</summary>
	struct tag_attribute {
		/// <summary>The name of the attribute.</summary>
		std::string name;

		/// <summary>The value of the attribute.</summary>
		std::string value;
	};

	/// <summary>Structure that defines an XML tag relative to a plaintext canvas.</summary>
	struct tag {
		/// <summary>The level of the tag. Outermost tags are level 0.</summary>
		long level = 0;

		/// <summary>The name of the tag.</summary>
		std::string name;

		/// <summary>The contents of the tag.</summary>
		std::string text;

		/// <summary>The position within the plaintext canvas at which the tag begins.</summary>
		unsigned long start_position = 0;

		/// <summary>The number of text positions within the tag.</summary>
		unsigned long length = 0;

		/// <summary>The tag's attributes</summary>
		std::vector<tag_attribute> attributes;

		bool operator== (tag& t) {
			if (level == t.level &&
				name == t.name &&
				text == t.text &&
				start_position == t.start_position &&
				length == t.length)
				return true;
			else
				return false;
		};

		bool operator!= (tag& t) {
			return !operator==(t);
		}
	};

	/// <summary>A structure that describes XML using positions of tags within a plaintext
	/// canvas.</summary>
	struct xml {
		/// <summary>The plaintext.</summary>
		std::string plain_text;

		/// <summary>The XML tags.</summary>
		std::vector<tag> tags;
	};

	/// <summary>Read XML text into a xml_parser::xml structure.</summary>
	/// <param name="xml_text">XML text.</param>
	/// <returns>Returns a description of the XML text using a xml_parser::xml data structure.
	/// </returns>
	xml read(std::string xml_text);

	/// <summary>Write data from a xml_parser::xml structure to an XML string.</summary>
	/// <param name="info">The description of the XML using the data structure.</param>
	/// <returns>An XML text string.</returns>
	std::string write(const xml& info);
};
