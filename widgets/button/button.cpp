//
// button.cpp - button widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../button.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::button::operator==(const button& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border);
		}

		bool widgets::button::operator!=(const button& param) {
			return !operator==(param);
		}

		std::string& widgets::button::text() { return _text; }

		widgets::button& widgets::button::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::button::tooltip() { return _tooltip; }

		widgets::button& widgets::button::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::badge_specs& widgets::button::badge() { return _badge; }

		widgets::button& widgets::button::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::button::rect() { return _rect; }

		widgets::button& widgets::button::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::button::on_resize() { return _on_resize; }

		widgets::button& widgets::button::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::button::cursor() { return _cursor; }

		widgets::button& widgets::button::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::button::font() { return _font; }

		widgets::button& widgets::button::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::button::font_size() { return _font_size; }

		widgets::button& widgets::button::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::button::color_text() { return _color_text; }

		widgets::button& widgets::button::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::button::color_fill() { return _color_fill; }

		widgets::button& widgets::button::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::button::color_hot() { return _color_hot; }

		widgets::button& widgets::button::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::button::color_selected() { return _color_selected; }

		widgets::button& widgets::button::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::button::color_disabled() { return _color_disabled; }

		widgets::button& widgets::button::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::button::border() { return _border; }

		widgets::button& widgets::button::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::button::color_border() { return _color_border; }

		widgets::button& widgets::button::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::button::corner_radius_x() { return _corner_radius_x; }

		widgets::button& widgets::button::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::button::corner_radius_y() { return _corner_radius_y; }

		widgets::button& widgets::button::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		namespace widgets {
			/// <summary>Button widget builder.</summary>
			class button_builder {
			public:
				/// <summary>Button builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				button_builder(containers::page& page);

				/// <summary>Button builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias of the button, e.g. "next_button".</param>
				button_builder(containers::page& page, const std::string& alias);
				~button_builder();

				/// <summary>Get the button specifications.</summary>
				/// <returns>A reference to the button specifications.</returns>
				[[nodiscard]]
				button& specs();

				/// <summary>Get the button specifications.</summary>
				/// <returns>A reference to the button specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				button& operator()();

				/// <summary>Get the specifications of a button.</summary>
				/// <param name="fm">The form containing the button.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/next_button".</param>
				/// <returns>A reference to the button specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static button& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				button_builder() = delete;
				button_builder(const button_builder&) = delete;
				button_builder& operator=(const button_builder&) = delete;
			};
		}

		class widgets::button_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_button(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::button))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::button_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::button_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::button_selected));

				_specs.badge()
					.color(defaults::color(_page._d_page._fm._d._theme, item::badge))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::badge_border))
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::badge_text));
			}
			containers::page& _page;
			button& _specs;
		};

		widgets::button_builder::button_builder(containers::page& page) :
			button_builder(page, "") {}

		widgets::button_builder::button_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::button_builder::~button_builder() { delete& _d; }

		widgets::button&
			widgets::button_builder::specs() {
			return _d._specs;
		}

		widgets::button&
			widgets::button_builder::operator()() {
			return specs();
		}

		widgets::button&
			widgets::button_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_button_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_button_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::button& liblec::lecui::widgets::button::add(containers::page& page, const std::string& alias) {
			return button_builder(page, alias).specs();
		}

		widgets::button& widgets::button::get(form& fm, const std::string& path) {
			return button_builder::specs(fm, path);
		}
	}
}
