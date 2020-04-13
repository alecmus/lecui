/*
** combo.cpp - combo widget implementation
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

#include "../combo.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::specs::combo::operator==(const combo& param) {
			return
				// generic specs
				widget::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_dropdown_hot == param.color_dropdown_hot) &&
				(color_menu == param.color_menu) &&
				(color_menu_hot == param.color_menu_hot) &&
				(color_menu_selected == param.color_menu_selected);
		}

		bool widgets::specs::combo::operator!=(const combo& param) {
			return !operator==(param);
		}

		class widgets::combo::combo_impl {
		public:
			combo_impl(containers::page& page) :
				page_(page) {}
			containers::page& page_;
		};

		widgets::combo::combo(containers::page& page) :
			d_(*(new combo_impl(page))) {}

		widgets::combo::~combo() { delete& d_; }

		widgets::specs::combo&
			widgets::combo::add(const std::string& alias) {
			return d_.page_.d_page_.add_combo(alias);
		}

		widgets::specs::combo&
			widgets::combo::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_combo(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
