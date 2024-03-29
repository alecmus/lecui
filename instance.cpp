//
// instance.cpp - instance implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "instance.h"
#include "form_impl/form_impl.h"
#include "error/win_error.h"

namespace liblec {
	namespace lecui {
		class instance_manager::impl {
		public:
			impl(form& fm, const std::string& guid) :
				_fm(fm) {
				if (!guid.empty()) {
					// capture form guid
					if (fm._d._guid.empty())
						fm._d._guid = guid;

					// for limiting the number of instances of the app
					if (!fm._d._p_instance)
						fm._d._p_instance = new limit_single_instance(guid);
				}
			}
			form& _fm;
		};

		instance_manager::instance_manager(form& fm, const std::string& guid) : _d(*(new impl(fm, guid))) {}
		instance_manager::~instance_manager() { delete& _d; }

		bool instance_manager::send_data(const std::string& guid, const std::string& data,
			const long& timeout_milliseconds, std::string& error) {
			
			// find the form's native handle
			auto hWnd = _d._fm._d.find_native_handle(guid);

			if (hWnd == nullptr) {
				error = "Instance not found";
				return false;
			}
			else {
				// pass the commandline to the existing instance
				LPSTR szData = (LPSTR)data.c_str();

				COPYDATASTRUCT cds;
				cds.cbData = static_cast<DWORD>(strlen(szData) + 1);
				cds.lpData = szData;

				DWORD_PTR result = 0;
				if (SendMessageTimeout(hWnd,
					WM_COPYDATA,	// message
					0,				// WPARAM
					(LPARAM)&cds,	// LPARAM
					SMTO_BLOCK |
					SMTO_ABORTIFHUNG,
					static_cast<UINT>(timeout_milliseconds),
					&result) == 0) {
					error = "Timeout while sending data";
					return false;
				}
				else {
					switch (result) {
					case form::impl::instance_messages::busy:
						error = "Previously sent data is still being processed by the target instance";
						return false;
					case form::impl::instance_messages::no_handler:
						error = "Target instance has no handler for the sent data";
						return false;
					case form::impl::instance_messages::handled:
					default:
						break;
					}
				}

				return true;
			}
		}
	}
}
