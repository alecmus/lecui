//
// date.cpp - date widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../date.h"
#include "../../form_impl/form_impl.h"
#include "../../utilities/date_time.h"

namespace liblec {
	namespace lecui {
		bool widgets::date::operator==(const date& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border);
		}

		bool widgets::date::operator!=(const date& param) {
			return !operator==(param);
		}

		std::string& widgets::date::text() { return _text; }

		widgets::date& widgets::date::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::date::tooltip() { return _tooltip; }

		widgets::date& widgets::date::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::date::badge() { return _badge; }

		widgets::date& widgets::date::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::date::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::date& widgets::date::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		resize_params& widgets::date::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::date& widgets::date::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::widget::cursor_type& widgets::date::cursor() { return _cursor; }

		widgets::date& widgets::date::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::date::font() { return _font; }

		widgets::date& widgets::date::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::date::font_size() { return _font_size; }

		widgets::date& widgets::date::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::date::color_text() { return _color_text; }

		widgets::date& widgets::date::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::date::color_fill() { return _color_fill; }

		widgets::date& widgets::date::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::date::color_hot() { return _color_hot; }

		widgets::date& widgets::date::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::date::color_selected() { return _color_selected; }

		widgets::date& widgets::date::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::date::color_disabled() { return _color_disabled; }

		widgets::date& widgets::date::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::date::border() { return _border; }

		widgets::date& widgets::date::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::date::color_border() { return _color_border; }

		widgets::date& widgets::date::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::date::corner_radius_x() { return _corner_radius_x; }

		widgets::date& widgets::date::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::date::corner_radius_y() { return _corner_radius_y; }

		widgets::date& widgets::date::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		lecui::date& widgets::date::date_value() { return _date_value; }

		widgets::date& widgets::date::date_value(const lecui::date& date_value) {
			_date_value = date_value;
			return *this;
		}

		namespace widgets {
			/// <summary>Date widget builder.</summary>
			class date_builder {
			public:
				/// <summary>Date builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				date_builder(containers::page& page);

				/// <summary>Date builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "last_saved".</param>
				date_builder(containers::page& page, const std::string& alias);
				~date_builder();

				/// <summary>Get the date specifications.</summary>
				/// <returns>A reference to the date specifications.</returns>
				[[nodiscard]]
				date& specs();

				/// <summary>Get the date specifications.</summary>
				/// <returns>A reference to the date specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				date& operator()();

				/// <summary>Get the specifications of a date.</summary>
				/// <param name="fm">The form containing the date.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/last_saved".</param>
				/// <returns>A reference to the date specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static date& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				date_builder() = delete;
				date_builder(const date&) = delete;
				date_builder& operator=(const date&) = delete;
			};
		}

		class widgets::date_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_date(alias)) {
				_specs
					.date_value(date_time::today())
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::text_field))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::text_field_border))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::text_field_disabled))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::text_field_selected));
			}
			containers::page& _page;
			date& _specs;
		};

		widgets::date_builder::date_builder(containers::page& page) :
			date_builder(page, "") {}

		widgets::date_builder::date_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::date_builder::~date_builder() { delete& _d; }

		widgets::date&
			widgets::date_builder::specs() {
			return _d._specs;
		}

		widgets::date&
			widgets::date_builder::operator()() {
			return specs();
		}

		widgets::date&
			widgets::date_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_date_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_date_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::date& liblec::lecui::widgets::date::add(containers::page& page, const std::string& alias) {
			return date_builder(page, alias).specs();
		}

		widgets::date& liblec::lecui::widgets::date::get(form& fm, const std::string& path) {
			return date_builder::specs(fm, path);
		}
	}
}
