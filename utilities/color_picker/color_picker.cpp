/*
** color_picker.cpp - color picker implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "../color_picker.h"
#include "../../form_impl/form_impl.h"

#include <Windows.h>

namespace liblec {
	namespace lecui {
		class color_picker::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		color_picker::color_picker(form& fm) :
			d_(*(new impl(fm))) {}

		color_picker::~color_picker() { delete& d_; }

		bool color_picker::pick(color& picked_color) {
			COLORREF crCustColor[] = {
				RGB(0,     5,   5),
				RGB(0,    15,  55),
				RGB(0,    25, 155),
				RGB(0,    35, 255),
				RGB(10,    0,   5),
				RGB(10,   20,  55),
				RGB(10,   40, 155),
				RGB(10,   60, 255),
				RGB(100,   5,   5),
				RGB(100,  25,  55),
				RGB(100,  50, 155),
				RGB(100, 125, 255),
				RGB(200, 120,   5),
				RGB(200, 150,  55),
				RGB(200, 200, 155),
				RGB(200, 250, 255)
			};

			CHOOSECOLOR cc;
			cc.lStructSize = sizeof(CHOOSECOLOR);
			cc.hwndOwner = d_.fm_.d_.hWnd_;
			cc.hInstance = NULL;
			cc.rgbResult = RGB(0x80, 0x80, 0x80);
			cc.lpCustColors = crCustColor;
			cc.Flags = CC_RGBINIT/* | CC_FULLOPEN*/;
			cc.lCustData = 0;
			cc.lpfnHook = NULL;
			cc.lpTemplateName = NULL;

			if (ChooseColor(&cc)) {
				picked_color.red = static_cast<unsigned short>(GetRValue(cc.rgbResult));
				picked_color.green = static_cast<unsigned short>(GetGValue(cc.rgbResult));
				picked_color.blue = static_cast<unsigned short>(GetBValue(cc.rgbResult));
				picked_color.alpha = 255;
				return true;
			}
			else
				return false;
		}
	}
}
