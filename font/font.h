//
// font.h - font interface
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

/// <summary>Font handling class.</summary>
class font {
public:
	/// <summary>Get list of font's that are available to the app.</summary>
	static std::vector<std::string> available_fonts();
};
