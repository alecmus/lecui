//
// win_error.h - windows error interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include <string>

/// <summary>
/// Get Windows last error as a string.
/// </summary>
std::string get_last_error();
