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
				fm_(fm) {}
			form& fm_;

			const unsigned int tray_icon_id_ = 1;
		};

		tray_icon::tray_icon(form& fm) : d_(*new impl(fm)) {}
		tray_icon::~tray_icon() {
			remove();
			delete& d_;
		}

		bool tray_icon::add(int png_resource, const std::string& title,
			const std::vector<tray_menu_item>& items,
			const std::string& default_item, std::string& error) {
			if (!png_resource) {
				error = "PNG resource for tray icon not specified";
				return false;
			}

			// do not allow more than one tray icon per form
			if (d_.fm_.d_.tray_icon_present_)
				return true;

			d_.fm_.d_.tray_icon_menu_items_ = items;

			// parse the default item text
			// the default color doesn't matter here we're just getting the plain text
			std::vector<formatted_text_parser::text_range_properties> formatting;
			widgets::parse_formatted_text(default_item, d_.fm_.d_.tray_item_default_,
				D2D1::ColorF(D2D1::ColorF::Black), formatting);

			// create system tray icon
			NOTIFYICONDATAA nid;
			nid.hWnd = d_.fm_.d_.hWnd_;
			nid.uID = d_.tray_icon_id_;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_APP;
			nid.hIcon = (HICON)LoadImageA(
				d_.fm_.d_.resource_module_handle_,
				MAKEINTRESOURCEA(png_resource),
				IMAGE_ICON,
				GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
				LR_DEFAULTCOLOR);
			lstrcpyA(nid.szTip, title.c_str());

			// add the system tray icon
			Shell_NotifyIconA(NIM_ADD, &nid);
			d_.fm_.d_.tray_icon_present_ = true;
			return true;
		}

		bool tray_icon::change(int png_resource,
			const std::string& title, std::string& error) {
			if (!png_resource) {
				error = "PNG resource for tray icon not specified";
				return false;
			}

			if (!d_.fm_.d_.tray_icon_present_) {
				error = "No tray icon present";
				return true;
			}

			// create system tray icon
			NOTIFYICONDATAA nid;
			nid.hWnd = d_.fm_.d_.hWnd_;
			nid.uID = d_.tray_icon_id_;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_APP;
			nid.hIcon = (HICON)LoadImageA(
				d_.fm_.d_.resource_module_handle_,
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
			if (d_.fm_.d_.tray_icon_present_) {
				NOTIFYICONDATA  nid;
				nid.hWnd = d_.fm_.d_.hWnd_;
				nid.uID = d_.tray_icon_id_;
				Shell_NotifyIcon(NIM_DELETE, &nid);
			}
		}
	}
}
