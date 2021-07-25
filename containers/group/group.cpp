//
// group.cpp - group container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../group.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool containers::group_specs::operator==(const group_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border);
		}

		bool containers::group_specs::operator!=(const group_specs& param) {
			return !operator==(param);
		}

		std::string& containers::group_specs::text() { return _text; }

		containers::group_specs& containers::group_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::group_specs::tooltip() { return _tooltip; }

		containers::group_specs& containers::group_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& containers::group_specs::rect() { return _rect; }

		containers::group_specs& containers::group_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& containers::group_specs::on_resize() { return _on_resize; }

		containers::group_specs& containers::group_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& containers::group_specs::cursor() { return _cursor; }

		containers::group_specs& containers::group_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::group_specs::font() { return _font; }

		containers::group_specs& containers::group_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::group_specs::font_size() { return _font_size; }

		containers::group_specs& containers::group_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::group_specs::color_text() { return _color_text; }

		containers::group_specs& containers::group_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::group_specs::color_fill() { return _color_fill; }

		containers::group_specs& containers::group_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::group_specs::color_hot() { return _color_hot; }

		containers::group_specs& containers::group_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::group_specs::color_selected() { return _color_selected; }

		containers::group_specs& containers::group_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::group_specs::color_disabled() { return _color_disabled; }

		containers::group_specs& containers::group_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::group_specs::border() { return _border; }

		containers::group_specs& containers::group_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::group_specs::color_border() { return _color_border; }

		containers::group_specs& containers::group_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::group_specs::corner_radius_x() { return _corner_radius_x; }

		containers::group_specs& containers::group_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::group_specs::corner_radius_y() { return _corner_radius_y; }

		containers::group_specs& containers::group_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		float& containers::group_specs::margin() { return _margin; }

		containers::group_specs& containers::group_specs::margin(const float& margin) {
			_margin = margin;
			return *this;
		}

		std::vector<std::string>& containers::group_specs::widgets() { return _widgets; }

		containers::group_specs& containers::group_specs::widgets(const std::vector<std::string>& widgets) {
			_widgets = widgets;
			return *this;
		}

		class containers::group_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_group(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::group))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::group_border));
			}
			containers::page& _page;
			group_specs& _specs;
		};

		containers::group_builder::group_builder(containers::page& page) :
			group_builder(page, "") {}

		containers::group_builder::group_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		containers::group_builder::~group_builder() { delete& _d; }

		containers::group_specs&
			containers::group_builder::specs() {
			return _d._specs;
		}

		containers::group_specs&
			containers::group_builder::operator()() {
			return specs();
		}

		containers::group_specs&
			containers::group_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
