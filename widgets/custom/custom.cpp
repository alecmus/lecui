//
// custom.cpp - custom widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../custom.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::custom::operator==(const custom& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				true;
		}

		bool widgets::custom::operator!=(const custom& param) {
			return !operator==(param);
		}

		std::string& widgets::custom::text() { return _text; }

		widgets::custom& widgets::custom::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::custom::tooltip() { return _tooltip; }

		widgets::custom& widgets::custom::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::custom::rect() { return _rect; }

		widgets::custom& widgets::custom::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::custom::on_resize() { return _on_resize; }

		widgets::custom& widgets::custom::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::custom::cursor() { return _cursor; }

		widgets::custom& widgets::custom::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::custom::font() { return _font; }

		widgets::custom& widgets::custom::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::custom::font_size() { return _font_size; }

		widgets::custom& widgets::custom::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::custom::color_text() { return _color_text; }

		widgets::custom& widgets::custom::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::custom::color_fill() { return _color_fill; }

		widgets::custom& widgets::custom::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::custom::color_hot() { return _color_hot; }

		widgets::custom& widgets::custom::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::custom::color_selected() { return _color_selected; }

		widgets::custom& widgets::custom::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::custom::color_disabled() { return _color_disabled; }

		widgets::custom& widgets::custom::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		namespace widgets {
			/// <summary>Custom widget builder.</summary>
			class custom_builder {
			public:
				/// <summary>Custom widget builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				custom_builder(containers::page& page);

				/// <summary>Custom widget builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "diagram".</param>
				custom_builder(containers::page& page, const std::string& alias);
				~custom_builder();

				/// <summary>Get the custom widget specifications.</summary>
				/// <returns>A reference to the custom widget's specifications.</returns>
				[[nodiscard]]
				custom& specs();

				/// <summary>Get the custom widget specifications.</summary>
				/// <returns>A reference to the custom widget's specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				custom& operator()();

				/// <summary>Get the specifications of a custom widget.</summary>
				/// <param name="fm">The form containing the widget.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/right_pane/tab_pane/tab_two/diagram".</param>
				/// <returns>A reference to the custom widget's specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static custom& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				custom_builder() = delete;
				custom_builder(const custom_builder&) = delete;
				custom_builder& operator=(const custom_builder&) = delete;
			};
		}

		class widgets::custom_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_custom(alias)) {}
			containers::page& _page;
			custom& _specs;
		};

		widgets::custom_builder::custom_builder(containers::page& page) :
			custom_builder(page, "") {}

		widgets::custom_builder::custom_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::custom_builder::~custom_builder() { delete& _d; }

		widgets::custom&
			widgets::custom_builder::specs() {
			return _d._specs;
		}

		widgets::custom&
			widgets::custom_builder::operator()() {
			return specs();
		}

		widgets::custom&
			widgets::custom_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_custom_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_custom_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::custom& liblec::lecui::widgets::custom::add(containers::page& page, const std::string& alias) {
			return custom_builder(page, alias).specs();
		}

		widgets::custom& liblec::lecui::widgets::custom::get(form& fm, const std::string& path) {
			return custom_builder::specs(fm, path);
		}
	}
}
