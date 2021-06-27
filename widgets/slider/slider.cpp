//
// slider.cpp - slider widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../slider.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::slider_specs::operator==(const slider_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_tick_ == param.color_tick_) &&
				(color_knob_ == param.color_knob_) &&
				(color_knob_hot_ == param.color_knob_hot_) &&
				(color_knob_border_ == param.color_knob_border_);
		}

		bool widgets::slider_specs::operator!=(const slider_specs& param) {
			return !operator==(param);
		}

		class widgets::slider_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_slider(alias)) {
				specs_
					.font_size(7.f)
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_border))
					.color_knob(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob))
					.color_knob_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_hot))
					.color_knob_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_border))
					.color_tick(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_tick))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_selected));
			}
			containers::page& page_;
			slider_specs& specs_;
		};

		widgets::slider_builder::slider_builder(containers::page& page) :
			slider_builder(page, "") {}

		widgets::slider_builder::slider_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::slider_builder::~slider_builder() { delete& d_; }

		widgets::slider_specs&
			widgets::slider_builder::specs() {
			return d_.specs_;
		}

		widgets::slider_specs&
			widgets::slider_builder::operator()() {
			return specs();
		}

		widgets::slider_specs&
			widgets::slider_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
