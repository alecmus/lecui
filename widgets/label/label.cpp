//
// label.cpp - label widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../label.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::label_specs::operator==(const label_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(text_ == param.text_) &&
				(color_hot_pressed_ == param.color_hot_pressed_) &&
				(multiline_ == param.multiline_) &&
				(center_h_ == param.center_h_) &&
				(center_v_ == param.center_v_);
		}

		bool widgets::label_specs::operator!=(const label_specs& param) {
			return !operator==(param);
		}

		class widgets::label_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_label(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_selected))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_hover))
					.color_hot_pressed(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_pressed))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::label_disabled));
			}
			containers::page& page_;
			label_specs& specs_;
		};

		widgets::label_builder::label_builder(containers::page& page) :
			label_builder(page, "") {}

		widgets::label_builder::label_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::label_builder::~label_builder() { delete& d_; }

		widgets::label_specs&
			widgets::label_builder::specs() {
			return d_.specs_;
		}

		widgets::label_specs&
			widgets::label_builder::operator()() {
			return specs();
		}

		widgets::label_specs&
			widgets::label_builder::specs(form& fm,
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
