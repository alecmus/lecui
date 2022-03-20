//
// on_dropfiles.cpp - drop files implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		void form::impl::on_dropfiles(WPARAM wParam) {
			if (_fm.events().drop_files != nullptr) {
				// a file has been dropped onto the form
				CHAR file[MAX_PATH];
				ZeroMemory(file, sizeof(file));

				HDROP hDrp = (HDROP)wParam;
				DragQueryFileA(hDrp, 0, file, sizeof(file));
				DragFinish(hDrp);

				const std::string fullpath = file;
				_fm.events().drop_files(fullpath);
			}
		}
	}
}
