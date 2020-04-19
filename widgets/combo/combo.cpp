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
		bool widgets::combo::combo_specs::operator==(const combo_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_dropdown_hot == param.color_dropdown_hot) &&
				(color_menu == param.color_menu) &&
				(color_menu_hot == param.color_menu_hot) &&
				(color_menu_selected == param.color_menu_selected);
		}

		bool widgets::combo::combo_specs::operator!=(const combo_specs& param) {
			return !operator==(param);
		}

		class widgets::combo::combo_impl {
		public:
			combo_impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_combo(alias)) {}
			containers::page& page_;
			combo_specs& specs_;
		};

		widgets::combo::combo(containers::page& page, const std::string& alias) :
			d_(*(new combo_impl(page, alias))) {}

		widgets::combo::~combo() { delete& d_; }

		widgets::combo::combo_specs&
			widgets::combo::specs() {
			return d_.specs_;
		}

		widgets::combo::combo_specs&
			widgets::combo::operator()() {
			return specs();
		}

		widgets::combo::combo_specs&
			widgets::combo::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_combo(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_combo(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
