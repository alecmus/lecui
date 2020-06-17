/*
** html_editor.cpp - html editor widget implementation
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

#include "../html_editor.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::html_editor::html_editor_specs::operator==(const html_editor_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_caret == param.color_caret);
		}

		bool widgets::html_editor::html_editor_specs::operator!=(const html_editor_specs& param) {
			return !operator==(param);
		}

		class widgets::html_editor::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_html_editor(alias)) {
				specs_.font = "Calibri";
				specs_.font_size = 11.f;
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_border);
				specs_.color_disabled = defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_disabled);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_selected);
				specs_.color_caret = defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_caret);
			}
			containers::page& page_;
			html_editor_specs& specs_;
		};

		widgets::html_editor::html_editor(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::html_editor::~html_editor() { delete& d_; }

		widgets::html_editor::html_editor_specs&
			widgets::html_editor::specs() {
			return d_.specs_;
		}

		widgets::html_editor::html_editor_specs&
			widgets::html_editor::operator()() {
			return specs();
		}

		widgets::html_editor::html_editor_specs&
			widgets::html_editor::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
