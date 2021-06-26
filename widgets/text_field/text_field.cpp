//
// text_field.cpp - text_field widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../text_field.h"
#include "../password_field.h"
#include "../../form_impl/form_impl.h"
#include "..\password_field.h"

namespace liblec {
	namespace lecui {
		bool widgets::text_field::text_field_specs::operator==(const text_field_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_prompt_ == param.color_prompt_) &&
				(color_caret_ == param.color_caret_);
		}

		bool widgets::text_field::text_field_specs::operator!=(const text_field_specs& param) {
			return !operator==(param);
		}

		class widgets::text_field::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_text_field(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_prompt(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_prompt))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_border))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_disabled))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_selected))
					.color_caret(defaults::color(page_.d_page_.fm_.d_.theme_, item::text_field_caret));
			}
			containers::page& page_;
			text_field_specs& specs_;
		};

		widgets::text_field::text_field(containers::page& page) :
			text_field(page, "") {}

		widgets::text_field::text_field(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::text_field::~text_field() { delete& d_; }

		widgets::text_field::text_field_specs&
			widgets::text_field::specs() {
			return d_.specs_;
		}

		widgets::text_field::text_field_specs&
			widgets::text_field::operator()() {
			return specs();
		}

		widgets::text_field::text_field_specs&
			widgets::text_field::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_text_field(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_text_field(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::password_field::password_field(containers::page& page) :
			password_field(page, "") {}

		widgets::password_field::password_field(containers::page& page, const std::string& alias) :
			text_field(page, alias) {
			d_.specs_.mask = '•';
			d_.specs_.prompt("Enter password here");
		}

		widgets::password_field::~password_field() {}
	}
}
