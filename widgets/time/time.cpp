/*
** time.cpp - time widget implementation
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

#include "../time.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::time::time_specs::operator==(const time_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border);
		}

		bool widgets::time::time_specs::operator!=(const time_specs& param) {
			return !operator==(param);
		}

		class widgets::time::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_time(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_border);
				specs_.color_disabled = defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_disabled);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_selected);
			}
			containers::page& page_;
			time_specs& specs_;
		};

		widgets::time::time(containers::page& page) :
			time(page, "") {}

		widgets::time::time(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::time::~time() { delete& d_; }

		widgets::time::time_specs&
			widgets::time::specs() {
			return d_.specs_;
		}

		widgets::time::time_specs&
			widgets::time::operator()() {
			return specs();
		}

		widgets::time::time_specs&
			widgets::time::specs(form& fm,
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
