//
// progress_indicator.cpp - progress indicator widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../progress_indicator.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::progress_indicator_specs::operator==(const progress_indicator_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_empty == param._color_empty);
		}

		bool widgets::progress_indicator_specs::operator!=(const progress_indicator_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::progress_indicator_specs::text() { return _text; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::progress_indicator_specs::tooltip() { return _tooltip; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::progress_indicator_specs::rect() { return _rect; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::resize_params& widgets::progress_indicator_specs::on_resize() { return _on_resize; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::progress_indicator_specs::cursor() { return _cursor; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::progress_indicator_specs::font() { return _font; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::progress_indicator_specs::font_size() { return _font_size; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_text() { return _color_text; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_fill() { return _color_fill; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_hot() { return _color_hot; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_selected() { return _color_selected; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_disabled() { return _color_disabled; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		color& widgets::progress_indicator_specs::color_empty() { return _color_empty; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::color_empty(const color& color_empty) {
			_color_empty = color_empty;
			return *this;
		}

		float& widgets::progress_indicator_specs::line_thickness_empty() { return _line_thickness_empty; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::line_thickness_empty(const float& line_thickness_empty) {
			_line_thickness_empty = line_thickness_empty;
			return *this;
		}

		float& widgets::progress_indicator_specs::line_thickness_fill() { return _line_thickness_fill; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::line_thickness_fill(const float& line_thickness_fill) {
			_line_thickness_fill = line_thickness_fill;
			return *this;
		}

		float& widgets::progress_indicator_specs::percentage() { return _percentage; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::percentage(const float& percentage) {
			_percentage = percentage;
			return *this;
		}

		int& widgets::progress_indicator_specs::precision() { return _precision; }

		widgets::progress_indicator_specs& widgets::progress_indicator_specs::precision(const int& precision) {
			_precision = precision;
			return *this;
		}

		class widgets::progress_indicator_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_progress_indicator(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator))
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator_text))
					.color_empty(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator_unfilled));
			}
			containers::page& _page;
			progress_indicator_specs& _specs;
		};

		widgets::progress_indicator_builder::progress_indicator_builder(containers::page& page) :
			progress_indicator_builder(page, "") {}

		widgets::progress_indicator_builder::progress_indicator_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::progress_indicator_builder::~progress_indicator_builder() { delete& _d; }

		widgets::progress_indicator_specs&
			widgets::progress_indicator_builder::specs() {
			return _d._specs;
		}

		widgets::progress_indicator_specs&
			widgets::progress_indicator_builder::operator()() {
			return specs();
		}

		widgets::progress_indicator_specs&
			widgets::progress_indicator_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_progress_indicator(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_progress_indicator(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
