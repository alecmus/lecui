//
// line.cpp - line widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../line.h"

#ifdef get_line
// prevent conflict with msxml.h
#undef get_line
#endif

#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::line::operator==(const line& param) {
			return
				// generic widget
				widget::operator==(param);
		}

		bool widgets::line::operator!=(const line& param) {
			return !operator==(param);
		}

		std::string& widgets::line::text() { return _text; }

		widgets::line& widgets::line::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::line::tooltip() { return _tooltip; }

		widgets::line& widgets::line::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::line::badge() { return _badge; }

		widgets::line& widgets::line::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::line::rect() { return _rect; }

		widgets::line& widgets::line::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::line::on_resize() { return _on_resize; }

		widgets::line& widgets::line::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::line::cursor() { return _cursor; }

		widgets::line& widgets::line::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::line::font() { return _font; }

		widgets::line& widgets::line::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::line::font_size() { return _font_size; }

		widgets::line& widgets::line::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::line::color_text() { return _color_text; }

		widgets::line& widgets::line::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::line::color_fill() { return _color_fill; }

		widgets::line& widgets::line::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::line::color_hot() { return _color_hot; }

		widgets::line& widgets::line::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::line::color_selected() { return _color_selected; }

		widgets::line& widgets::line::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::line::color_disabled() { return _color_disabled; }

		widgets::line& widgets::line::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::line::thickness() { return _thickness; }

		widgets::line& widgets::line::thickness(const float& thickness) {
			_thickness = thickness;
			return *this;
		}

		std::vector<point>& widgets::line::points() { return _points; }

		widgets::line& widgets::line::points(const std::vector<point>& points) {
			_points = points;
			return *this;
		}

		namespace widgets {
			/// <summary>Line widget builder.</summary>
			class line_builder {
			public:
				/// <summary>Line builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				line_builder(containers::page& page);

				/// <summary>Line builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_line".</param>
				line_builder(containers::page& page, const std::string& alias);
				~line_builder();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				[[nodiscard]]
				line& specs();

				/// <summary>Get the line specifications.</summary>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				line& operator()();

				/// <summary>Get the specifications of a line.</summary>
				/// <param name="fm">The form containing the line.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_line".</param>
				/// <returns>A reference to the line specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static line& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				line_builder() = delete;
				line_builder(const line_builder&) = delete;
				line_builder& operator=(const line_builder&) = delete;
			};
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
			line& _specs;
		};

		widgets::line_builder::line_builder(containers::page& page) :
			line_builder(page, "") {}

		widgets::line_builder::line_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::line_builder::~line_builder() { delete& _d; }

		widgets::line&
			widgets::line_builder::specs() {
			return _d._specs;
		}

		widgets::line&
			widgets::line_builder::operator()() {
			return specs();
		}

		widgets::line&
			widgets::line_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_line_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_line_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::line& liblec::lecui::widgets::line::add(containers::page& page, const std::string& alias) {
			return line_builder(page, alias).specs();
		}

		widgets::line& liblec::lecui::widgets::line::get(form& fm, const std::string& path) {
			return line_builder::specs(fm, path);
		}
	}
}
