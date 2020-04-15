/*
** toggle.cpp - toggle widget implementation
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

#include "../toggle.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::toggle::toggle_specs::operator==(const toggle_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_on == param.color_on) &&
				(color_off == param.color_off);
		}

		bool widgets::toggle::toggle_specs::operator!=(const toggle_specs& param) {
			return !operator==(param);
		}

		class widgets::toggle::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_toggle(alias)) {}
			containers::page& page_;
			toggle_specs& specs_;
		};

		widgets::toggle::toggle(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::toggle::~toggle() { delete& d_; }

		widgets::toggle::toggle_specs&
			widgets::toggle::specs() {
			return d_.specs_;
		}

		widgets::toggle::toggle_specs&
			widgets::toggle::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_toggle(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
