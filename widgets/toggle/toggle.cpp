//
// toggle.cpp - toggle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../toggle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::toggle_specs::operator==(const toggle_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_on == param._color_on) &&
				(_color_off == param._color_off);
		}

		bool widgets::toggle_specs::operator!=(const toggle_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::toggle_specs::text() { return _text; }

		widgets::toggle_specs& widgets::toggle_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::toggle_specs::tooltip() { return _tooltip; }

		widgets::toggle_specs& widgets::toggle_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::toggle_specs::rect() { return _rect; }

		widgets::toggle_specs& widgets::toggle_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::toggle_specs::on_resize() { return _on_resize; }

		widgets::toggle_specs& widgets::toggle_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::toggle_specs::cursor() { return _cursor; }

		widgets::toggle_specs& widgets::toggle_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::toggle_specs::font() { return _font; }

		widgets::toggle_specs& widgets::toggle_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::toggle_specs::font_size() { return _font_size; }

		widgets::toggle_specs& widgets::toggle_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::toggle_specs::color_text() { return _color_text; }

		widgets::toggle_specs& widgets::toggle_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::toggle_specs::color_fill() { return _color_fill; }

		widgets::toggle_specs& widgets::toggle_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::toggle_specs::color_hot() { return _color_hot; }

		widgets::toggle_specs& widgets::toggle_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::toggle_specs::color_selected() { return _color_selected; }

		widgets::toggle_specs& widgets::toggle_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::toggle_specs::color_disabled() { return _color_disabled; }

		widgets::toggle_specs& widgets::toggle_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		std::string& widgets::toggle_specs::text_off() { return _text_off; }

		widgets::toggle_specs& widgets::toggle_specs::text_off(const std::string& text_off) {
			_text_off = text_off;
			return *this;
		}

		color& widgets::toggle_specs::color_on() { return _color_on; }

		widgets::toggle_specs& widgets::toggle_specs::color_on(const color& color_on) {
			_color_on = color_on;
			return *this;
		}

		lecui::color& widgets::toggle_specs::color_off() { return _color_off; }

		widgets::toggle_specs& widgets::toggle_specs::color_off(const color& color_off) {
			_color_off = color_off;
			return *this;
		}

		bool& widgets::toggle_specs::on() { return _on; }

		widgets::toggle_specs& widgets::toggle_specs::on(const bool& on) {
			_on = on;
			return *this;
		}

		class widgets::toggle_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_toggle(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::toggle))
					.color_on(defaults::color(_page._d_page._fm._d._theme, item::toggle_on))
					.color_off(defaults::color(_page._d_page._fm._d._theme, item::toggle_off))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::toggle_selected));
			}
			containers::page& _page;
			toggle_specs& _specs;
		};

		widgets::toggle_builder::toggle_builder(containers::page& page) :
			toggle_builder(page, "") {}

		widgets::toggle_builder::toggle_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::toggle_builder::~toggle_builder() { delete& _d; }

		widgets::toggle_specs&
			widgets::toggle_builder::specs() {
			return _d._specs;
		}

		widgets::toggle_specs&
			widgets::toggle_builder::operator()() {
			return specs();
		}

		widgets::toggle_specs&
			widgets::toggle_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_toggle(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
