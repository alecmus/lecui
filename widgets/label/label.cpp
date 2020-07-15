/*
** label.cpp - label widget implementation
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

#include "../label.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::label::label_specs::operator==(const label_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(text == param.text) &&
				(color_hot_pressed == param.color_hot_pressed) &&
				(multiline == param.multiline) &&
				(center_h == param.center_h) &&
				(center_v == param.center_v);
		}

		bool widgets::label::label_specs::operator!=(const label_specs& param) {
			return !operator==(param);
		}

		class widgets::label::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_label(alias)) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::label_selected);
				specs_.color_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::label_hover);
				specs_.color_hot_pressed = defaults::color(page_.d_page_.fm_.d_.theme_, item::label_pressed);
				specs_.color_disabled = defaults::color(page_.d_page_.fm_.d_.theme_, item::label_disabled);
			}
			containers::page& page_;
			label_specs& specs_;
		};

		widgets::label::label(containers::page& page) :
			label(page, "") {}

		widgets::label::label(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::label::~label() { delete& d_; }

		widgets::label::label_specs&
			widgets::label::specs() {
			return d_.specs_;
		}

		widgets::label::label_specs&
			widgets::label::operator()() {
			return specs();
		}

		widgets::label::label_specs&
			widgets::label::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
