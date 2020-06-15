/*
** font.h - font interface
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
#include <vector>

/// <summary>Font handling class.</summary>
class font {
public:
	/// <summary>Get list of font's that are available to the app.</summary>
	static std::vector<std::string> available_fonts();
};
