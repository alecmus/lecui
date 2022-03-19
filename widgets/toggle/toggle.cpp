//
// toggle.cpp - toggle widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../toggle.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::toggle::operator==(const toggle& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_on == param._color_on) &&
				(_color_off == param._color_off);
		}

		bool widgets::toggle::operator!=(const toggle& param) {
			return !operator==(param);
		}

		std::string& widgets::toggle::text() { return _text; }

		widgets::toggle& widgets::toggle::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::toggle::tooltip() { return _tooltip; }

		widgets::toggle& widgets::toggle::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::toggle::rect() { return _rect; }

		widgets::toggle& widgets::toggle::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::toggle::on_resize() { return _on_resize; }

		widgets::toggle& widgets::toggle::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::toggle::cursor() { return _cursor; }

		widgets::toggle& widgets::toggle::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::toggle::font() { return _font; }

		widgets::toggle& widgets::toggle::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::toggle::font_size() { return _font_size; }

		widgets::toggle& widgets::toggle::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::toggle::color_text() { return _color_text; }

		widgets::toggle& widgets::toggle::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::toggle::color_fill() { return _color_fill; }

		widgets::toggle& widgets::toggle::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::toggle::color_hot() { return _color_hot; }

		widgets::toggle& widgets::toggle::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::toggle::color_selected() { return _color_selected; }

		widgets::toggle& widgets::toggle::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::toggle::color_disabled() { return _color_disabled; }

		widgets::toggle& widgets::toggle::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		std::string& widgets::toggle::text_off() { return _text_off; }

		widgets::toggle& widgets::toggle::text_off(const std::string& text_off) {
			_text_off = text_off;
			return *this;
		}

		color& widgets::toggle::color_on() { return _color_on; }

		widgets::toggle& widgets::toggle::color_on(const color& color_on) {
			_color_on = color_on;
			return *this;
		}

		lecui::color& widgets::toggle::color_off() { return _color_off; }

		widgets::toggle& widgets::toggle::color_off(const color& color_off) {
			_color_off = color_off;
			return *this;
		}

		bool& widgets::toggle::on() { return _on; }

		widgets::toggle& widgets::toggle::on(const bool& on) {
			_on = on;
			return *this;
		}

		namespace widgets {
			/// <summary>Toggle widget builder.</summary>
			class toggle_builder {
			public:
				/// <summary>Toggle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				toggle_builder(containers::page& page);

				/// <summary>Toggle builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "use_ssl".</param>
				toggle_builder(containers::page& page, const std::string& alias);
				~toggle_builder();

				/// <summary>Get the toggle specifications.</summary>
				/// <returns>A reference to the toggle specifications.</returns>
				[[nodiscard]]
				toggle& specs();

				/// <summary>Get the toggle specifications.</summary>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				toggle& operator()();

				/// <summary>Get the specifications of a toggle.</summary>
				/// <param name="fm">The form containing the toggle.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/tools_tab_pane/options_tab/use_ssl".</param>
				/// <returns>A reference to the toggle specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static toggle& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				toggle_builder() = delete;
				toggle_builder(const toggle_builder&) = delete;
				toggle_builder& operator=(const toggle_builder&) = delete;
			};
		}

		class widgets::toggle_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_toggle(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, element::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, element::toggle))
					.color_on(defaults::color(_page._d_page._fm._d._theme, element::toggle_on))
					.color_off(defaults::color(_page._d_page._fm._d._theme, element::toggle_off))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, element::toggle_selected));
			}
			containers::page& _page;
			toggle& _specs;
		};

		widgets::toggle_builder::toggle_builder(containers::page& page) :
			toggle_builder(page, "") {}

		widgets::toggle_builder::toggle_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::toggle_builder::~toggle_builder() { delete& _d; }

		widgets::toggle&
			widgets::toggle_builder::specs() {
			return _d._specs;
		}

		widgets::toggle&
			widgets::toggle_builder::operator()() {
			return specs();
		}

		widgets::toggle&
			widgets::toggle_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_toggle_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_toggle_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::toggle& liblec::lecui::widgets::toggle::add(containers::page& page, const std::string& alias) {
			return toggle_builder(page, alias).specs();
		}

		widgets::toggle& liblec::lecui::widgets::toggle::get(form& fm, const std::string& path) {
			return toggle_builder::specs(fm, path);
		}
	}
}
