//
// slider.cpp - slider widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../slider.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::slider_specs::operator==(const slider_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border) &&
				(_color_tick == param._color_tick) &&
				(_color_knob == param._color_knob) &&
				(_color_knob_hot == param._color_knob_hot) &&
				(_color_knob_border == param._color_knob_border);
		}

		bool widgets::slider_specs::operator!=(const slider_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::slider_specs::text() { return _text; }

		widgets::slider_specs& widgets::slider_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::slider_specs::tooltip() { return _tooltip; }

		widgets::slider_specs& widgets::slider_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::slider_specs::rect() { return _rect; }

		widgets::slider_specs& widgets::slider_specs::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::slider_specs::on_resize() { return _on_resize; }

		widgets::slider_specs& widgets::slider_specs::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::slider_specs::cursor() { return _cursor; }

		widgets::slider_specs& widgets::slider_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::slider_specs::font() { return _font; }

		widgets::slider_specs& widgets::slider_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::slider_specs::font_size() { return _font_size; }

		widgets::slider_specs& widgets::slider_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::slider_specs::color_text() { return _color_text; }

		widgets::slider_specs& widgets::slider_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::slider_specs::color_fill() { return _color_fill; }

		widgets::slider_specs& widgets::slider_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::slider_specs::color_hot() { return _color_hot; }

		widgets::slider_specs& widgets::slider_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::slider_specs::color_selected() { return _color_selected; }

		widgets::slider_specs& widgets::slider_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::slider_specs::color_disabled() { return _color_disabled; }

		widgets::slider_specs& widgets::slider_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::slider_specs::border() { return _border; }

		widgets::slider_specs& widgets::slider_specs::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_border() { return _color_border; }

		widgets::slider_specs& widgets::slider_specs::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob() { return _color_knob; }

		widgets::slider_specs& widgets::slider_specs::color_knob(const color& color_knob) {
			_color_knob = color_knob;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob_hot() { return _color_knob_hot; }

		widgets::slider_specs& widgets::slider_specs::color_knob_hot(const color& color_knob_hot) {
			_color_knob_hot = color_knob_hot;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob_border() { return _color_knob_border; }

		widgets::slider_specs& widgets::slider_specs::color_knob_border(const color& color_knob_border) {
			_color_knob_border = color_knob_border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_tick() { return _color_tick; }

		widgets::slider_specs& widgets::slider_specs::color_tick(const color& color_tick) {
			_color_tick = color_tick;
			return *this;
		}

		bool& widgets::slider_specs::snap_to_ticks() { return _snap_to_ticks; }

		widgets::slider_specs& widgets::slider_specs::snap_to_ticks(const bool& snap_to_ticks) {
			_snap_to_ticks = snap_to_ticks;
			return *this;
		}

		bool& widgets::slider_specs::show_tick_marks() { return _show_tick_marks; }

		widgets::slider_specs& widgets::slider_specs::show_tick_marks(const bool& show_tick_marks) {
			_show_tick_marks = show_tick_marks;
			return *this;
		}

		bool& widgets::slider_specs::show_tick_labels() { return _show_tick_labels; }

		widgets::slider_specs& widgets::slider_specs::show_tick_labels(const bool& show_tick_labels) {
			_show_tick_labels = show_tick_labels;
			return *this;
		}

		widgets::slider_specs::slider_range& widgets::slider_specs::range() { return _range; }

		widgets::slider_specs& widgets::slider_specs::range(const slider_range& range) {
			_range = range;
			return *this;
		}

		float& widgets::slider_specs::knob_radius() { return _knob_radius; }

		widgets::slider_specs& widgets::slider_specs::knob_radius(const float& knob_radius) {
			_knob_radius = knob_radius;
			return *this;
		}

		float& widgets::slider_specs::slide_thickness() { return _slide_thickness; }

		widgets::slider_specs& widgets::slider_specs::slide_thickness(const float& slide_thickness) {
			_slide_thickness = slide_thickness;
			return *this;
		}

		float& widgets::slider_specs::major_tick_unit() { return _major_tick_unit; }

		widgets::slider_specs& widgets::slider_specs::major_tick_unit(const float& major_tick_unit) {
			_major_tick_unit = major_tick_unit;
			return *this;
		}

		long& widgets::slider_specs::minor_tick_count() { return _minor_tick_count; }

		widgets::slider_specs& widgets::slider_specs::minor_tick_count(const long& minor_tick_count) {
			_minor_tick_count = minor_tick_count;
			return *this;
		}

		float& widgets::slider_specs::value() { return _value; }

		widgets::slider_specs& widgets::slider_specs::value(const float& value) {
			_value = value;
			return *this;
		}

		class widgets::slider_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_slider(alias)) {
				_specs
					.font_size(7.f)
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::slider))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::slider_border))
					.color_knob(defaults::color(_page._d_page._fm._d._theme, item::slider_knob))
					.color_knob_hot(defaults::color(_page._d_page._fm._d._theme, item::slider_knob_hot))
					.color_knob_border(defaults::color(_page._d_page._fm._d._theme, item::slider_knob_border))
					.color_tick(defaults::color(_page._d_page._fm._d._theme, item::slider_tick))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::slider_selected));
			}
			containers::page& _page;
			slider_specs& _specs;
		};

		widgets::slider_builder::slider_builder(containers::page& page) :
			slider_builder(page, "") {}

		widgets::slider_builder::slider_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::slider_builder::~slider_builder() { delete& _d; }

		widgets::slider_specs&
			widgets::slider_builder::specs() {
			return _d._specs;
		}

		widgets::slider_specs&
			widgets::slider_builder::operator()() {
			return specs();
		}

		widgets::slider_specs&
			widgets::slider_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
