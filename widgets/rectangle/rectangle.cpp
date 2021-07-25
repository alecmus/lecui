//
// rectangle.cpp - rectangle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../rectangle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::rectangle_specs::operator==(const rectangle_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border) &&
				(_color_border_hot == param._color_border_hot);
		}

		bool widgets::rectangle_specs::operator!=(const rectangle_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::rectangle_specs::text() { return _text; }

		widgets::rectangle_specs& widgets::rectangle_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::rectangle_specs::tooltip() { return _tooltip; }

		widgets::rectangle_specs& widgets::rectangle_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::rectangle_specs::rect() { return _rect; }

		widgets::rectangle_specs& widgets::rectangle_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::resize_params& widgets::rectangle_specs::on_resize() { return _on_resize; }

		widgets::rectangle_specs& widgets::rectangle_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::rectangle_specs::cursor() { return _cursor; }

		widgets::rectangle_specs& widgets::rectangle_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::rectangle_specs::font() { return _font; }

		widgets::rectangle_specs& widgets::rectangle_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::rectangle_specs::font_size() { return _font_size; }

		widgets::rectangle_specs& widgets::rectangle_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::rectangle_specs::color_text() { return _color_text; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::rectangle_specs::color_fill() { return _color_fill; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::rectangle_specs::color_hot() { return _color_hot; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::rectangle_specs::color_selected() { return _color_selected; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::rectangle_specs::color_disabled() { return _color_disabled; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::rectangle_specs::border() { return _border; }

		widgets::rectangle_specs& widgets::rectangle_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::rectangle_specs::color_border() { return _color_border; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		lecui::color& widgets::rectangle_specs::color_border_hot() { return _color_border_hot; }

		widgets::rectangle_specs& widgets::rectangle_specs::color_border_hot(const color& color_border_hot) {
			_color_border_hot = color_border_hot;
			return *this;
		}

		float& widgets::rectangle_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::rectangle_specs& widgets::rectangle_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::rectangle_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::rectangle_specs& widgets::rectangle_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		class widgets::rectangle_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_rectangle(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::rectangle))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::rectangle_border))
					.color_border_hot(defaults::color(_page._d_page._fm._d._theme, item::rectangle_border_hot))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::rectangle_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::rectangle_selected));

			}
			containers::page& _page;
			rectangle_specs& _specs;
		};

		widgets::rectangle_builder::rectangle_builder(containers::page& page) :
			rectangle_builder(page, "") {}

		widgets::rectangle_builder::rectangle_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::rectangle_builder::~rectangle_builder() { delete& _d; }

		widgets::rectangle_specs&
			widgets::rectangle_builder::specs() {
			return _d._specs;
		}

		widgets::rectangle_specs&
			widgets::rectangle_builder::operator()() {
			return specs();
		}

		widgets::rectangle_specs&
			widgets::rectangle_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_rectangle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_rectangle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
