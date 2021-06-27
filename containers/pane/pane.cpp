//
// pane.cpp - pane container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs) {
				specs_
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::pane))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::pane_border));
			}
			containers::page& page_;
			containers::pane_specs& specs_;
		};

		containers::pane_builder::pane_builder(containers::page& page) :
			pane_builder(page, "") {}

		containers::pane_builder::pane_builder(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_pane(alias), alias))) {}

		containers::pane_builder::~pane_builder() { delete& d_; }

		containers::pane_specs& containers::pane_builder::specs() {
			return d_.specs_;
		}

		containers::pane_specs& containers::pane_builder::operator()() {
			return specs();
		}

		containers::pane_specs&
			containers::pane_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		containers::page& containers::pane_builder::get() {
			auto& pane_ = d_.page_.d_page_.get_pane(d_.specs_.alias());

			const std::string pane_name = "pane";

			if (pane_.p_panes_.count(pane_name))
				return pane_.p_panes_.at(pane_name);

			pane_.p_panes_.try_emplace(pane_name, d_.page_.d_page_.fm_, pane_name);
			pane_.current_pane_ = pane_name;
			auto& page_impl = pane_.p_panes_.at(pane_name).d_page_;

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(d_.page_.d_page_.direct2d_factory());

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(d_.page_.d_page_.directwrite_factory());

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(d_.page_.d_page_.iwic_factory());

			// specify parent
			page_impl.parent(d_.page_);

			const float thickness = 10.f;
			const float margin = 10.f;
			const float page_tolerance_ = 10.f;
			rect rect_client_area = pane_().rect();

			// initialize the page's horizontal scroll bar
			{
				auto& specs_ = page_impl.h_scrollbar().specs();
				specs_.on_resize().perc_width = 100.f;
				specs_.on_resize().perc_y = 100.f;

				specs_.rect().left = 0.f;
				specs_.rect().right =
					(rect_client_area.right - rect_client_area.left) - (margin + thickness);
				specs_.rect().bottom =
					(rect_client_area.bottom - rect_client_area.top) - page_tolerance_;
				specs_.rect().top = specs_.rect().bottom - thickness;

				specs_
					.color_fill(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
					.color_scrollbar_border(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
					.color_hot(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
			}

			// initialize the page's vertical scroll bar
			{
				auto& specs_ = page_impl.v_scrollbar().specs();
				specs_.on_resize().perc_height = 100.f;
				specs_.on_resize().perc_x = 100.f;

				specs_.rect().top = 0;
				specs_.rect().bottom =
					(rect_client_area.bottom - rect_client_area.top) - (margin + thickness);
				specs_.rect().right =
					(rect_client_area.right - rect_client_area.left) - margin;
				specs_.rect().left = specs_.rect().right - thickness;

				specs_
					.color_fill(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar))
					.color_scrollbar_border(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border))
					.color_hot(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed));
			}

			// set page size
			page_impl.size({ rect_client_area.width(), rect_client_area.height() });
			page_impl.width(page_impl.width() - (2.f * page_tolerance_));
			page_impl.height(page_impl.height() - (2.f * page_tolerance_));

			// add an invisible rect to bound the page. This is essential for scroll bars
			// to work appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
			rectangle.color_fill().alpha = 0;

			// make it transparent
			rectangle.color_border({ 255, 0, 0, 0 }).color_hot({ 255, 0, 0, 0 })

				// set its dimensions to exactly match the page
				.corner_radius_x(15.f)
				.corner_radius_y(15.f)
				.rect().size(page_impl.size());

			rectangle.on_resize().perc_width = 100.f;
			rectangle.on_resize().perc_height = 100.f;

			// return reference to page so caller can add widgets to it
			return pane_.p_panes_.at(pane_name);
		}

		containers::page&
			containers::pane_builder::get(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			try {
				// check form pages
				if (idx == std::string::npos)
					return fm.d_.p_pages_.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm.d_.p_pages_.at(page_alias);
					return fm.d_.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}
			try {
				// check status panes
				if (idx == std::string::npos)
					return fm.d_.p_status_panes_.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					return fm.d_.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}

			throw std::invalid_argument("Invalid path");
		}
	}
}
