/*
** image_converter.h - image converter interface
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

#include <Windows.h>
#include <string>

class image_converter {
public:
	/// <summary>Convert a PNG resource to an HBITMAP with an alpha channel.</summary>
	/// <param name="h_module">The handle of the module containing the PNG resource.</param>
	/// <param name="id_png">The ID of the PNG image.</param>
	/// <param name="error">Error information.</param>
	/// <returns>An ARGB HBITMAP, else returns null when not successful (in which case error
	/// information will be written to the 'error' parameter.</returns>
	HBITMAP png_to_argb(HMODULE h_module,
		int id_png,
		std::string& error);
};
