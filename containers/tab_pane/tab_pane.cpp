/*
** tab_pane.cpp - tab pane implementation
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

#include "../tab_pane.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::tab_pane::impl {
		public:
			impl(containers::page& page,
				containers::tab_pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs), alias_(alias) {}
			containers::page& page_;
			containers::tab_pane_specs& specs_;
			std::string alias_;
		};

		containers::tab_pane::tab_pane(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_tab_pane(alias), alias))) {}

		containers::tab_pane::~tab_pane() { delete& d_; }

		containers::tab_pane_specs& containers::tab_pane::specs() {
			return d_.specs_;
		}

		containers::tab_pane_specs&
			containers::tab_pane::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_tab_pane(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}

		void containers::tab_pane::select(const std::string& tab_name) {
			auto& tab_pane_ = d_.page_.d_page_.get_tab_pane(d_.alias_);
			tab_pane_.current_tab_ = tab_name;
		}

		class containers::tab::impl {
		public:
			impl(containers::tab_pane& tc) :
				tc_(tc) {}
			containers::tab_pane& tc_;
		};

		containers::tab::tab(containers::tab_pane& tc) :
			d_(*(new impl(tc))) {}

		containers::tab::~tab() { delete& d_; }

		containers::page& containers::tab::add(const std::string& tab_name) {
			auto& tab_pane_ = d_.tc_.d_.page_.d_page_.get_tab_pane(d_.tc_.d_.alias_);

			log("containers::tab::add");

			if (tab_pane_.p_tabs_.count(tab_name)) {
				log("library usage error containers::tab::add");

				/// to-do: find a mechanism that makes sense ...
				return tab_pane_.p_tabs_.at(tab_name);
			}

			tab_pane_.p_tabs_.try_emplace(tab_name, d_.tc_.d_.page_.d_page_.fm_, tab_name);

			// specify direct2d factory (used internally for geometries and stuff)
			tab_pane_.p_tabs_.at(tab_name).d_page_.direct2d_factory(
				d_.tc_.d_.page_.d_page_.direct2d_factory());

			// specify directwrite factory (used internally for text rendering)
			tab_pane_.p_tabs_.at(tab_name).d_page_.directwrite_factory(
				d_.tc_.d_.page_.d_page_.directwrite_factory());

			// specify iwic imaging factory (used internally for image rendering)
			tab_pane_.p_tabs_.at(tab_name).d_page_.iwic_factory(d_.tc_.d_.page_.d_page_.iwic_factory());

			const float thickness = 10.f;
			const float margin = 10.f;
			const float page_tolerance_ = 10.f;
			const float caption_bar_height_ = tab_pane_.caption_bar_height();
			rect rect_client_area = tab_pane_.specs().rect;

			// initialize the page's horizontal scroll bar
			{
				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().on_resize.perc_width = 100.f;
				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().on_resize.perc_y = 100.f;

				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().rect.left = 0.f;
				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().rect.right =
					(rect_client_area.right - rect_client_area.left) - (margin + thickness);
				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().rect.bottom =
					(rect_client_area.bottom - rect_client_area.top) -
					(caption_bar_height_ + page_tolerance_);
				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().rect.top =
					tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().rect.bottom - thickness;

				tab_pane_.p_tabs_.at(tab_name).d_page_.h_scrollbar().specs().on_click = nullptr;
			}

			// initialize the page's vertical scroll bar
			{
				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().on_resize.perc_height = 100.f;
				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().on_resize.perc_x = 100.f;

				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().rect.top = 0.f;
				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().rect.bottom =
					(rect_client_area.bottom - rect_client_area.top) -
					(margin + thickness) - caption_bar_height_;
				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().rect.right =
					(rect_client_area.right - rect_client_area.left) - margin;
				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().rect.left =
					tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().rect.right - thickness;

				tab_pane_.p_tabs_.at(tab_name).d_page_.v_scrollbar().specs().on_click = nullptr;
			}

			// add an invisible rect to bound the page. This is essential for scroll bars
			// to work appropriately when contents don't reach the page borders
			auto& rectangle =
				tab_pane_.p_tabs_.at(tab_name).d_page_.add_rectangle(widgets_impl::rectangle::page_rect_alias());
			rectangle.color_fill.alpha = 0;

			// make it transparent
			rectangle.color_border = { 255, 0, 0, 0 };
			rectangle.color_hot = { 255, 0, 0, 0 };

			tab_pane_.p_tabs_.at(tab_name).d_page_.size({ 0.f, 0.f });
			tab_pane_.p_tabs_.at(tab_name).d_page_.width(rect_client_area.right - rect_client_area.left);
			tab_pane_.p_tabs_.at(tab_name).d_page_.height(rect_client_area.bottom - rect_client_area.top);

			tab_pane_.p_tabs_.at(tab_name).d_page_.width(tab_pane_.p_tabs_.at(tab_name).d_page_.width() -
			(2.f * page_tolerance_));
			tab_pane_.p_tabs_.at(tab_name).d_page_.height(tab_pane_.p_tabs_.at(tab_name).d_page_.height() -
			(2.f * page_tolerance_ + caption_bar_height_));

			rectangle.rect.set(0.f, 0.f, tab_pane_.p_tabs_.at(tab_name).d_page_.width(),
				tab_pane_.p_tabs_.at(tab_name).d_page_.height());

			rectangle.corner_radius_x = 15.f;
			rectangle.corner_radius_y = 15.f;

			rectangle.on_resize.perc_width = 100.f;
			rectangle.on_resize.perc_height = 100.f;

			// return reference to page so caller can add widgets to it
			return tab_pane_.p_tabs_.at(tab_name);
		}

		containers::page&
			containers::tab::get(form& fm, const std::string& path) {
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
