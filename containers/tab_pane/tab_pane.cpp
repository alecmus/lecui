//
// tab_pane.cpp - tab pane implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../tab_pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class containers::tab_pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::tab_pane_specs& specs,
				const std::string& alias) :
				_page(page), _specs(specs) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::tab_pane))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::tab_pane_border))
					.color_tabs(defaults::color(_page._d_page._fm._d._theme, item::tab_bar))
					.color_tabs_border(defaults::color(_page._d_page._fm._d._theme, item::tab_pane_border))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::tab_selected));
			}
			containers::page& _page;
			containers::tab_pane_specs& _specs;
		};

		containers::tab_pane_builder::tab_pane_builder(containers::page& page, const float& content_margin) :
			tab_pane_builder(page, "", content_margin) {}

		containers::tab_pane_builder::tab_pane_builder(containers::page& page,
			const std::string& alias, const float& content_margin) :
			_d(*(new impl(page, page._d_page.add_tab_pane(alias, content_margin), alias))) {}

		containers::tab_pane_builder::~tab_pane_builder() { delete& _d; }

		containers::tab_pane_specs& containers::tab_pane_builder::specs() {
			return _d._specs;
		}

		containers::tab_pane_specs& containers::tab_pane_builder::operator()() {
			return specs();
		}

		containers::tab_pane_specs&
			containers::tab_pane_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tab_pane(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		void containers::tab_pane_builder::select(const std::string& tab_name) {
			auto& _tab_pane = _d._page._d_page.get_tab_pane(_d._specs.alias());
			_tab_pane._current_tab = tab_name;
		}

		class containers::tab_builder::impl {
		public:
			impl(containers::tab_pane_builder& tp, const std::string& tab_name) :
				_tp(tp),
				_page(add(tp, tab_name)) {}
			containers::tab_pane_builder& _tp;
			containers::page& _page;

			static containers::page& add(tab_pane_builder& tp, const std::string& tab_name) {
				auto& _tab_pane = tp._d._page._d_page.get_tab_pane(tp._d._specs.alias());

				if (_tab_pane._p_tabs.count(tab_name)) {
					log("library usage error containers::tab::impl::add");

					/// to-do: find a mechanism that makes sense ...
					return _tab_pane._p_tabs.at(tab_name);
				}

				_tab_pane._p_tabs.try_emplace(tab_name, tp._d._page._d_page._fm, tab_name);
				_tab_pane._tab_order.push_back(tab_name);
				auto& page_impl = _tab_pane._p_tabs.at(tab_name)._d_page;

				// specify direct2d factory (used internally for geometries and stuff)
				page_impl.direct2d_factory(tp._d._page._d_page.direct2d_factory());

				// specify directwrite factory (used internally for text rendering)
				page_impl.directwrite_factory(tp._d._page._d_page.directwrite_factory());

				// specify iwic imaging factory (used internally for image rendering)
				page_impl.iwic_factory(tp._d._page._d_page.iwic_factory());

				// specify parent
				page_impl.parent(tp._d._page);

				const float thickness = 10.f;	// thickness of scroll bars
				const float _content_margin = _tab_pane.content_margin();
				float _tab_height = _tab_pane.tab_height();
				rect rect_client_area = _tab_pane().rect();

				bool tabs_perpendicular = false;

				switch (tp._d._specs.tab_side()) {
				case tab_pane_specs::side::left:
				case tab_pane_specs::side::right:
					if (tp._d._specs.caption_orientation() == tab_pane_specs::orientation::horizontal)
						tabs_perpendicular = true;
					break;

				case tab_pane_specs::side::top:
				case tab_pane_specs::side::bottom:
				default: 
					if (tp._d._specs.caption_orientation() == tab_pane_specs::orientation::vertical)
						tabs_perpendicular = true;
					break;
				}

				if (tabs_perpendicular && !_tab_pane.tab_height_set()) {
					if (!_tab_pane.specs().caption_reserve().empty()) {
						log(tab_name + ": using caption reserve to compute dimensions");
						// compute the longest tab caption in the reserve
						for (const auto& alias : _tab_pane.specs().caption_reserve()) {
							D2D1_RECT_F max_rect = { 0.f, 0.f, _tab_pane.specs().rect().height(), _tab_pane.caption_bar_height() };
							auto caption_rect = widgets::measure_label(tp._d._page._d_page._p_directwrite_factory, alias, _tab_pane.specs().font(),
								_tab_pane.specs().font_size(), false, true, max_rect);

							_tab_height = largest(_tab_height, caption_rect.right - caption_rect.left + 3.f * _tab_pane.padding());
						}
					}
					else {
						log(tab_name + ": WARNING - no caption reserve for perpendicular tab captions!");
						// use current caption to set tab height
						D2D1_RECT_F max_rect = { 0.f, 0.f, _tab_pane.specs().rect().height(), _tab_pane.caption_bar_height() };
						auto caption_rect = widgets::measure_label(tp._d._page._d_page._p_directwrite_factory, tab_name, _tab_pane.specs().font(),
							_tab_pane.specs().font_size(), false, true, max_rect);

						_tab_height = largest(_tab_height, caption_rect.right - caption_rect.left + 3.f * _tab_pane.padding());
					}

					_tab_pane.set_tab_height(_tab_height);
				}

				const auto _caption_bar_height = _tab_pane.caption_bar_height();

				// set page size
				page_impl.size({ rect_client_area.width(), rect_client_area.height() });

				switch (tp._d._specs.tab_side()) {
				case tab_pane_specs::side::left:
				case tab_pane_specs::side::right:
					page_impl.width(page_impl.width() - (2.f * _content_margin + _caption_bar_height));
					page_impl.height(page_impl.height() - (2.f * _content_margin));
					break;

				case tab_pane_specs::side::top:
				case tab_pane_specs::side::bottom:
				default:
					page_impl.width(page_impl.width() - (2.f * _content_margin));
					page_impl.height(page_impl.height() - (2.f * _content_margin + _caption_bar_height));
					break;
				}

				// add an invisible rect to bound the page. This is essential for scroll bars
				// to work appropriately when contents don't reach the page borders
				auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
				rectangle.color_fill().alpha(0);

				// make it transparent
				rectangle
					.color_border({ 255, 0, 0, 0 })
					.color_hot({ 255, 0, 0, 0 })
					.corner_radius_x(15.f)
					.corner_radius_y(15.f)

					// set its dimensions to exactly match the page
					.rect().size(page_impl.size());

				rectangle.on_resize()
					.perc_width(100.f)
					.perc_height(100.f);

				// capture pointer to pane rect and page
				_tab_pane._p_page_impl = &page_impl;
				_tab_pane._p_tab_pane_rect = &rectangle.rect();

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

					switch (tp._d._specs.tab_side()) {
					case tab_pane_specs::side::left:
					case tab_pane_specs::side::right:
						width -= _caption_bar_height;
						break;

					case tab_pane_specs::side::top:
					case tab_pane_specs::side::bottom:
					default:
						break;
					}

					if (thickness > _content_margin)
						width -= (thickness - _content_margin);

					_specs.rect()
						.width(width)
						.height(thickness)
						.place(ref_rect, 50.f, 100.f);

					_specs
						.color_fill(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar))
						.color_scrollbar_border(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_border))
						.color_hot(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_hover))
						.color_hot_pressed(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_pressed));
				}

				// initialize the page's vertical scroll bar
				{
					auto& _specs = page_impl.v_scrollbar().specs();
					_specs.on_resize()
						.perc_height(100.f)
						.perc_x(100.f);

					auto height = rect_client_area.bottom() - rect_client_area.top() - 2.f * _content_margin;

					switch (tp._d._specs.tab_side()) {
					case tab_pane_specs::side::left:
					case tab_pane_specs::side::right:
						break;

					case tab_pane_specs::side::top:
					case tab_pane_specs::side::bottom:
					default:
						height -= _caption_bar_height;
						break;
					}

					if (thickness > _content_margin)
						height -= (thickness - _content_margin);

					_specs.rect()
						.width(thickness)
						.height(height)
						.place(ref_rect, 100.f, 50.f);

					_specs
						.color_fill(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar))
						.color_scrollbar_border(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_border))
						.color_hot(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_hover))
						.color_hot_pressed(defaults::color(tp._d._page._d_page._fm._d._theme, item::scrollbar_pressed));
				}

				// return reference to page so caller can add widgets to it
				return _tab_pane._p_tabs.at(tab_name);
			}
		};

		containers::tab_builder::tab_builder(containers::tab_pane_builder& tp, const std::string& tab_name) :
			_d(*(new impl(tp, tab_name))) {}

		containers::tab_builder::~tab_builder() { delete& _d; }

		containers::page& containers::tab_builder::get() {
			return _d._page;
		}

		containers::page&
			containers::tab_builder::get(form& fm, const std::string& path) {
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

		std::string& containers::tab_pane_specs::text() { return _text; }

		containers::tab_pane_specs& containers::tab_pane_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::tab_pane_specs::tooltip() { return _tooltip; }

		containers::tab_pane_specs& containers::tab_pane_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& containers::tab_pane_specs::rect() { return _rect; }

		containers::tab_pane_specs& containers::tab_pane_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& containers::tab_pane_specs::on_resize() { return _on_resize; }

		containers::tab_pane_specs& containers::tab_pane_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::tab_pane_specs::cursor() { return _cursor; }

		containers::tab_pane_specs& containers::tab_pane_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::tab_pane_specs::font() { return _font; }

		containers::tab_pane_specs& containers::tab_pane_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::tab_pane_specs::font_size() { return _font_size; }

		containers::tab_pane_specs& containers::tab_pane_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::tab_pane_specs::color_text() { return _color_text; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::tab_pane_specs::color_fill() { return _color_fill; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::tab_pane_specs::color_hot() { return _color_hot; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::tab_pane_specs::color_selected() { return _color_selected; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::tab_pane_specs::color_disabled() { return _color_disabled; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::tab_pane_specs::border() { return _border; }

		containers::tab_pane_specs& containers::tab_pane_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_border() { return _color_border; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::tab_pane_specs::corner_radius_x() { return _corner_radius_x; }

		containers::tab_pane_specs& containers::tab_pane_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::tab_pane_specs::corner_radius_y() { return _corner_radius_y; }

		containers::tab_pane_specs& containers::tab_pane_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		containers::tab_pane_specs::side& containers::tab_pane_specs::tab_side() { return _tab_side; }

		containers::tab_pane_specs& containers::tab_pane_specs::tab_side(const side& tab_side) {
			_tab_side = tab_side;
			return *this;
		}

		std::vector<std::string>& containers::tab_pane_specs::caption_reserve() { return _caption_reserve; }

		containers::tab_pane_specs& containers::tab_pane_specs::caption_reserve(const std::vector<std::string>& caption_reserve) {
			_caption_reserve = caption_reserve;
			return *this;
		}

		containers::tab_pane_specs::orientation& containers::tab_pane_specs::caption_orientation() { return _caption_orientation; }

		containers::tab_pane_specs& containers::tab_pane_specs::caption_orientation(const orientation& caption_orientation) {
			_caption_orientation = caption_orientation;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_tabs() { return _color_tabs; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_tabs(const color& color_tabs) {
			_color_tabs = color_tabs;
			return *this;
		}

		lecui::color& containers::tab_pane_specs::color_tabs_border() { return _color_tabs_border; }

		containers::tab_pane_specs& containers::tab_pane_specs::color_tabs_border(const color& color_tabs_border) {
			_color_tabs_border = color_tabs_border;
			return *this;
		}

		float& containers::tab_pane_specs::tabs_border() { return _tabs_border; }

		containers::tab_pane_specs& containers::tab_pane_specs::tabs_border(const float& tabs_border) {
			_tabs_border = tabs_border;
			return *this;
		}
	}
}
