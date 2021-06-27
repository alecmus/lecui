//
// time.cpp - time widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../time.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::time_specs::operator==(const time_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::time_specs::operator!=(const time_specs& param) {
			return !operator==(param);
		}

		class widgets::time_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_time(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_border))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_disabled))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_selected));
			}
			containers::page& page_;
			time_specs& specs_;
		};

		widgets::time_builder::time_builder(containers::page& page) :
			time_builder(page, "") {}

		widgets::time_builder::time_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::time_builder::~time_builder() { delete& d_; }

		widgets::time_specs&
			widgets::time_builder::specs() {
			return d_.specs_;
		}

		widgets::time_specs&
			widgets::time_builder::operator()() {
			return specs();
		}

		widgets::time_specs&
			widgets::time_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_time(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_time(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
