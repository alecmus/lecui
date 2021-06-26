//
// image_view.cpp - image view widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../image_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::image_view::image_view_specs::operator==(const image_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(file_ == param.file_) &&
				(png_resource_ == param.png_resource_) &&
				(quality_ == param.quality_) &&
				(enlarge_if_smaller_ == param.enlarge_if_smaller_) &&
				(keep_aspect_ratio_ == param.keep_aspect_ratio_);
		}

		bool widgets::image_view::image_view_specs::operator!=(const image_view_specs& param) {
			return !operator==(param);
		}

		class widgets::image_view::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_image_view(alias)) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::image_view_selected));
			}
			containers::page& page_;
			image_view_specs& specs_;
		};

		widgets::image_view::image_view(containers::page& page) :
			image_view(page, "") {}

		widgets::image_view::image_view(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::image_view::~image_view() { delete& d_; }

		widgets::image_view::image_view_specs&
			widgets::image_view::specs() {
			return d_.specs_;
		}

		widgets::image_view::image_view_specs&
			widgets::image_view::operator()() {
			return specs();
		}

		widgets::image_view::image_view_specs&
			widgets::image_view::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_image_view(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
