//
// checkbox.cpp - checkbox widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../checkbox.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::checkbox_specs::operator==(const checkbox_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border) &&
				(_color_check == param._color_check);
		}

		bool widgets::checkbox_specs::operator!=(const checkbox_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::checkbox_specs::text() { return _text; }

		widgets::checkbox_specs& widgets::checkbox_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::checkbox_specs::tooltip() { return _tooltip; }

		widgets::checkbox_specs& widgets::checkbox_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::checkbox_specs::rect() { return _rect; }

		widgets::checkbox_specs& widgets::checkbox_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::checkbox_specs::on_resize() { return _on_resize; }

		widgets::checkbox_specs& widgets::checkbox_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::checkbox_specs::cursor() { return _cursor; }

		widgets::checkbox_specs& widgets::checkbox_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::checkbox_specs::font() { return _font; }

		widgets::checkbox_specs& widgets::checkbox_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::checkbox_specs::font_size() { return _font_size; }

		widgets::checkbox_specs& widgets::checkbox_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::checkbox_specs::color_text() { return _color_text; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::checkbox_specs::color_fill() { return _color_fill; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::checkbox_specs::color_hot() { return _color_hot; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::checkbox_specs::color_selected() { return _color_selected; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::checkbox_specs::color_disabled() { return _color_disabled; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::checkbox_specs::border() { return _border; }

		widgets::checkbox_specs& widgets::checkbox_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::checkbox_specs::color_border() { return _color_border; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		std::string& widgets::checkbox_specs::text_unchecked() { return _text_unchecked; }

		widgets::checkbox_specs& widgets::checkbox_specs::text_unchecked(const std::string& text_unchecked) {
			_text_unchecked = text_unchecked;
			return *this;
		}

		std::string& widgets::checkbox_specs::text_indeterminate() { return _text_indeterminate; }

		widgets::checkbox_specs& widgets::checkbox_specs::text_indeterminate(const std::string& text_indeterminate) {
			_text_indeterminate = text_indeterminate;
			return *this;
		}

		lecui::color& widgets::checkbox_specs::color_check() { return _color_check; }

		widgets::checkbox_specs& widgets::checkbox_specs::color_check(const color& color_check) {
			_color_check = color_check;
			return *this;
		}

		widgets::checkbox_specs::checkbox_status& widgets::checkbox_specs::status() { return _status; }

		widgets::checkbox_specs& widgets::checkbox_specs::status(const checkbox_status& status) {
			_status = status;
			return *this;
		}

		class widgets::checkbox_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_checkbox(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::checkbox))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::checkbox_border))
					.color_check(defaults::color(_page._d_page._fm._d._theme, item::checkbox_tick))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::checkbox_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::checkbox_selected));
			}
			containers::page& _page;
			checkbox_specs& _specs;
		};

		widgets::checkbox_builder::checkbox_builder(containers::page& page) :
			checkbox_builder(page, "") {}

		widgets::checkbox_builder::checkbox_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::checkbox_builder::~checkbox_builder() { delete& _d; }

		widgets::checkbox_specs&
			widgets::checkbox_builder::specs() {
			return _d._specs;
		}

		widgets::checkbox_specs&
			widgets::checkbox_builder::operator()() {
			return specs();
		}

		widgets::checkbox_specs&
			widgets::checkbox_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_checkbox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_checkbox(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
