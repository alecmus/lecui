/*
** pane.cpp - pane container implementation
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

#include "../pane.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::pane::impl {
		public:
			impl(containers::page& page,
				containers::pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs), alias_(alias) {}
			containers::page& page_;
			containers::pane_specs& specs_;
			std::string alias_;
		};

		containers::pane::pane(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_pane(alias), alias))) {}

		containers::pane::~pane() { delete& d_; }

		containers::pane_specs& containers::pane::specs() {
			return d_.specs_;
		}

		containers::pane_specs&
			containers::pane::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_pane(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}

		containers::page& containers::pane::get() {
			auto& pane_ = d_.page_.d_page_.get_pane(d_.alias_);

			const std::string pane_name = "pane";

			if (pane_.p_panes_.count(pane_name))
				return pane_.p_panes_.at(pane_name);

			pane_.p_panes_.try_emplace(pane_name, d_.page_.d_page_.fm_, pane_name);
			pane_.current_pane_ = pane_name;

			// specify direct2d factory (used internally for geometries and stuff)
			pane_.p_panes_.at(pane_name).d_page_.direct2d_factory(d_.page_.d_page_.direct2d_factory());

			// specify directwrite factory (used internally for text rendering)
			pane_.p_panes_.at(pane_name).d_page_.directwrite_factory(d_.page_.d_page_.directwrite_factory());

			// specify iwic imaging factory (used internally for image rendering)
			pane_.p_panes_.at(pane_name).d_page_.iwic_factory(d_.page_.d_page_.iwic_factory());

			const float thickness = 10.f;
			const float margin = 10.f;
			const float page_tolerance_ = 10.f;
			rect rect_client_area = pane_.specs().rect;

			// initialize the page's horizontal scroll bar
			{
				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().on_resize.perc_width = 100.f;
				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().on_resize.perc_y = 100.f;

				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().rect.left = 0.f;
				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().rect.right =
					(rect_client_area.right - rect_client_area.left) - (margin + thickness);
				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().rect.bottom =
					(rect_client_area.bottom - rect_client_area.top) - page_tolerance_;
				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().rect.top =
					pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

				pane_.p_panes_.at(pane_name).d_page_.h_scrollbar().specs().on_click = nullptr;
			}

			// initialize the page's vertical scroll bar
			{
				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().on_resize.perc_height = 100.f;
				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().on_resize.perc_x = 100.f;

				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().rect.top = 0;
				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().rect.bottom =
					(rect_client_area.bottom - rect_client_area.top) - (margin + thickness);
				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().rect.right =
					(rect_client_area.right - rect_client_area.left) - margin;
				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().rect.left =
					pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().rect.right - thickness;

				pane_.p_panes_.at(pane_name).d_page_.v_scrollbar().specs().on_click = nullptr;
			}

			// add an invisible rect to bound the page. This is essential for scroll bars
			// to work appropriately when contents don't reach the page borders
			auto& rectangle = pane_.p_panes_.at(pane_name).d_page_.add_rectangle(widgets_impl::rectangle::page_rect_alias());
			rectangle.color_fill.alpha = 0;

			// make it transparent
			rectangle.color_border = { 255, 0, 0, 0 };
			rectangle.color_hot = { 255, 0, 0, 0 };

			pane_.p_panes_.at(pane_name).d_page_.size({ 0, 0 });
			pane_.p_panes_.at(pane_name).d_page_.width(rect_client_area.right - rect_client_area.left);
			pane_.p_panes_.at(pane_name).d_page_.height(rect_client_area.bottom - rect_client_area.top);

			pane_.p_panes_.at(pane_name).d_page_.width(pane_.p_panes_.at(pane_name).d_page_.width() -
			(2.f * page_tolerance_));
			pane_.p_panes_.at(pane_name).d_page_.height(pane_.p_panes_.at(pane_name).d_page_.height() -
			(2.f * page_tolerance_));

			rectangle.rect.set(0.f, 0.f, pane_.p_panes_.at(pane_name).d_page_.width(),
				pane_.p_panes_.at(pane_name).d_page_.height());

			rectangle.corner_radius_x = 15.f;
			rectangle.corner_radius_y = 15.f;

			rectangle.on_resize.perc_width = 100.f;
			rectangle.on_resize.perc_height = 100.f;

			// return reference to page so caller can add widgets to it
			return pane_.p_panes_.at(pane_name);
		}

		containers::page&
			containers::pane::get(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx == std::string::npos)
				return fm.d_.p_pages_.at(path);
			else {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				return fm.d_.find_page(page, path_remaining);
			}
		}
	}
}
