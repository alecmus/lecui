//
// toggle.cpp - toggle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../toggle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::toggle::toggle_specs::operator==(const toggle_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_on == param.color_on) &&
				(color_off == param.color_off);
		}

		bool widgets::toggle::toggle_specs::operator!=(const toggle_specs& param) {
			return !operator==(param);
		}

		class widgets::toggle::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_toggle(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle);
				specs_.color_on = defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_on);
				specs_.color_off = defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_off);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::toggle_selected);
			}
			containers::page& page_;
			toggle_specs& specs_;
		};

		widgets::toggle::toggle(containers::page& page) :
			toggle(page, "") {}

		widgets::toggle::toggle(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::toggle::~toggle() { delete& d_; }

		widgets::toggle::toggle_specs&
			widgets::toggle::specs() {
			return d_.specs_;
		}

		widgets::toggle::toggle_specs&
			widgets::toggle::operator()() {
			return specs();
		}

		widgets::toggle::toggle_specs&
			widgets::toggle::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
