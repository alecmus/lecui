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
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::specs::label::operator==(const label& param) {
			return
				// generic specs
				widget::operator==(param) &&

				// widget specific specs
				(color_hot_pressed == param.color_hot_pressed) &&
				(multiline == param.multiline) &&
				(center_h == param.center_h) &&
				(center_v == param.center_v);
		}

		bool widgets::specs::label::operator!=(const label& param) {
			return !operator==(param);
		}

		class widgets::label::impl {
		public:
			impl(containers::page& page) :
				page_(page) {}
			containers::page& page_;
		};

		widgets::label::label(containers::page& page) :
			d_(*(new impl(page))) {}

		widgets::label::~label() { delete& d_; }

		widgets::specs::label&
			widgets::label::add(const std::string& alias) {
			return d_.page_.d_page_.add_label(alias);
		}

		widgets::specs::label&
			widgets::label::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_label(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
