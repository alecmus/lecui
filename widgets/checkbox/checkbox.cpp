/*
** checkbox.cpp - checkbox widget implementation
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

#include "../checkbox.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::checkbox::checkbox_specs::operator==(const checkbox_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_check == param.color_check);
		}

		bool widgets::checkbox::checkbox_specs::operator!=(const checkbox_specs& param) {
			return !operator==(param);
		}

		class widgets::checkbox::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_checkbox(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_border);
				specs_.color_check = defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_tick);
				specs_.color_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_hover);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_selected);
			}
			containers::page& page_;
			checkbox_specs& specs_;
		};

		widgets::checkbox::checkbox(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::checkbox::~checkbox() { delete& d_; }

		widgets::checkbox::checkbox_specs&
			widgets::checkbox::specs() {
			return d_.specs_;
		}

		widgets::checkbox::checkbox_specs&
			widgets::checkbox::operator()() {
			return specs();
		}

		widgets::checkbox::checkbox_specs&
			widgets::checkbox::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_checkbox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_checkbox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
