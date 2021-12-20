//
// label.cpp - label widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../label.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::label::operator==(const label& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_text == param._text) &&
				(_color_hot_pressed == param._color_hot_pressed) &&
				(_multiline == param._multiline) &&
				(_center_h == param._center_h) &&
				(_center_v == param._center_v);
		}

		bool widgets::label::operator!=(const label& param) {
			return !operator==(param);
		}

		std::string& widgets::label::text() { return _text; }

		widgets::label& widgets::label::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::label::tooltip() { return _tooltip; }

		widgets::label& widgets::label::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::label::badge() { return _badge; }

		widgets::label& widgets::label::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::label::rect() { return _rect; }

		widgets::label& widgets::label::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::label::on_resize() { return _on_resize; }

		widgets::label& widgets::label::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::label::cursor() { return _cursor; }

		widgets::label& widgets::label::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::label::font() { return _font; }

		widgets::label& widgets::label::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::label::font_size() { return _font_size; }

		widgets::label& widgets::label::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::label::color_text() { return _color_text; }

		widgets::label& widgets::label::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::label::color_fill() { return _color_fill; }

		widgets::label& widgets::label::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::label::color_hot() { return _color_hot; }

		widgets::label& widgets::label::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::label::color_selected() { return _color_selected; }

		widgets::label& widgets::label::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::label::color_disabled() { return _color_disabled; }

		widgets::label& widgets::label::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		lecui::color& widgets::label::color_hot_pressed() { return _color_hot_pressed; }

		widgets::label& widgets::label::color_hot_pressed(const color& color_hot_pressed) {
			_color_hot_pressed = color_hot_pressed;
			return *this;
		}

		bool& widgets::label::multiline() { return _multiline; }

		widgets::label& widgets::label::multiline(const bool& multiline) {
			_multiline = multiline;
			return *this;
		}

		bool& widgets::label::center_h() { return _center_h; }

		widgets::label& widgets::label::center_h(const bool& center_h) {
			_center_h = center_h;
			return *this;
		}

		bool& widgets::label::center_v() { return _center_v; }

		widgets::label& widgets::label::center_v(const bool& center_v) {
			_center_v = center_v;
			return *this;
		}

		namespace widgets {
			/// <summary>Label widget builder.</summary>
			class label_builder {
			public:
				/// <summary>Label builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				label_builder(containers::page& page);

				/// <summary>Label builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "caption".</param>
				label_builder(containers::page& page, const std::string& alias);
				~label_builder();

				/// <summary>Get the label specifications.</summary>
				/// <returns>A reference to the label specifications.</returns>
				[[nodiscard]]
				label& specs();

				/// <summary>Get the label specifications.</summary>
				/// <returns>A reference to the label specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				label& operator()();

				/// <summary>Get the specifications of a label.</summary>
				/// <param name="fm">The form containing the label.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_pane/caption".</param>
				/// <returns>A reference to the label specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static label& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				label_builder() = delete;
				label_builder(const label_builder&) = delete;
				label_builder& operator=(const label_builder&) = delete;
			};
		}

		class widgets::label_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_label(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::label_selected))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::label_hover))
					.color_hot_pressed(defaults::color(_page._d_page._fm._d._theme, item::label_pressed))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::label_disabled));
			}
			containers::page& _page;
			label& _specs;
		};

		widgets::label_builder::label_builder(containers::page& page) :
			label_builder(page, "") {}

		widgets::label_builder::label_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::label_builder::~label_builder() { delete& _d; }

		widgets::label&
			widgets::label_builder::specs() {
			return _d._specs;
		}

		widgets::label&
			widgets::label_builder::operator()() {
			return specs();
		}

		widgets::label&
			widgets::label_builder::specs(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_label_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_label_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::label& liblec::lecui::widgets::label::add(containers::page& page, const std::string& alias) {
			return label_builder(page, alias).specs();
		}

		widgets::label& widgets::label::get(form& fm, const std::string& path) {
			return label_builder::specs(fm, path);
		}
	}
}
