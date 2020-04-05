/*
** lecui.cpp - lecui implementation
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

#include "lecui.h"
#include "versioninfo.h"
#include "form_common.h"

#include <Windows.h>

// DllMain function
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	default:
		break;
	}
	return TRUE;
}

std::string liblec::lecui::version() {
	return lecuiname + std::string(" ") + lecuiversion + std::string(" ") + lecuidate;
}

void liblec::lecui::rect::place(const rect& rect_reference,
	const float& perc_h, const float& perc_v) {
	pos_rect(rect_reference, *this, perc_h, perc_v);
}

bool liblec::lecui::rect::operator==(const rect& param) {
	return
		(left == param.left) &&
		(right == param.right) &&
		(top == param.top) &&
		(bottom == param.bottom);
}

bool liblec::lecui::rect::operator!=(const rect& param) {
	return !operator==(param);
}

bool liblec::lecui::color::operator==(const color& param) {
	return
		(red == param.red) &&
		(green == param.green) &&
		(blue == param.blue) &&
		(alpha == param.alpha);
}

bool liblec::lecui::color::operator!=(const color& param) {
	return !operator==(param);
}

bool liblec::lecui::list_column::operator==(const list_column& param) {
	return
		(name == param.name) &&
		(width == param.width);
}

bool liblec::lecui::list_column::operator!=(const list_column& param) {
	return !operator==(param);
}
