/*
** progress_bar.cpp - progress bar widget implementation
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

#include "../progress_bar.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::progress_bar::progress_bar_specs::operator==(const progress_bar_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border);
		}

		bool widgets::progress_bar::progress_bar_specs::operator!=(const progress_bar_specs& param) {
			return !operator==(param);
		}

		class widgets::progress_bar::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_progress_bar(alias)) {
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::progress_bar_border);
			}
			containers::page& page_;
			progress_bar_specs& specs_;
		};

		widgets::progress_bar::progress_bar(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::progress_bar::~progress_bar() { delete& d_; }

		widgets::progress_bar::progress_bar_specs&
			widgets::progress_bar::specs() {
			return d_.specs_;
		}

		widgets::progress_bar::progress_bar_specs&
			widgets::progress_bar::operator()() {
			return specs();
		}

		widgets::progress_bar::progress_bar_specs&
			widgets::progress_bar::specs(form& fm, const std::string& path) {
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
