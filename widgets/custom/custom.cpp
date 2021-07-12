//
// custom.cpp - custom widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../custom.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::custom_specs::operator==(const custom_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				true;
		}

		bool widgets::custom_specs::operator!=(const custom_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::custom_specs::text() { return _text; }

		widgets::custom_specs& widgets::custom_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::custom_specs::tooltip() { return _tooltip; }

		widgets::custom_specs& widgets::custom_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::custom_specs::rect() { return _rect; }

		widgets::custom_specs& widgets::custom_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::custom_specs::on_resize() { return _on_resize; }

		widgets::custom_specs& widgets::custom_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::custom_specs::cursor() { return _cursor; }

		widgets::custom_specs& widgets::custom_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::custom_specs::font() { return _font; }

		widgets::custom_specs& widgets::custom_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::custom_specs::font_size() { return _font_size; }

		widgets::custom_specs& widgets::custom_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::custom_specs::color_text() { return _color_text; }

		widgets::custom_specs& widgets::custom_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::custom_specs::color_fill() { return _color_fill; }

		widgets::custom_specs& widgets::custom_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::custom_specs::color_hot() { return _color_hot; }

		widgets::custom_specs& widgets::custom_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::custom_specs::color_selected() { return _color_selected; }

		widgets::custom_specs& widgets::custom_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::custom_specs::color_disabled() { return _color_disabled; }

		widgets::custom_specs& widgets::custom_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		class widgets::custom_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_custom(alias)) {}
			containers::page& _page;
			custom_specs& _specs;
		};

		widgets::custom_builder::custom_builder(containers::page& page) :
			custom_builder(page, "") {}

		widgets::custom_builder::custom_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::custom_builder::~custom_builder() { delete& _d; }

		widgets::custom_specs&
			widgets::custom_builder::specs() {
			return _d._specs;
		}

		widgets::custom_specs&
			widgets::custom_builder::operator()() {
			return specs();
		}

		widgets::custom_specs&
			widgets::custom_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_custom(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_custom(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
