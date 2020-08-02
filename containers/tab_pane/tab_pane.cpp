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
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::tab_pane::impl {
		public:
			impl(containers::page& page,
				containers::tab_pane::tab_pane_specs& specs,
				const std::string& alias) :
				page_(page), specs_(specs), alias_(alias) {
				specs_.color_text = defaults::color(page_.d_page_.fm_.d_.theme_, item::label);
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_pane);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_pane_border);
				specs_.color_tabs = defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_bar);
				specs_.color_selected = defaults::color(page_.d_page_.fm_.d_.theme_, item::tab_selected);
			}
			containers::page& page_;
			containers::tab_pane::tab_pane_specs& specs_;
			std::string alias_;
		};

		containers::tab_pane::tab_pane(containers::page& page) :
			tab_pane(page, "") {}

		containers::tab_pane::tab_pane(containers::page& page,
			const std::string& alias) :
			d_(*(new impl(page, page.d_page_.add_tab_pane(alias), alias))) {}

		containers::tab_pane::~tab_pane() { delete& d_; }

		containers::tab_pane::tab_pane_specs& containers::tab_pane::specs() {
			return d_.specs_;
		}

		containers::tab_pane::tab_pane_specs& containers::tab_pane::operator()() {
			return specs();
		}

		containers::tab_pane::tab_pane_specs&
			containers::tab_pane::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		void containers::tab_pane::select(const std::string& tab_name) {
			auto& tab_pane_ = d_.page_.d_page_.get_tab_pane(d_.alias_);
			tab_pane_.current_tab_ = tab_name;
		}

		class containers::tab::impl {
		public:
			impl(containers::tab_pane& tp, const std::string& tab_name) :
				tp_(tp),
				page_(add(tp, tab_name)) {}
			containers::tab_pane& tp_;
			containers::page& page_;

			static containers::page& add(tab_pane& tp, const std::string& tab_name) {
				auto& tab_pane_ = tp.d_.page_.d_page_.get_tab_pane(tp.d_.alias_);

				if (tab_pane_.p_tabs_.count(tab_name)) {
					log("library usage error containers::tab::impl::add");

					/// to-do: find a mechanism that makes sense ...
					return tab_pane_.p_tabs_.at(tab_name);
				}

				tab_pane_.p_tabs_.try_emplace(tab_name, tp.d_.page_.d_page_.fm_, tab_name);
				tab_pane_.tab_order_.push_back(tab_name);
				auto& page_impl = tab_pane_.p_tabs_.at(tab_name).d_page_;

				// specify direct2d factory (used internally for geometries and stuff)
				page_impl.direct2d_factory(tp.d_.page_.d_page_.direct2d_factory());

				// specify directwrite factory (used internally for text rendering)
				page_impl.directwrite_factory(tp.d_.page_.d_page_.directwrite_factory());

				// specify iwic imaging factory (used internally for image rendering)
				page_impl.iwic_factory(tp.d_.page_.d_page_.iwic_factory());

				// specify parent
				page_impl.parent(tp.d_.page_);

				const float thickness = 10.f;
				const float margin = 10.f;
				const float page_tolerance_ = 10.f;
				float tab_height_ = tab_pane_.tab_height();
				rect rect_client_area = tab_pane_().rect;

				bool tabs_perpendicular = false;

				switch (tp.d_.specs_.tab_side) {
				case tab_pane::side::left:
				case tab_pane::side::right:
					if (tp.d_.specs_.caption_orientation == tab_pane::orientation::horizontal)
						tabs_perpendicular = true;
					break;

				case tab_pane::side::top:
				case tab_pane::side::bottom:
				default: 
					if (tp.d_.specs_.caption_orientation == tab_pane::orientation::vertical)
						tabs_perpendicular = true;
					break;
				}

				if (tabs_perpendicular && !tab_pane_.tab_height_set()) {
					if (!tab_pane_.specs().caption_reserve.empty()) {
						log(tab_name + ": using caption reserve to compute dimensions");
						// compute the longest tab caption in the reserve
						for (const auto& alias : tab_pane_.specs().caption_reserve) {
							D2D1_RECT_F max_rect = { 0.f, 0.f, tab_pane_.specs().rect.height(), tab_pane_.caption_bar_height() };
							auto caption_rect = widgets::measure_label(tp.d_.page_.d_page_.p_directwrite_factory_, alias, tab_pane_.specs().font,
								tab_pane_.specs().font_size, false, true, max_rect);

							tab_height_ = largest(tab_height_, caption_rect.right - caption_rect.left + 3.f * tab_pane_.padding());
						}
					}
					else {
						log(tab_name + ": WARNING - no caption reserve for perpendicular tab captions!");
						// use current caption to set tab height
						D2D1_RECT_F max_rect = { 0.f, 0.f, tab_pane_.specs().rect.height(), tab_pane_.caption_bar_height() };
						auto caption_rect = widgets::measure_label(tp.d_.page_.d_page_.p_directwrite_factory_, tab_name, tab_pane_.specs().font,
							tab_pane_.specs().font_size, false, true, max_rect);

						tab_height_ = largest(tab_height_, caption_rect.right - caption_rect.left + 3.f * tab_pane_.padding());
					}

					tab_pane_.set_tab_height(tab_height_);
				}

				const auto caption_bar_height_ = tab_pane_.caption_bar_height();

				// initialize the page's horizontal scroll bar
				{
					auto& specs_ = page_impl.h_scrollbar().specs();
					specs_.on_resize.perc_width = 100.f;
					specs_.on_resize.perc_y = 100.f;

					switch (tp.d_.specs_.tab_side)
					{
					case tab_pane::side::left:
					case tab_pane::side::right:
						specs_.rect.left = 0.f;
						specs_.rect.right =
							(rect_client_area.right - rect_client_area.left) - (margin + thickness) -
							caption_bar_height_;
						specs_.rect.bottom = (rect_client_area.bottom - rect_client_area.top) -
							page_tolerance_;
						specs_.rect.top = specs_.rect.bottom - thickness;
						break;

					case tab_pane::side::top:
					case tab_pane::side::bottom:
					default:
						specs_.rect.left = 0.f;
						specs_.rect.right =
							(rect_client_area.right - rect_client_area.left) - (margin + thickness);
						specs_.rect.bottom = (rect_client_area.bottom - rect_client_area.top) -
							(caption_bar_height_ + page_tolerance_);
						specs_.rect.top = specs_.rect.bottom - thickness;
						break;
					}

					specs_.color_fill = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar);
					specs_.color_scrollbar_border = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border);
					specs_.color_hot = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover);
					specs_.color_hot_pressed = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed);
				}

				// initialize the page's vertical scroll bar
				{
					auto& specs_ = page_impl.v_scrollbar().specs();
					specs_.on_resize.perc_height = 100.f;
					specs_.on_resize.perc_x = 100.f;

					switch (tp.d_.specs_.tab_side)
					{
					case tab_pane::side::left:
					case tab_pane::side::right:
						specs_.rect.top = 0.f;
						specs_.rect.bottom = (rect_client_area.bottom - rect_client_area.top) -
							(margin + thickness);
						specs_.rect.right =
							(rect_client_area.right - rect_client_area.left) - margin - caption_bar_height_;
						specs_.rect.left = specs_.rect.right - thickness;
						break;

					case tab_pane::side::top:
					case tab_pane::side::bottom:
					default:
						specs_.rect.top = 0.f;
						specs_.rect.bottom = (rect_client_area.bottom - rect_client_area.top) -
							(margin + thickness) - caption_bar_height_;
						specs_.rect.right =
							(rect_client_area.right - rect_client_area.left) - margin;
						specs_.rect.left = specs_.rect.right - thickness;
						break;
					}

					specs_.color_fill = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar);
					specs_.color_scrollbar_border = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_border);
					specs_.color_hot = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_hover);
					specs_.color_hot_pressed = defaults::color(tp.d_.page_.d_page_.fm_.d_.theme_, item::scrollbar_pressed);
				}

				// set page size
				page_impl.size({ rect_client_area.width(), rect_client_area.height() });

				switch (tp.d_.specs_.tab_side)
				{
				case tab_pane::side::left:
				case tab_pane::side::right:
					page_impl.width(page_impl.width() - (2.f * page_tolerance_ + caption_bar_height_));
					page_impl.height(page_impl.height() - (2.f * page_tolerance_));
					break;

				case tab_pane::side::top:
				case tab_pane::side::bottom:
				default:
					page_impl.width(page_impl.width() - (2.f * page_tolerance_));
					page_impl.height(page_impl.height() - (2.f * page_tolerance_ + caption_bar_height_));
					break;
				}

				// add an invisible rect to bound the page. This is essential for scroll bars
				// to work appropriately when contents don't reach the page borders
				auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
				rectangle.color_fill.alpha = 0;

				// make it transparent
				rectangle.color_border = { 255, 0, 0, 0 };
				rectangle.color_hot = { 255, 0, 0, 0 };

				// set its dimensions to exactly match the page
				rectangle.rect.size(page_impl.size());
				rectangle.corner_radius_x = 15.f;
				rectangle.corner_radius_y = 15.f;
				rectangle.on_resize.perc_width = 100.f;
				rectangle.on_resize.perc_height = 100.f;

				// return reference to page so caller can add widgets to it
				return tab_pane_.p_tabs_.at(tab_name);
			}
		};

		containers::tab::tab(containers::tab_pane& tp, const std::string& tab_name) :
			d_(*(new impl(tp, tab_name))) {}

		containers::tab::~tab() { delete& d_; }

		containers::page& containers::tab::get() {
			return d_.page_;
		}

		containers::page&
			containers::tab::get(form& fm, const std::string& path) {
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
