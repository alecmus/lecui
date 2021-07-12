//
// tray_icon.cpp - tray icon implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../tray_icon.h"
#include "../../form_impl/form_impl.h"
#include <Windows.h>

namespace liblec {
	namespace lecui {
		class tray_icon::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;

			const unsigned int _tray_icon_id = 1;
		};

		tray_icon::tray_icon(form& fm) : _d(*new impl(fm)) {}
		tray_icon::~tray_icon() {
			remove();
			delete& _d;
		}

		bool tray_icon::add(int png_resource, const std::string& title,
			const std::vector<tray_menu_item>& items,
			const std::string& default_item, std::string& error) {
			if (!png_resource) {
				error = "PNG resource for tray icon not specified";
				return false;
			}

			// do not allow more than one tray icon per form
			if (_d._fm._d._tray_icon_present)
				return true;

			_d._fm._d._tray_icon_menu_items = items;

			// parse the default item text
			// the default color doesn't matter here we're just getting the plain text
			std::vector<formatted_text_parser::text_range_properties> formatting;
			widgets::parse_formatted_text(default_item, _d._fm._d._tray_item_default,
				D2D1::ColorF(D2D1::ColorF::Black), formatting);

			// create system tray icon
			NOTIFYICONDATAA nid;
			nid.hWnd = _d._fm._d._hWnd;
			nid.uID = _d._tray_icon_id;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_APP;
			nid.hIcon = (HICON)LoadImageA(
				_d._fm._d._resource_module_handle,
				MAKEINTRESOURCEA(png_resource),
				IMAGE_ICON,
				GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
				LR_DEFAULTCOLOR);
			lstrcpyA(nid.szTip, title.c_str());

			// add the system tray icon
			Shell_NotifyIconA(NIM_ADD, &nid);
			_d._fm._d._tray_icon_present = true;
			return true;
		}

		bool tray_icon::change(int png_resource,
			const std::string& title, std::string& error) {
			if (!png_resource) {
				error = "PNG resource for tray icon not specified";
				return false;
			}

			if (!_d._fm._d._tray_icon_present) {
				error = "No tray icon present";
				return true;
			}

			// create system tray icon
			NOTIFYICONDATAA nid;
			nid.hWnd = _d._fm._d._hWnd;
			nid.uID = _d._tray_icon_id;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_APP;
			nid.hIcon = (HICON)LoadImageA(
				_d._fm._d._resource_module_handle,
				MAKEINTRESOURCEA(png_resource),
				IMAGE_ICON,
				GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
				LR_DEFAULTCOLOR);
			lstrcpyA(nid.szTip, title.c_str());

			// modify the tray icon
			Shell_NotifyIconA(NIM_MODIFY, &nid);
			return true;
		}

		void tray_icon::remove() {
			if (_d._fm._d._tray_icon_present) {
				NOTIFYICONDATA  nid;
				nid.hWnd = _d._fm._d._hWnd;
				nid.uID = _d._tray_icon_id;
				Shell_NotifyIcon(NIM_DELETE, &nid);
			}
		}
	}
}
