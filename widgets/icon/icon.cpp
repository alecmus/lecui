//
// icon.cpp - icon widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../icon.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::icon_specs::operator==(const icon_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(png_resource_ == param.png_resource_) &&
				(file_ == param.file_) &&
				(color_border_ == param.color_border_) &&
				(color_text_description_ == param.color_text_description_);
		}

		bool widgets::icon_specs::operator!=(const icon_specs& param) {
			return !operator==(param);
		}

		class widgets::icon_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_icon(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_hot))
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_text))
					.color_text_description(defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_description_text));
			}
			containers::page& page_;
			icon_specs& specs_;
		};

		widgets::icon_builder::icon_builder(containers::page& page) :
			icon_builder(page, "") {}

		widgets::icon_builder::icon_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::icon_builder::~icon_builder() { delete& d_; }

		widgets::icon_specs&
			widgets::icon_builder::specs() {
			return d_.specs_;
		}

		widgets::icon_specs&
			widgets::icon_builder::operator()() {
			return specs();
		}

		widgets::icon_specs&
			widgets::icon_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_icon(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
