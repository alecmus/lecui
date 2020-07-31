/*
** table_view.cpp - table view widget implementation
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

#include "../table_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::table_view::table_view_specs::operator==(const table_view_specs& param) {
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

		bool widgets::table_view::table_view_specs::operator!=(const table_view_specs& param) {
			return !operator==(param);
		}

		class widgets::table_view::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_table_view(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_border);
				specs_.color_fill_alternate = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_alternate);
				specs_.color_row_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_row_hover);
				specs_.color_row_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_row_selected);
				specs_.color_text_header = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_text_header);
				specs_.color_fill_header = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_header);
				specs_.color_grid = defaults::color(page_.d_page_.fm_.d_.theme_, item::table_view_grid);
			}
			containers::page& page_;
			table_view_specs& specs_;
		};

		widgets::table_view::table_view(containers::page& page) :
			table_view(page, "") {}

		widgets::table_view::table_view(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::table_view::~table_view() { delete& d_; }

		widgets::table_view::table_view_specs&
			widgets::table_view::specs() {
			return d_.specs_;
		}

		widgets::table_view::table_view_specs&
			widgets::table_view::operator()() {
			return specs();
		}

		widgets::table_view::table_view_specs&
			widgets::table_view::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_table_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}