/*
** line.cpp - line widget implementation
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

#include "../line.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::line::line_specs::operator==(const line_specs& param) {
			return
				// generic specs
				specs::operator==(param);
		}

		bool widgets::line::line_specs::operator!=(const line_specs& param) {
			return !operator==(param);
		}

		class widgets::line::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_line(alias)) {
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::line);
				specs_.color_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::line_hover);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::line_selected);

			}
			containers::page& page_;
			line_specs& specs_;
		};

		widgets::line::line(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::line::~line() { delete& d_; }

		widgets::line::line_specs&
			widgets::line::specs() {
			return d_.specs_;
		}

		widgets::line::line_specs&
			widgets::line::operator()() {
			return specs();
		}

		widgets::line::line_specs&
			widgets::line::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
