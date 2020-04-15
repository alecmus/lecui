/*
** table.cpp - table widget implementation
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

#include "../table.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::table::table_specs::operator==(const table_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_dropdown_hot == param.color_dropdown_hot) &&
				(color_menu == param.color_menu) &&
				(color_grid == param.color_grid) &&
				(color_text_header == param.color_text_header) &&
				(color_fill_header == param.color_fill_header) &&
				(color_fill_alternate == param.color_fill_alternate) &&
				(color_row_hot == param.color_row_hot) &&
				(color_row_selected == param.color_row_selected);
		}

		bool widgets::table::table_specs::operator!=(const table_specs& param) {
			return !operator==(param);
		}

		class widgets::table::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_table(alias)) {}
			containers::page& page_;
			table_specs& specs_;
		};

		widgets::table::table(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::table::~table() { delete& d_; }

		widgets::table::table_specs&
			widgets::table::specs() {
			return d_.specs_;
		}

		widgets::table::table_specs&
			widgets::table::operator()() {
			return specs();
		}

		widgets::table::table_specs&
			widgets::table::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_table(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
