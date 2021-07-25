//
// page.cpp - page implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../page.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		containers::page::page(form& fm, const std::string& alias) :
			_d_page(*new impl(fm, *this, alias)) {}

		containers::page::~page() { delete& _d_page; }

		inline const size containers::page::size() { return _d_page.size(); }

		class page_manager::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		page_manager::page_manager(form& fm) : _d(*(new impl(fm))) {}
		page_manager::~page_manager() { delete& _d; }

		bool page_manager::exists(const std::string& alias) {
			return _d._fm._d._p_pages.count(alias) != 0;
		}

		containers::page& page_manager::add(const std::string& alias) {
			if (_d._fm._d._p_pages.count(alias)) {
				log("library usage error page::add");

				/// to-do: find a mechanism that makes sense ...
				return _d._fm._d._p_pages.at(alias);
			}

			_d._fm._d._p_pages.try_emplace(alias, _d._fm, alias);
			auto& page_impl = _d._fm._d._p_pages.at(alias)._d_page;

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(_d._fm._d._p_direct2d_factory);

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(_d._fm._d._p_directwrite_factory);

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(_d._fm._d._p_iwic_factory);

			// set page size
			page_impl.size(_d._fm._d._size);
			page_impl.width(page_impl.width() - (2.f * _d._fm._d._content_margin));
			page_impl.height(page_impl.height() - (2.f * _d._fm._d._content_margin + _d._fm._d._caption_bar_height));

			// get status pane sizes
			const auto rect_status_bottom = _d._fm._d.get_status_size(containers::status_pane_specs::location::bottom);
			const auto rect_status_top = _d._fm._d.get_status_size(containers::status_pane_specs::location::top);
			const auto rect_status_left = _d._fm._d.get_status_size(containers::status_pane_specs::location::left);
			const auto rect_status_right = _d._fm._d.get_status_size(containers::status_pane_specs::location::right);

			// adjust for status panes
			page_impl.height(page_impl.height() - rect_status_bottom.height);
			page_impl.height(page_impl.height() - rect_status_top.height);
			page_impl.width(page_impl.width() - rect_status_left.width);
			page_impl.width(page_impl.width() - rect_status_right.width);

			const float thickness = 10.f;
			const float margin = _d._fm._d._content_margin;

			// initialize the page's horizontal scroll bar
			{
				auto& _specs = page_impl.h_scrollbar().specs();
				_specs.on_resize()
					.perc_width(100.f).perc_y(100.f);

				_specs.rect()
					.left(margin + thickness - _d._fm._d._content_margin)
					.right(page_impl.size().width - (margin + thickness) - _d._fm._d._content_margin)
					.bottom(page_impl.size().height - margin)
					.top(_specs.rect().bottom() - thickness);

				_specs
					.color_fill(defaults::color(_d._fm._d._theme, item::scrollbar))
					.color_scrollbar_border(defaults::color(_d._fm._d._theme, item::scrollbar_border))
					.color_hot(defaults::color(_d._fm._d._theme, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(_d._fm._d._theme, item::scrollbar_pressed));
			}

			// initialize the page's vertical scroll bar
			{
				auto& _specs = page_impl.v_scrollbar().specs();
				_specs.on_resize()
					.perc_height(100.f).perc_x(100.f);

				_specs.rect()
					.top(margin + thickness - _d._fm._d._content_margin)
					.bottom(page_impl.size().height - (margin + thickness))
					.right(page_impl.size().width - margin - _d._fm._d._content_margin)
					.left(_specs.rect().right() - thickness);

				_specs
					.color_fill(defaults::color(_d._fm._d._theme, item::scrollbar))
					.color_scrollbar_border(defaults::color(_d._fm._d._theme, item::scrollbar_border))
					.color_hot(defaults::color(_d._fm._d._theme, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(_d._fm._d._theme, item::scrollbar_pressed));
			}

			// add an invisible rect to bound the page. This is essential for scroll bars to work
			// appropriately when contents don't reach the page borders
			auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
			rectangle.color_fill().alpha(0);

			// make it transparent
			rectangle.color_border({ 255, 0, 0, 0 }).color_hot({ 255, 0, 0, 0 })

				// set its dimensions to exactly match the page
				.corner_radius_x(15.f)
				.corner_radius_y(15.f)
				.rect().size(page_impl.size());

			rectangle.on_resize()
				.perc_width(100.f).perc_height(100.f);

			// return reference to page so caller can add widgets to it
			return _d._fm._d._p_pages.at(alias);
		}

		containers::page&
			page_manager::get(form& fm, const std::string& alias) {
			try {
				// check form pages
				return fm._d._p_pages.at(alias);
			}
			catch (const std::exception&) {}
			try {
				// check status panes
				return fm._d._p_status_panes.at(alias);
			}
			catch (const std::exception&) {}

			throw std::invalid_argument("Invalid path");
		}

		void page_manager::show(const std::string& alias) {
			_d._fm._d._current_page = alias;

			if (IsWindow(_d._fm._d._hWnd))
				_d._fm._d.update();
		}

		void page_manager::close(const std::string& path) {
			_d._fm._d.close_container(path);
		}
	}
}
