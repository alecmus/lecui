//
// progress_bar.cpp - progress bar widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../progress_bar.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::progress_bar_specs::operator==(const progress_bar_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::progress_bar_specs::operator!=(const progress_bar_specs& param) {
			return !operator==(param);
		}

		class widgets::progress_bar_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_progress_bar(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar_border));
			}
			containers::page& page_;
			progress_bar_specs& specs_;
		};

		widgets::progress_bar_builder::progress_bar_builder(containers::page& page) :
			progress_bar_builder(page, "") {}

		widgets::progress_bar_builder::progress_bar_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::progress_bar_builder::~progress_bar_builder() { delete& d_; }

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::specs() {
			return d_.specs_;
		}

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::operator()() {
			return specs();
		}

		widgets::progress_bar_specs&
			widgets::progress_bar_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_progress_bar(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_progress_bar(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
