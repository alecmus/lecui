/*
** icon.cpp - icon widget implementation
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

#include "../icon.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::icon::icon_specs::operator==(const icon_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(png_resource == param.png_resource) &&
				(file == param.file) &&
				(color_border == param.color_border) &&
				(color_description_text == param.color_description_text);
		}

		bool widgets::icon::icon_specs::operator!=(const icon_specs& param) {
			return !operator==(param);
		}

		class widgets::icon::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_icon(alias)) {
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::icon);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_border);
				specs_.color_hot = defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_hot);
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_text);
				specs_.color_description_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::icon_description_text);
			}
			containers::page& page_;
			icon_specs& specs_;
		};

		widgets::icon::icon(containers::page& page) :
			icon(page, "") {}

		widgets::icon::icon(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::icon::~icon() { delete& d_; }

		widgets::icon::icon_specs&
			widgets::icon::specs() {
			return d_.specs_;
		}

		widgets::icon::icon_specs&
			widgets::icon::operator()() {
			return specs();
		}

		widgets::icon::icon_specs&
			widgets::icon::specs(form& fm,
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
