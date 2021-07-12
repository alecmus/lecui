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
				_fm(fm) {}
			form& _fm;
		};

		form_menu::form_menu(form& fm) : _d(*(new impl(fm))) {}
		form_menu::~form_menu() { delete& _d; }

		bool form_menu::add(const std::string& text,
			const std::string& tooltip,
			const std::vector<form_menu_item>& items,
			std::string& error) {
			for (const auto& it : _d._fm._d._form_menu) {
				if (it.text == text) {
					error = "Menu item already exists";
					return false;
				}
			}

			_d._fm._d._form_menu.push_back({ text, tooltip, items });
			return true;
		}
	}
}
