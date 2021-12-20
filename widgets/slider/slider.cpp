//
// slider.cpp - slider widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../slider.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::slider::operator==(const slider& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_tick == param._color_tick) &&
				(_color_knob == param._color_knob) &&
				(_color_knob_hot == param._color_knob_hot) &&
				(_color_knob_border == param._color_knob_border);
		}

		bool widgets::slider::operator!=(const slider& param) {
			return !operator==(param);
		}

		std::string& widgets::slider::text() { return _text; }

		widgets::slider& widgets::slider::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::slider::tooltip() { return _tooltip; }

		widgets::slider& widgets::slider::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::slider::badge() { return _badge; }

		widgets::slider& widgets::slider::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::slider::rect() { return _rect; }

		widgets::slider& widgets::slider::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::slider::on_resize() { return _on_resize; }

		widgets::slider& widgets::slider::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::slider::cursor() { return _cursor; }

		widgets::slider& widgets::slider::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::slider::font() { return _font; }

		widgets::slider& widgets::slider::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::slider::font_size() { return _font_size; }

		widgets::slider& widgets::slider::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::slider::color_text() { return _color_text; }

		widgets::slider& widgets::slider::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::slider::color_fill() { return _color_fill; }

		widgets::slider& widgets::slider::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::slider::color_hot() { return _color_hot; }

		widgets::slider& widgets::slider::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::slider::color_selected() { return _color_selected; }

		widgets::slider& widgets::slider::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::slider::color_disabled() { return _color_disabled; }

		widgets::slider& widgets::slider::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::slider::border() { return _border; }

		widgets::slider& widgets::slider::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::slider::color_border() { return _color_border; }

		widgets::slider& widgets::slider::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		lecui::color& widgets::slider::color_knob() { return _color_knob; }

		widgets::slider& widgets::slider::color_knob(const color& color_knob) {
			_color_knob = color_knob;
			return *this;
		}

		lecui::color& widgets::slider::color_knob_hot() { return _color_knob_hot; }

		widgets::slider& widgets::slider::color_knob_hot(const color& color_knob_hot) {
			_color_knob_hot = color_knob_hot;
			return *this;
		}

		lecui::color& widgets::slider::color_knob_border() { return _color_knob_border; }

		widgets::slider& widgets::slider::color_knob_border(const color& color_knob_border) {
			_color_knob_border = color_knob_border;
			return *this;
		}

		lecui::color& widgets::slider::color_tick() { return _color_tick; }

		widgets::slider& widgets::slider::color_tick(const color& color_tick) {
			_color_tick = color_tick;
			return *this;
		}

		bool& widgets::slider::snap_to_ticks() { return _snap_to_ticks; }

		widgets::slider& widgets::slider::snap_to_ticks(const bool& snap_to_ticks) {
			_snap_to_ticks = snap_to_ticks;
			return *this;
		}

		bool& widgets::slider::show_tick_marks() { return _show_tick_marks; }

		widgets::slider& widgets::slider::show_tick_marks(const bool& show_tick_marks) {
			_show_tick_marks = show_tick_marks;
			return *this;
		}

		bool& widgets::slider::show_tick_labels() { return _show_tick_labels; }

		widgets::slider& widgets::slider::show_tick_labels(const bool& show_tick_labels) {
			_show_tick_labels = show_tick_labels;
			return *this;
		}

		widgets::slider::slider_range& widgets::slider::range() { return _range; }

		widgets::slider& widgets::slider::range(const slider_range& range) {
			_range = range;
			return *this;
		}

		float& widgets::slider::knob_radius() { return _knob_radius; }

		widgets::slider& widgets::slider::knob_radius(const float& knob_radius) {
			_knob_radius = knob_radius;
			return *this;
		}

		float& widgets::slider::slide_thickness() { return _slide_thickness; }

		widgets::slider& widgets::slider::slide_thickness(const float& slide_thickness) {
			_slide_thickness = slide_thickness;
			return *this;
		}

		float& widgets::slider::major_tick_unit() { return _major_tick_unit; }

		widgets::slider& widgets::slider::major_tick_unit(const float& major_tick_unit) {
			_major_tick_unit = major_tick_unit;
			return *this;
		}

		long& widgets::slider::minor_tick_count() { return _minor_tick_count; }

		widgets::slider& widgets::slider::minor_tick_count(const long& minor_tick_count) {
			_minor_tick_count = minor_tick_count;
			return *this;
		}

		float& widgets::slider::value() { return _value; }

		widgets::slider& widgets::slider::value(const float& value) {
			_value = value;
			return *this;
		}

		namespace widgets {
			/// <summary>Slider widget builder.</summary>
			class slider_builder {
			public:
				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				slider_builder(containers::page& page);

				/// <summary>Slider constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "volume".</param>
				slider_builder(containers::page& page, const std::string& alias);
				~slider_builder();

				/// <summary>Get the slider specifications.</summary>
				/// <returns>A reference to the slider specifications.</returns>
				[[nodiscard]]
				slider& specs();

				/// <summary>Get the slider specifications.</summary>
				/// <returns>A reference to the slider specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				slider& operator()();

				/// <summary>Get the specifications of a slider.</summary>
				/// <param name="fm">The form containing the slider.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/tools_tab_pane/options_tab/volume".</param>
				/// <returns>A reference to the slider specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static slider& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				slider_builder() = delete;
				slider_builder(const slider_builder&) = delete;
				slider_builder& operator=(const slider_builder&) = delete;
			};
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
			slider& _specs;
		};

		widgets::slider_builder::slider_builder(containers::page& page) :
			slider_builder(page, "") {}

		widgets::slider_builder::slider_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::slider_builder::~slider_builder() { delete& _d; }

		widgets::slider&
			widgets::slider_builder::specs() {
			return _d._specs;
		}

		widgets::slider&
			widgets::slider_builder::operator()() {
			return specs();
		}

		widgets::slider&
			widgets::slider_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_slider_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_slider_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::slider& liblec::lecui::widgets::slider::add(containers::page& page, const std::string& alias) {
			return slider_builder(page, alias).specs();
		}

		widgets::slider& liblec::lecui::widgets::slider::get(form& fm, const std::string& path) {
			return slider_builder::specs(fm, path);
		}
	}
}
