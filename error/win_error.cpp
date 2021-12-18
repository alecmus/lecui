//
// win_error.cpp - windows error implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "win_error.h"
#include <Windows.h>

std::string get_last_error() {
	std::string last_error;

	CHAR buffer[256] = "?";

	const DWORD dw_last_error = GetLastError();

	if (dw_last_error != 0) {
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,		// It's a system error
			NULL,										// No string to be formatted needed
			dw_last_error,								// Hey Windows: Please explain this error!
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// Do it in the standard language
			buffer,										// Put the message here
			256 - 1,									// Number of bytes to store the message
			NULL);

		last_error = buffer;
	}

	return last_error;
}
