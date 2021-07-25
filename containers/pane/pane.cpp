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
				_page(page), _specs(specs) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::pane))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::pane_border));
			}
			containers::page& _page;
			containers::pane_specs& _specs;
		};

		containers::pane_builder::pane_builder(containers::page& page, const float& content_margin) :
			pane_builder(page, "", content_margin) {}

		containers::pane_builder::pane_builder(containers::page& page,
			const std::string& alias, const float& content_margin) :
			_d(*(new impl(page, page._d_page.add_pane(alias, content_margin), alias))) {}

		containers::pane_builder::~pane_builder() { delete& _d; }

		containers::pane_specs& containers::pane_builder::specs() {
			return _d._specs;
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
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		containers::page& containers::pane_builder::get() {
			auto& _pane = _d._page._d_page.get_pane(_d._specs.alias());

			const std::string pane_name = "pane";

			if (_pane._p_panes.count(pane_name))
				return _pane._p_panes.at(pane_name);

			_pane._p_panes.try_emplace(pane_name, _d._page._d_page._fm, pane_name);
			_pane._current_pane = pane_name;
			auto& page_impl = _pane._p_panes.at(pane_name)._d_page;

			// specify direct2d factory (used internally for geometries and stuff)
			page_impl.direct2d_factory(_d._page._d_page.direct2d_factory());

			// specify directwrite factory (used internally for text rendering)
			page_impl.directwrite_factory(_d._page._d_page.directwrite_factory());

			// specify iwic imaging factory (used internally for image rendering)
			page_impl.iwic_factory(_d._page._d_page.iwic_factory());

			// specify parent
			page_impl.parent(_d._page);

			const float thickness = 10.f;	// thickness of scroll bars
			const float _content_margin = _pane.content_margin();
			rect rect_client_area = _pane().rect();

			// set page size
			page_impl.size({ rect_client_area.width(), rect_client_area.height() });
			page_impl.width(page_impl.width() - (2.f * _content_margin));
			page_impl.height(page_impl.height() - (2.f * _content_margin));

			// add an invisible rect to bound the page. This is essential for scroll bars
			// to work appropriately when contents don't reach the page borders
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

			// capture pointer to pane rect and page
			_pane._p_page_impl = &page_impl;
			_pane._p_pane_rect = &rectangle.rect();

			// define reference rect for scroll bars
			lecui::rect ref_rect = rectangle.rect();
			ref_rect.left() -= _content_margin;
			ref_rect.right() += _content_margin;
			ref_rect.top() -= _content_margin;
			ref_rect.bottom() += _content_margin;

			// initialize the page's horizontal scroll bar
			{
				auto& _specs = page_impl.h_scrollbar().specs();
				_specs.on_resize()
					.perc_width(100.f)
					.perc_y(100.f);

				auto width = rect_client_area.right() - rect_client_area.left() - 2.f * _content_margin;

				if (thickness > _content_margin)
					width -= (thickness - _content_margin);

				_specs.rect()
					.width(width)
					.height(thickness)
					.place(ref_rect, 50.f, 100.f);

				_specs
					.color_fill(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar))
					.color_scrollbar_border(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_border))
					.color_hot(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_pressed));
			}

			// initialize the page's vertical scroll bar
			{
				auto& _specs = page_impl.v_scrollbar().specs();
				_specs.on_resize()
					.perc_height(100.f)
					.perc_x(100.f);

				auto height = rect_client_area.bottom() - rect_client_area.top() - 2.f * _content_margin;

				if (thickness > _content_margin)
					height -= (thickness - _content_margin);

				_specs.rect()
					.width(thickness)
					.height(height)
					.place(ref_rect, 100.f, 50.f);

				_specs
					.color_fill(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar))
					.color_scrollbar_border(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_border))
					.color_hot(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_hover))
					.color_hot_pressed(defaults::color(_d._page._d_page._fm._d._theme, item::scrollbar_pressed));
			}

			// return reference to page so caller can add widgets to it
			return _pane._p_panes.at(pane_name);
		}

		containers::page&
			containers::pane_builder::get(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			try {
				// check form pages
				if (idx == std::string::npos)
					return fm._d._p_pages.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm._d._p_pages.at(page_alias);
					return fm._d.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}
			try {
				// check status panes
				if (idx == std::string::npos)
					return fm._d._p_status_panes.at(path);
				else {
					const auto page_alias = path.substr(0, idx);
					const auto path_remaining = path.substr(idx + 1);
					auto& page = fm._d._p_status_panes.at(page_alias);
					return fm._d.find_page(page, path_remaining);
				}
			}
			catch (const std::exception&) {}

			throw std::invalid_argument("Invalid path");
		}

		std::string& containers::pane_specs::text() { return _text; }

		containers::pane_specs& containers::pane_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::pane_specs::tooltip() { return _tooltip; }

		containers::pane_specs& containers::pane_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& containers::pane_specs::rect() { return _rect; }

		containers::pane_specs& containers::pane_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::resize_params& containers::pane_specs::on_resize() { return _on_resize; }

		containers::pane_specs& containers::pane_specs::on_resize(const widgets::resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::pane_specs::cursor() { return _cursor; }

		containers::pane_specs& containers::pane_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::pane_specs::font() { return _font; }

		containers::pane_specs& containers::pane_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::pane_specs::font_size() { return _font_size; }

		containers::pane_specs& containers::pane_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::pane_specs::color_text() { return _color_text; }

		containers::pane_specs& containers::pane_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::pane_specs::color_fill() { return _color_fill; }

		containers::pane_specs& containers::pane_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::pane_specs::color_hot() { return _color_hot; }

		containers::pane_specs& containers::pane_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::pane_specs::color_selected() { return _color_selected; }

		containers::pane_specs& containers::pane_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::pane_specs::color_disabled() { return _color_disabled; }

		containers::pane_specs& containers::pane_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::pane_specs::border() { return _border; }

		containers::pane_specs& containers::pane_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::pane_specs::color_border() { return _color_border; }

		containers::pane_specs& containers::pane_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::pane_specs::corner_radius_x() { return _corner_radius_x; }

		containers::pane_specs& containers::pane_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::pane_specs::corner_radius_y() { return _corner_radius_y; }

		containers::pane_specs& containers::pane_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}
	}
}
