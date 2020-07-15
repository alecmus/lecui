/*
** date.cpp - date widget implementation
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

#include "../date.h"
#include "../../form_impl/form_impl.h"
#include "../../utilities/date_time.h"

namespace liblec {
	namespace lecui {
		bool widgets::date::date_specs::operator==(const date_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				true;
		}

		bool widgets::date::date_specs::operator!=(const date_specs& param) {
			return !operator==(param);
		}

		class widgets::date::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_date(alias)) {
				specs_.date_value = date_time::today();
			}
			containers::page& page_;
			date_specs& specs_;
		};

		widgets::date::date(containers::page& page) :
			date(page, "") {}

		widgets::date::date(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::date::~date() { delete& d_; }

		widgets::date::date_specs&
			widgets::date::specs() {
			return d_.specs_;
		}

		widgets::date::date_specs&
			widgets::date::operator()() {
			return specs();
		}

		widgets::date::date_specs&
			widgets::date::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_date(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_date(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
