//
// checkbox.cpp - checkbox widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../checkbox.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::checkbox::operator==(const checkbox& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_check == param._color_check);
		}

		bool widgets::checkbox::operator!=(const checkbox& param) {
			return !operator==(param);
		}

		std::string& widgets::checkbox::text() { return _text; }

		widgets::checkbox& widgets::checkbox::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::checkbox::tooltip() { return _tooltip; }

		widgets::checkbox& widgets::checkbox::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::checkbox::badge() { return _badge; }

		widgets::checkbox& widgets::checkbox::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::checkbox::rect() { return _rect; }

		widgets::checkbox& widgets::checkbox::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::checkbox::on_resize() { return _on_resize; }

		widgets::checkbox& widgets::checkbox::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::checkbox::cursor() { return _cursor; }

		widgets::checkbox& widgets::checkbox::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::checkbox::font() { return _font; }

		widgets::checkbox& widgets::checkbox::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::checkbox::font_size() { return _font_size; }

		widgets::checkbox& widgets::checkbox::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::checkbox::color_text() { return _color_text; }

		widgets::checkbox& widgets::checkbox::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::checkbox::color_fill() { return _color_fill; }

		widgets::checkbox& widgets::checkbox::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::checkbox::color_hot() { return _color_hot; }

		widgets::checkbox& widgets::checkbox::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::checkbox::color_selected() { return _color_selected; }

		widgets::checkbox& widgets::checkbox::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::checkbox::color_disabled() { return _color_disabled; }

		widgets::checkbox& widgets::checkbox::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::checkbox::border() { return _border; }

		widgets::checkbox& widgets::checkbox::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::checkbox::color_border() { return _color_border; }

		widgets::checkbox& widgets::checkbox::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		std::string& widgets::checkbox::text_unchecked() { return _text_unchecked; }

		widgets::checkbox& widgets::checkbox::text_unchecked(const std::string& text_unchecked) {
			_text_unchecked = text_unchecked;
			return *this;
		}

		std::string& widgets::checkbox::text_indeterminate() { return _text_indeterminate; }

		widgets::checkbox& widgets::checkbox::text_indeterminate(const std::string& text_indeterminate) {
			_text_indeterminate = text_indeterminate;
			return *this;
		}

		lecui::color& widgets::checkbox::color_check() { return _color_check; }

		widgets::checkbox& widgets::checkbox::color_check(const color& color_check) {
			_color_check = color_check;
			return *this;
		}

		widgets::checkbox::checkbox_status& widgets::checkbox::status() { return _status; }

		widgets::checkbox& widgets::checkbox::status(const checkbox_status& status) {
			_status = status;
			return *this;
		}

		namespace widgets {
			/// <summary>Checkbox widget builder.</summary>
			class checkbox_builder {
			public:
				/// <summary>Checkbox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				checkbox_builder(containers::page& page);

				/// <summary>Checkbox builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "employment_status".</param>
				checkbox_builder(containers::page& page, const std::string& alias);
				~checkbox_builder();

				/// <summary>Get the checkbox specifications.</summary>
				/// <returns>A reference to the checkbox specifications.</returns>
				[[nodiscard]]
				checkbox& specs();

				/// <summary>Get the checkbox specifications.</summary>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				checkbox& operator()();

				/// <summary>Get the specifications of a checkbox.</summary>
				/// <param name="fm">The form containing the checkbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/employment_status".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static checkbox& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				checkbox_builder() = delete;
				checkbox_builder(const checkbox_builder&) = delete;
				checkbox_builder& operator=(const checkbox_builder&) = delete;
			};
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
			checkbox& _specs;
		};

		widgets::checkbox_builder::checkbox_builder(containers::page& page) :
			checkbox_builder(page, "") {}

		widgets::checkbox_builder::checkbox_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::checkbox_builder::~checkbox_builder() { delete& _d; }

		widgets::checkbox&
			widgets::checkbox_builder::specs() {
			return _d._specs;
		}

		widgets::checkbox&
			widgets::checkbox_builder::operator()() {
			return specs();
		}

		widgets::checkbox&
			widgets::checkbox_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_checkbox_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_checkbox_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::checkbox& liblec::lecui::widgets::checkbox::add(containers::page& page, const std::string& alias) {
			return checkbox_builder(page, alias).specs();
		}

		widgets::checkbox& liblec::lecui::widgets::checkbox::get(form& fm, const std::string& path) {
			return checkbox_builder::specs(fm, path);
		}
	}
}
