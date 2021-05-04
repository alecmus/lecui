/*
** font.cpp - font implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "font.h"
#include <Windows.h>

static BOOL CALLBACK enumerate_font_callback(LPLOGFONTA lplf,
	LPNEWTEXTMETRICA lpntm, DWORD FontType, LPVOID pVoid) {
	UNREFERENCED_PARAMETER(lplf);
	UNREFERENCED_PARAMETER(lpntm);

	/// Windows lists fonts which have a vmtx (vertical metrics) table twice.
	/// Once using their normal name, and again preceded by '@'. These appear
	/// in font lists in some windows apps, such as wordpad. We don't want
	/// these so we skip any font where the first character is '@'
	if (lplf->lfFaceName[0] == '@')
		return TRUE;

	std::vector<std::string>* p_font_names = (std::vector<std::string>*) pVoid;

	// capture font names
	p_font_names->push_back(lplf->lfFaceName);

	return TRUE;
}

std::vector<std::string> font::available_fonts() {
	HDC hdc = GetDC(GetDesktopWindow());

	std::vector<std::string> font_names;
	EnumFontFamiliesA(hdc, (LPCSTR)NULL,
		(FONTENUMPROCA)enumerate_font_callback, (LPARAM)&font_names);

	ReleaseDC(GetDesktopWindow(), hdc);

	return font_names;
}
