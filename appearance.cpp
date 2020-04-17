/*
** appearance.cpp - appearance implementation
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

#include "appearance.h"
#include "form_impl.h"

namespace liblec {
	namespace lecui {
		class appearance::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		appearance::appearance(form& fm) :
			d_(*(new impl(fm))) {}

		appearance::~appearance() { delete& d_; }

		void appearance::shadow(const bool& enable) {
			if (IsWindow(d_.fm_.d_.hWnd_))
				d_.fm_.d_.set_borderless_shadow(d_.fm_.d_.hWnd_, enable);
			else
				d_.fm_.d_.borderless_shadow_ = enable;
		}

		void appearance::dark_theme(const bool& enable) {
			d_.fm_.d_.dark_theme_ = enable;

			if (enable) {
				// change the background color
				background(color{ 40, 45, 55 }, color{ 40, 45, 55 });

				// change the theme colors
				theme(color{ 60, 190, 175 }, color{ 255, 255, 255, 255 },
					color{ 30, 30, 30, 255 }, color{ 155, 165, 180, 255 });
			}
		}

		void appearance::theme(const lecui::color& color,
			const lecui::color& color_hot,
			const lecui::color& color_disabled,
			const lecui::color& color_text) {
			d_.fm_.d_.clr_theme_ = color;
			d_.fm_.d_.clr_theme_hot_ = color_hot;
			d_.fm_.d_.clr_theme_disabled_ = color_disabled;
			d_.fm_.d_.clr_theme_text_ = color_text;
		}

		void appearance::background(const lecui::color& color,
			const lecui::color& color_titlebar) {
			d_.fm_.d_.clr_background_ = color;
			d_.fm_.d_.clr_titlebar_background_ = color_titlebar;
		}
	}
}
