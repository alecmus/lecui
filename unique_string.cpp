//
// unique_string.cpp - unique string implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

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
