//
// line.cpp - line widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../line.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::line_specs::operator==(const line_specs& param) {
			return
				// generic specs
				specs::operator==(param);
		}

		bool widgets::line_specs::operator!=(const line_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::line_specs::text() { return _text; }

		widgets::line_specs& widgets::line_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::line_specs::tooltip() { return _tooltip; }

		widgets::line_specs& widgets::line_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::line_specs::rect() { return _rect; }

		widgets::line_specs& widgets::line_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::line_specs::on_resize() { return _on_resize; }

		widgets::line_specs& widgets::line_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::line_specs::cursor() { return _cursor; }

		widgets::line_specs& widgets::line_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::line_specs::font() { return _font; }

		widgets::line_specs& widgets::line_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::line_specs::font_size() { return _font_size; }

		widgets::line_specs& widgets::line_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::line_specs::color_text() { return _color_text; }

		widgets::line_specs& widgets::line_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::line_specs::color_fill() { return _color_fill; }

		widgets::line_specs& widgets::line_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::line_specs::color_hot() { return _color_hot; }

		widgets::line_specs& widgets::line_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::line_specs::color_selected() { return _color_selected; }

		widgets::line_specs& widgets::line_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::line_specs::color_disabled() { return _color_disabled; }

		widgets::line_specs& widgets::line_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::line_specs::thickness() { return _thickness; }

		widgets::line_specs& widgets::line_specs::thickness(const float& thickness) {
			_thickness = thickness;
			return *this;
		}

		std::vector<point>& widgets::line_specs::points() { return _points; }

		widgets::line_specs& widgets::line_specs::points(const std::vector<point>& points) {
			_points = points;
			return *this;
		}

		class widgets::line_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_line(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::line))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::line_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::line_selected));

			}
			containers::page& _page;
			line_specs& _specs;
		};

		widgets::line_builder::line_builder(containers::page& page) :
			line_builder(page, "") {}

		widgets::line_builder::line_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::line_builder::~line_builder() { delete& _d; }

		widgets::line_specs&
			widgets::line_builder::specs() {
			return _d._specs;
		}

		widgets::line_specs&
			widgets::line_builder::operator()() {
			return specs();
		}

		widgets::line_specs&
			widgets::line_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_line(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
