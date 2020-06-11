/*
** custom.cpp - custom widget implementation
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

#include "../custom.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::custom::custom_specs::operator==(const custom_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				true;
		}

		bool widgets::custom::custom_specs::operator!=(const custom_specs& param) {
			return !operator==(param);
		}

		class widgets::custom::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_custom(alias)) {}
			containers::page& page_;
			custom_specs& specs_;
		};

		widgets::custom::custom(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::custom::~custom() { delete& d_; }

		widgets::custom::custom_specs&
			widgets::custom::specs() {
			return d_.specs_;
		}

		widgets::custom::custom_specs&
			widgets::custom::operator()() {
			return specs();
		}

		widgets::custom::custom_specs&
			widgets::custom::specs(form& fm, const std::string& path) {
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
