//
// label.cpp - label widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../label.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::label_specs::operator==(const label_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_text == param._text) &&
				(_color_hot_pressed == param._color_hot_pressed) &&
				(_multiline == param._multiline) &&
				(_center_h == param._center_h) &&
				(_center_v == param._center_v);
		}

		bool widgets::label_specs::operator!=(const label_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::label_specs::text() { return _text; }

		widgets::label_specs& widgets::label_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::label_specs::tooltip() { return _tooltip; }

		widgets::label_specs& widgets::label_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::label_specs::rect() { return _rect; }

		widgets::label_specs& widgets::label_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::label_specs::on_resize() { return _on_resize; }

		widgets::label_specs& widgets::label_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::label_specs::cursor() { return _cursor; }

		widgets::label_specs& widgets::label_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::label_specs::font() { return _font; }

		widgets::label_specs& widgets::label_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::label_specs::font_size() { return _font_size; }

		widgets::label_specs& widgets::label_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::label_specs::color_text() { return _color_text; }

		widgets::label_specs& widgets::label_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::label_specs::color_fill() { return _color_fill; }

		widgets::label_specs& widgets::label_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::label_specs::color_hot() { return _color_hot; }

		widgets::label_specs& widgets::label_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::label_specs::color_selected() { return _color_selected; }

		widgets::label_specs& widgets::label_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::label_specs::color_disabled() { return _color_disabled; }

		widgets::label_specs& widgets::label_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		lecui::color& widgets::label_specs::color_hot_pressed() { return _color_hot_pressed; }

		widgets::label_specs& widgets::label_specs::color_hot_pressed(const color& color_hot_pressed) {
			_color_hot_pressed = color_hot_pressed;
			return *this;
		}

		bool& widgets::label_specs::multiline() { return _multiline; }

		widgets::label_specs& widgets::label_specs::multiline(const bool& multiline) {
			_multiline = multiline;
			return *this;
		}

		bool& widgets::label_specs::center_h() { return _center_h; }

		widgets::label_specs& widgets::label_specs::center_h(const bool& center_h) {
			_center_h = center_h;
			return *this;
		}

		bool& widgets::label_specs::center_v() { return _center_v; }

		widgets::label_specs& widgets::label_specs::center_v(const bool& center_v) {
			_center_v = center_v;
			return *this;
		}

		class widgets::label_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_label(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::label_selected))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::label_hover))
					.color_hot_pressed(defaults::color(_page._d_page._fm._d._theme, item::label_pressed))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::label_disabled));
			}
			containers::page& _page;
			label_specs& _specs;
		};

		widgets::label_builder::label_builder(containers::page& page) :
			label_builder(page, "") {}

		widgets::label_builder::label_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::label_builder::~label_builder() { delete& _d; }

		widgets::label_specs&
			widgets::label_builder::specs() {
			return _d._specs;
		}

		widgets::label_specs&
			widgets::label_builder::operator()() {
			return specs();
		}

		widgets::label_specs&
			widgets::label_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_label(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
