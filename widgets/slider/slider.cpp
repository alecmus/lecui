/*
** slider.cpp - slider widget implementation
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

#include "../slider.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::slider::slider_specs::operator==(const slider_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border) &&
				(color_tick == param.color_tick) &&
				(color_knob == param.color_knob) &&
				(color_knob_hot == param.color_knob_hot) &&
				(color_knob_border == param.color_knob_border);
		}

		bool widgets::slider::slider_specs::operator!=(const slider_specs& param) {
			return !operator==(param);
		}

		class widgets::slider::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_slider(alias)) {
				specs_.font_size = 7.f;
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_border);
				specs_.color_knob = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob);
				specs_.color_knob_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_hot);
				specs_.color_knob_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_border);
				specs_.color_tick = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_tick);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_selected);
			}
			containers::page& page_;
			slider_specs& specs_;
		};

		widgets::slider::slider(containers::page& page) :
			slider(page, "") {}

		widgets::slider::slider(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::slider::~slider() { delete& d_; }

		widgets::slider::slider_specs&
			widgets::slider::specs() {
			return d_.specs_;
		}

		widgets::slider::slider_specs&
			widgets::slider::operator()() {
			return specs();
		}

		widgets::slider::slider_specs&
			widgets::slider::specs(form& fm, const std::string& path) {
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
