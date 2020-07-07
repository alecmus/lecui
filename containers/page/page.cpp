/*
** page.cpp - page implementation
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

#include "../page.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::page(form& fm, const std::string& alias) :
			d_page_(*new impl(fm, *this, alias)) {}

		containers::page::~page() { delete& d_page_; }

		inline size containers::page::size() { return d_page_.size(); }

		class page_management::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		page_management::page_management(form& fm) : d_(*(new impl(fm))) {}
		page_management::~page_management() { delete& d_; }

		bool page_management::exists(const std::string& alias) {
			return d_.fm_.d_.p_pages_.count(alias) != 0;
		}

		containers::page& page_management::add(const std::string& alias) {
			if (d_.fm_.d_.p_pages_.count(alias)) {
				log("library usage error page::add");

				/// to-do: find a mechanism that makes sense ...
				return d_.fm_.d_.p_pages_.at(alias);
			}

			d_.fm_.d_.p_pages_.try_emplace(alias, d_.fm_, alias);
			auto& page_impl = d_.fm_.d_.p_pages_.at(alias).d_page_;

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(d_.fm_.d_.p_direct2d_factory_);

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(d_.fm_.d_.p_directwrite_factory_);

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(d_.fm_.d_.p_iwic_factory_);

			// set page size
			page_impl.size(d_.fm_.d_.size_);
			page_impl.width(page_impl.width() - (2.f * d_.fm_.d_.page_tolerance_));
			page_impl.height(page_impl.height() - (2.f * d_.fm_.d_.page_tolerance_ + d_.fm_.d_.caption_bar_height_));

			// get status pane sizes
			const auto rect_status_bottom = d_.fm_.d_.get_status_size(containers::status_pane::location::bottom);
			const auto rect_status_top = d_.fm_.d_.get_status_size(containers::status_pane::location::top);
			const auto rect_status_left = d_.fm_.d_.get_status_size(containers::status_pane::location::left);
			const auto rect_status_right = d_.fm_.d_.get_status_size(containers::status_pane::location::right);

			// adjust for status panes
			page_impl.height(page_impl.height() - rect_status_bottom.height);
			page_impl.height(page_impl.height() - rect_status_top.height);
			page_impl.width(page_impl.width() - rect_status_left.width);
			page_impl.width(page_impl.width() - rect_status_right.width);

			const float thickness = 10.f;
			const float margin = d_.fm_.d_.page_tolerance_;

			// initialize the page's horizontal scroll bar
			{
				auto& specs_ = page_impl.h_scrollbar().specs();
				specs_.on_resize.perc_width = 100;
				specs_.on_resize.perc_y = 100;

				specs_.rect.left = margin + thickness - d_.fm_.d_.page_tolerance_;
				specs_.rect.right = page_impl.size().width - (margin + thickness) - d_.fm_.d_.page_tolerance_;
				specs_.rect.bottom = page_impl.size().height - margin;
				specs_.rect.top = specs_.rect.bottom - thickness;

				specs_.color_fill = defaults::color(d_.fm_.d_.theme_, item::scrollbar);
				specs_.color_scrollbar_border = defaults::color(d_.fm_.d_.theme_, item::scrollbar_border);
				specs_.color_hot = defaults::color(d_.fm_.d_.theme_, item::scrollbar_hover);
				specs_.color_hot_pressed = defaults::color(d_.fm_.d_.theme_, item::scrollbar_pressed);
			}

			// initialize the page's vertical scroll bar
			{
				auto& specs_ = page_impl.v_scrollbar().specs();
				specs_.on_resize.perc_height = 100;
				specs_.on_resize.perc_x = 100;

				specs_.rect.top = margin + thickness - d_.fm_.d_.page_tolerance_;
				specs_.rect.bottom = page_impl.size().height - (margin + thickness);
				specs_.rect.right = page_impl.size().width - margin - d_.fm_.d_.page_tolerance_;
				specs_.rect.left = specs_.rect.right - thickness;

				specs_.color_fill = defaults::color(d_.fm_.d_.theme_, item::scrollbar);
				specs_.color_scrollbar_border = defaults::color(d_.fm_.d_.theme_, item::scrollbar_border);
				specs_.color_hot = defaults::color(d_.fm_.d_.theme_, item::scrollbar_hover);
				specs_.color_hot_pressed = defaults::color(d_.fm_.d_.theme_, item::scrollbar_pressed);
			}

			// add an invisible rect to bound the page. This is essential for scroll bars to work
			// appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets_impl::rectangle_impl::page_rect_alias());
			rectangle.color_fill.alpha = 0;

			// make it transparent
			rectangle.color_border = { 255, 0, 0, 0 };
			rectangle.color_hot = { 255, 0, 0, 0 };

			// set its dimensions to exactly match the page
			rectangle.rect.size(page_impl.size());
			rectangle.corner_radius_x = 15.f;
			rectangle.corner_radius_y = 15.f;
			rectangle.on_resize.perc_width = 100;
			rectangle.on_resize.perc_height = 100;

			// return reference to page so caller can add widgets to it
			return d_.fm_.d_.p_pages_.at(alias);
		}

		containers::page&
			page_management::get(form& fm, const std::string& alias) {
			try {
				// check form pages
				return fm.d_.p_pages_.at(alias);
			}
			catch (const std::exception&) {}
			try {
				// check status panes
				return fm.d_.p_status_panes_.at(alias);
			}
			catch (const std::exception&) {}

			throw std::invalid_argument("Invalid path");
		}

		void page_management::show(const std::string& alias) {
			log("page::show");

			d_.fm_.d_.current_page_ = alias;

			if (IsWindow(d_.fm_.d_.hWnd_))
				d_.fm_.d_.update();
		}
	}
}
