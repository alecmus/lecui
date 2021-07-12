//
// time.cpp - time widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../time.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::time_specs::operator==(const time_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border);
		}

		bool widgets::time_specs::operator!=(const time_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::time_specs::text() { return _text; }

		widgets::time_specs& widgets::time_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::time_specs::tooltip() { return _tooltip; }

		widgets::time_specs& widgets::time_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::time_specs::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::time_specs& widgets::time_specs::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::time_specs::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::time_specs& widgets::time_specs::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::time_specs::cursor() { return _cursor; }

		widgets::time_specs& widgets::time_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::time_specs::font() { return _font; }

		widgets::time_specs& widgets::time_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::time_specs::font_size() { return _font_size; }

		widgets::time_specs& widgets::time_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::time_specs::color_text() { return _color_text; }

		widgets::time_specs& widgets::time_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::time_specs::color_fill() { return _color_fill; }

		widgets::time_specs& widgets::time_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::time_specs::color_hot() { return _color_hot; }

		widgets::time_specs& widgets::time_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::time_specs::color_selected() { return _color_selected; }

		widgets::time_specs& widgets::time_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::time_specs::color_disabled() { return _color_disabled; }

		widgets::time_specs& widgets::time_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::time_specs::border() { return _border; }

		widgets::time_specs& widgets::time_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::time_specs::color_border() { return _color_border; }

		widgets::time_specs& widgets::time_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::time_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::time_specs& widgets::time_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::time_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::time_specs& widgets::time_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		lecui::time& widgets::time_specs::time_value() { return _time_value; }

		widgets::time_specs& widgets::time_specs::time_value(const lecui::time& time_value) {
			_time_value = time_value;
			return *this;
		}

		class widgets::time_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_time(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::text_field))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::text_field_border))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::text_field_disabled))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::text_field_selected));
			}
			containers::page& _page;
			time_specs& _specs;
		};

		widgets::time_builder::time_builder(containers::page& page) :
			time_builder(page, "") {}

		widgets::time_builder::time_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::time_builder::~time_builder() { delete& _d; }

		widgets::time_specs&
			widgets::time_builder::specs() {
			return _d._specs;
		}

		widgets::time_specs&
			widgets::time_builder::operator()() {
			return specs();
		}

		widgets::time_specs&
			widgets::time_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_time(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_time(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
