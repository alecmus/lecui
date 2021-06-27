//
// custom.cpp - custom widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../custom.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::custom_specs::operator==(const custom_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				true;
		}

		bool widgets::custom_specs::operator!=(const custom_specs& param) {
			return !operator==(param);
		}

		class widgets::custom_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_custom(alias)) {}
			containers::page& page_;
			custom_specs& specs_;
		};

		widgets::custom_builder::custom_builder(containers::page& page) :
			custom_builder(page, "") {}

		widgets::custom_builder::custom_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::custom_builder::~custom_builder() { delete& d_; }

		widgets::custom_specs&
			widgets::custom_builder::specs() {
			return d_.specs_;
		}

		widgets::custom_specs&
			widgets::custom_builder::operator()() {
			return specs();
		}

		widgets::custom_specs&
			widgets::custom_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_custom(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_custom(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
