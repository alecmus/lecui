//
// form_menu.cpp - form menu implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../form_menu.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class form_menu::form_menu::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		form_menu::form_menu(form& fm) : d_(*(new impl(fm))) {}
		form_menu::~form_menu() { delete& d_; }

		bool form_menu::add(const std::string& text,
			const std::vector<form_menu_item>& items,
			std::string& error) {
			for (const auto& it : d_.fm_.d_.form_menu_) {
				if (it.text == text) {
					error = "Menu item already exists";
					return false;
				}
			}

			d_.fm_.d_.form_menu_.push_back({ text, items });
			return true;
		}
	}
}
