//
// checkbox.cpp - checkbox widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../checkbox.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::checkbox_specs::operator==(const checkbox_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_check_ == param.color_check_);
		}

		bool widgets::checkbox_specs::operator!=(const checkbox_specs& param) {
			return !operator==(param);
		}

		class widgets::checkbox_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_checkbox(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_border))
					.color_check(defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_tick))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::checkbox_selected));
			}
			containers::page& page_;
			checkbox_specs& specs_;
		};

		widgets::checkbox_builder::checkbox_builder(containers::page& page) :
			checkbox_builder(page, "") {}

		widgets::checkbox_builder::checkbox_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::checkbox_builder::~checkbox_builder() { delete& d_; }

		widgets::checkbox_specs&
			widgets::checkbox_builder::specs() {
			return d_.specs_;
		}

		widgets::checkbox_specs&
			widgets::checkbox_builder::operator()() {
			return specs();
		}

		widgets::checkbox_specs&
			widgets::checkbox_builder::specs(form& fm, const std::string& path) {
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
