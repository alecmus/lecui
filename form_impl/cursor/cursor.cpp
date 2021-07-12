//
// cursor.cpp - cursor implementation
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
		D2D1_POINT_2F form::impl::get_cursor_position() {
			POINT pt = { 0 };
			GetCursorPos(&pt);
			ScreenToClient(_hWnd, &pt);
			return { static_cast<float>(pt.x), static_cast<float>(pt.y) };
		}

		D2D1_SIZE_F form::impl::get_cursor_size() {
			// get the size of the opaque area of bitmap
			auto get_size_of_opaque = [](HBITMAP hbmSrc) {
				SIZE size;
				size = { 0 };

				HDC hdc_src, hdc_dst;
				HBITMAP hbm_old;

				if ((hdc_src = CreateCompatibleDC(NULL)) != NULL) {
					if ((hdc_dst = CreateCompatibleDC(NULL)) != NULL) {
						int row, column;

						BITMAP bm;
						GetObject(hbmSrc, sizeof(bm), &bm);
						hbm_old = (HBITMAP)SelectObject(hdc_src, hbmSrc);

						BitBlt(hdc_dst, 0, 0, bm.bmWidth, bm.bmHeight, hdc_src, 0, 0, SRCCOPY);

						// loop through every pixel
						for (row = 0; row < bm.bmHeight; row++) {
							for (column = 0; column < bm.bmWidth; column++) {
								// get color of pixel
								// this is a layer mask so
								// black pixel means opaque
								// white pixel means transparent
								COLORREF clrTP = GetPixel(hdc_src, column, row);

								DWORD r = GetRValue(clrTP);
								DWORD g = GetGValue(clrTP);
								DWORD b = GetBValue(clrTP);

								if (clrTP == RGB(0, 0, 0)) {
									// black (opaque)
									if (row > size.cy)
										size.cy = row;

									if (column > size.cx)
										size.cx = column;
								}
							}
						}

						DeleteDC(hdc_dst);
					}

					DeleteDC(hdc_src);
				}

				return size;
			};

			SIZE size = { 0 };

			CURSORINFO ci = { 0 };
			ci.cbSize = sizeof(CURSORINFO);
			GetCursorInfo(&ci);

			ICONINFO info = { 0 };
			if (GetIconInfo(ci.hCursor, &info)) {
				bool bw_cursor = (info.hbmColor == NULL);
				BITMAP bmpinfo = { 0 };


				if (GetObject(info.hbmMask, sizeof(BITMAP), &bmpinfo) != 0) {
					// size of ENTIRE cursor (this includes a whole transparent area!)
					size.cx = bmpinfo.bmWidth;
					size.cy = abs(bmpinfo.bmHeight) / (bw_cursor ? 2 : 1);
				}

				// get the size of ONLY the opaque area of the cursor (clip out the transparent area)
				size = get_size_of_opaque(info.hbmMask);

				DeleteObject(info.hbmColor);
				DeleteObject(info.hbmMask);
			}

			return { static_cast<float>(size.cx), static_cast<float>(size.cy) };
		}
	}
}
