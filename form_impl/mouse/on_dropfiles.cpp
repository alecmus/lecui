//
// on_dropfiles.cpp - drop files implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		void form::impl::on_dropfiles(WPARAM wParam) {
			if (on_drop_files_ != nullptr) {
				// a file has been dropped onto the form
				CHAR file[MAX_PATH];
				ZeroMemory(file, sizeof(file));

				HDROP hDrp = (HDROP)wParam;
				DragQueryFileA(hDrp, 0, file, sizeof(file));
				DragFinish(hDrp);

				const std::string fullpath = file;
				on_drop_files_(fullpath);
			}
		}
	}
}
