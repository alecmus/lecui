/*
** unique_string.cpp - implementation
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

// for UuidToString, RpcStringFree
#include <rpc.h>
#pragma comment(lib, "rpcrt4.lib")

namespace liblec {
	namespace lecui {
		std::string unique_string() {
			std::string unique;

			UUID uuid;
			UuidCreate(&uuid);
			RPC_CSTR uuidStr;
			RPC_STATUS status = UuidToStringA(&uuid, &uuidStr);
			unique += reinterpret_cast<char*>(uuidStr);
			RpcStringFreeA(&uuidStr);

			return unique;
		}

		std::string unique_string_short() {
			std::string unique = unique_string();
			const auto idx = unique.find('-');

			if (idx != std::string::npos)
				return unique.substr(0, idx);
			else
				return unique;
		}
	}
}
