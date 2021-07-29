//
// rectangle.cpp - rectangle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../rectangle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::rectangle::operator==(const rectangle& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_border_hot == param._color_border_hot);
		}

		bool widgets::rectangle::operator!=(const rectangle& param) {
			return !operator==(param);
		}

		std::string& widgets::rectangle::text() { return _text; }

		widgets::rectangle& widgets::rectangle::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::rectangle::tooltip() { return _tooltip; }

		widgets::rectangle& widgets::rectangle::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::rectangle::rect() { return _rect; }

		widgets::rectangle& widgets::rectangle::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::rectangle::on_resize() { return _on_resize; }

		widgets::rectangle& widgets::rectangle::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::rectangle::cursor() { return _cursor; }

		widgets::rectangle& widgets::rectangle::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::rectangle::font() { return _font; }

		widgets::rectangle& widgets::rectangle::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::rectangle::font_size() { return _font_size; }

		widgets::rectangle& widgets::rectangle::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::rectangle::color_text() { return _color_text; }

		widgets::rectangle& widgets::rectangle::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::rectangle::color_fill() { return _color_fill; }

		widgets::rectangle& widgets::rectangle::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::rectangle::color_hot() { return _color_hot; }

		widgets::rectangle& widgets::rectangle::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::rectangle::color_selected() { return _color_selected; }

		widgets::rectangle& widgets::rectangle::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::rectangle::color_disabled() { return _color_disabled; }

		widgets::rectangle& widgets::rectangle::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::rectangle::border() { return _border; }

		widgets::rectangle& widgets::rectangle::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::rectangle::color_border() { return _color_border; }

		widgets::rectangle& widgets::rectangle::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		lecui::color& widgets::rectangle::color_border_hot() { return _color_border_hot; }

		widgets::rectangle& widgets::rectangle::color_border_hot(const color& color_border_hot) {
			_color_border_hot = color_border_hot;
			return *this;
		}

		float& widgets::rectangle::corner_radius_x() { return _corner_radius_x; }

		widgets::rectangle& widgets::rectangle::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::rectangle::corner_radius_y() { return _corner_radius_y; }

		widgets::rectangle& widgets::rectangle::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		namespace widgets {
			/// <summary>Rectangle widget builder.</summary>
			class rectangle_builder {
			public:
				/// <summary>Rectangle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				rectangle_builder(containers::page& page);

				/// <summary>Rectangle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "border_rect".</param>
				rectangle_builder(containers::page& page, const std::string& alias);
				~rectangle_builder();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				[[nodiscard]]
				rectangle& specs();

				/// <summary>Get the rectangle specifications.</summary>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				rectangle& operator()();

				/// <summary>Get the specifications of a rectangle.</summary>
				/// <param name="fm">The form containing the rectangle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_three/border_rect".</param>
				/// <returns>A reference to the rectangle specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static rectangle& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				rectangle_builder() = delete;
				rectangle_builder(const rectangle_builder&) = delete;
				rectangle_builder& operator=(const rectangle_builder&) = delete;
			};
		}

		class widgets::rectangle_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_rectangle(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::rectangle))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::rectangle_border))
					.color_border_hot(defaults::color(_page._d_page._fm._d._theme, item::rectangle_border_hot))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::rectangle_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::rectangle_selected));

			}
			containers::page& _page;
			rectangle& _specs;
		};

		widgets::rectangle_builder::rectangle_builder(containers::page& page) :
			rectangle_builder(page, "") {}

		widgets::rectangle_builder::rectangle_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::rectangle_builder::~rectangle_builder() { delete& _d; }

		widgets::rectangle&
			widgets::rectangle_builder::specs() {
			return _d._specs;
		}

		widgets::rectangle&
			widgets::rectangle_builder::operator()() {
			return specs();
		}

		widgets::rectangle&
			widgets::rectangle_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_rectangle_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_rectangle_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::rectangle& liblec::lecui::widgets::rectangle::add(containers::page& page, const std::string& alias) {
			return rectangle_builder(page, alias).specs();
		}

		widgets::rectangle& liblec::lecui::widgets::rectangle::get(form& fm, const std::string& path) {
			return rectangle_builder::specs(fm, path);
		}
	}
}
