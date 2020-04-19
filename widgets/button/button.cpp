/*
** button.cpp - button widget implementation
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

#include "../button.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::button::button_specs::operator==(const button_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border);
		}

		bool widgets::button::button_specs::operator!=(const button_specs& param) {
			return !operator==(param);
		}

		class widgets::button::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_button(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::button);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::button_border);
				specs_.color_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::button_hover);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::button_selected);
			}
			containers::page& page_;
			button_specs& specs_;
		};

		widgets::button::button(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::button::~button() { delete& d_; }

		widgets::button::button_specs&
			widgets::button::specs() {
			return d_.specs_;
		}

		widgets::button::button_specs&
			widgets::button::operator()() {
			return specs();
		}

		widgets::button::button_specs&
			widgets::button::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_button(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_button(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
