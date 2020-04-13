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
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::page(form& fm, const std::string& alias) :
			d_page_(*new impl(fm, alias)) {}

		containers::page::~page() { delete& d_page_; }

		inline size containers::page::size() { return d_page_.size(); }

		class page::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		page::page(form& fm) : d_(*(new impl(fm))) {}
		page::~page() { delete& d_; }

		bool page::exists(const std::string& alias) {
			return d_.fm_.d_.p_pages_.count(alias) != 0;
		}

		containers::page& page::add(const std::string& alias) {
			log("page::add");

			if (d_.fm_.d_.p_pages_.count(alias)) {
				log("library usage error page::add");

				/// to-do: find a mechanism that makes sense ...
				return d_.fm_.d_.p_pages_.at(alias);
			}

			d_.fm_.d_.p_pages_.try_emplace(alias, d_.fm_, alias);

			// specify direct2d factory (used internally for geometries and stuff)
			d_.fm_.d_.p_pages_.at(alias).d_page_.direct2d_factory(d_.fm_.d_.p_direct2d_factory_);

			// specify directwrite factory (used internally for text rendering)
			d_.fm_.d_.p_pages_.at(alias).d_page_.directwrite_factory(d_.fm_.d_.p_directwrite_factory_);

			// specify iwic imaging factory (used internally for image rendering)
			d_.fm_.d_.p_pages_.at(alias).d_page_.iwic_factory(d_.fm_.d_.p_iwic_factory_);

			const float thickness = 10.f;
			const float margin = d_.fm_.d_.page_tolerance_;

			// initialize the page's horizontal scroll bar
			{
				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().resize.perc_width = 100;
				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().resize.perc_y = 100;

				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().rect.left =
					margin + thickness - d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().rect.right =
					d_.fm_.d_.size_.width - (margin + thickness) - d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().rect.bottom =
					d_.fm_.d_.size_.height - margin -
					d_.fm_.d_.caption_bar_height_ + d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().rect.top =
					d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().rect.bottom - thickness;

				d_.fm_.d_.p_pages_.at(alias).d_page_.h_scrollbar().specs().on_click = nullptr;
			}

			// initialize the page's vertical scroll bar
			{
				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().resize.perc_height = 100;
				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().resize.perc_x = 100;

				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().rect.top =
					margin + thickness - d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().rect.bottom =
					d_.fm_.d_.size_.height - (margin + thickness) -
					d_.fm_.d_.caption_bar_height_ + d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().rect.right =
					d_.fm_.d_.size_.width - margin - d_.fm_.d_.page_tolerance_;
				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().rect.left =
					d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().rect.right - thickness;

				d_.fm_.d_.p_pages_.at(alias).d_page_.v_scrollbar().specs().on_click = nullptr;
			}

			// add an invisible rect to bound the page. This is essential for scroll bars to work
			// appropriately when contents don't reach the page borders
			auto& rectangle =
				d_.fm_.d_.p_pages_.at(alias).d_page_.add_rectangle(widgets_impl::rectangle::page_rect_alias());
			rectangle.color_fill.alpha = 0;

			// make it transparent
			rectangle.color_border = { 255, 0, 0, 0 };
			rectangle.color_hot = { 255, 0, 0, 0 };

			d_.fm_.d_.p_pages_.at(alias).d_page_.size(d_.fm_.d_.size_);

			d_.fm_.d_.p_pages_.at(alias).d_page_.width(d_.fm_.d_.p_pages_.at(alias).d_page_.width() -
			(2.f * d_.fm_.d_.page_tolerance_));
			d_.fm_.d_.p_pages_.at(alias).d_page_.height(d_.fm_.d_.p_pages_.at(alias).d_page_.height() -
			(2.f * d_.fm_.d_.page_tolerance_ + d_.fm_.d_.caption_bar_height_));

			rectangle.rect.set(0, 0, d_.fm_.d_.p_pages_.at(alias).d_page_.width(),
				d_.fm_.d_.p_pages_.at(alias).d_page_.height());

			rectangle.corner_radius_x = 15.f;
			rectangle.corner_radius_y = 15.f;

			rectangle.resize.perc_width = 100;
			rectangle.resize.perc_height = 100;

			// return reference to page so caller can add widgets to it
			return d_.fm_.d_.p_pages_.at(alias);
		}

		containers::page&
			page::get(form& fm, const std::string& alias) {
			return fm.d_.p_pages_.at(alias);
		}

		void page::show(const std::string& alias) {
			log("page::show");

			d_.fm_.d_.current_page_ = alias;

			if (IsWindow(d_.fm_.d_.hWnd_))
				d_.fm_.d_.update();
		}
	}
}
