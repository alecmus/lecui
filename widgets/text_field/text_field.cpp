//
// text_field.cpp - text_field widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../text_field.h"
#include "../password_field.h"
#include "../../form_impl/form_impl.h"
#include "..\password_field.h"

namespace liblec {
	namespace lecui {
		bool widgets::text_field::operator==(const text_field& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_prompt == param._color_prompt) &&
				(_color_caret == param._color_caret) &&
				(_color_input_error == param._color_input_error);
		}

		bool widgets::text_field::operator!=(const text_field& param) {
			return !operator==(param);
		}

		std::string& widgets::text_field::text() { return _text; }

		widgets::text_field& widgets::text_field::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::text_field::tooltip() { return _tooltip; }

		widgets::text_field& widgets::text_field::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		widgets::widget::badge_specs& widgets::text_field::badge() { return _badge; }

		widgets::text_field& widgets::text_field::badge(const badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		lecui::rect& widgets::text_field::rect() { return _rect; }

		widgets::text_field& widgets::text_field::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::text_field::on_resize() { return _on_resize; }

		widgets::text_field& widgets::text_field::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::text_field::cursor() { return _cursor; }

		widgets::text_field& widgets::text_field::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::text_field::font() { return _font; }

		widgets::text_field& widgets::text_field::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::text_field::font_size() { return _font_size; }

		widgets::text_field& widgets::text_field::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::text_field::color_text() { return _color_text; }

		widgets::text_field& widgets::text_field::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::text_field::color_fill() { return _color_fill; }

		widgets::text_field& widgets::text_field::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::text_field::color_hot() { return _color_hot; }

		widgets::text_field& widgets::text_field::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::text_field::color_selected() { return _color_selected; }

		widgets::text_field& widgets::text_field::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::text_field::color_disabled() { return _color_disabled; }

		widgets::text_field& widgets::text_field::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::text_field::border() { return _border; }

		widgets::text_field& widgets::text_field::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::text_field::color_border() { return _color_border; }

		widgets::text_field& widgets::text_field::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::text_field::corner_radius_x() { return _corner_radius_x; }

		widgets::text_field& widgets::text_field::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::text_field::corner_radius_y() { return _corner_radius_y; }

		widgets::text_field& widgets::text_field::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		std::string& widgets::text_field::prompt() { return _prompt; }

		widgets::text_field& widgets::text_field::prompt(const std::string& prompt) {
			_prompt = prompt;
			return *this;
		}

		lecui::color& widgets::text_field::color_prompt() { return _color_prompt; }

		widgets::text_field& widgets::text_field::color_prompt(const color& color_prompt) {
			_color_prompt = color_prompt;
			return *this;
		}

		lecui::color& widgets::text_field::color_caret() { return _color_caret; }

		widgets::text_field& widgets::text_field::color_caret(const color& color_caret) {
			_color_caret = color_caret;
			return *this;
		}

		color& widgets::text_field::color_input_error() { return _color_input_error; }

		widgets::text_field& widgets::text_field::color_input_error(const color& color_input_error) {
			_color_input_error = color_input_error;
			return *this;
		}

		std::set<char>& widgets::text_field::allowed_characters() { return _allowed_characters; }

		widgets::text_field& widgets::text_field::allowed_characters(const std::set<char>& allowed_characters) {
			_allowed_characters = allowed_characters;
			return *this;
		}

		std::set<char>& widgets::text_field::forbidden_characters() { return _forbidden_characters; }

		widgets::text_field& widgets::text_field::forbidden_characters(const std::set<char>& forbidden_characters) {
			_forbidden_characters = forbidden_characters;
			return *this;
		}

		int& widgets::text_field::maximum_length() { return _maximum_length; }

		widgets::text_field& widgets::text_field::maximum_length(const int& maximum_length) {
			_maximum_length = maximum_length;
			return *this;
		}

		namespace widgets {
			/// <summary>Text field widget builder.</summary>
			class text_field_builder {
			public:
				/// <summary>Text field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				text_field_builder(containers::page& page);

				/// <summary>Text field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "username".</param>
				text_field_builder(containers::page& page, const std::string& alias);
				virtual ~text_field_builder();

				/// <summary>Get the text_field specifications.</summary>
				/// <returns>A reference to the text_field specifications.</returns>
				[[nodiscard]]
				text_field& specs();

				/// <summary>Get the text_field specifications.</summary>
				/// <returns>A reference to the text_field specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				text_field& operator()();

				/// <summary>Get the specifications of a text_field.</summary>
				/// <param name="fm">The form containing the text_field.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "login_page/credentials_pane/username".</param>
				/// <returns>A reference to the text_field specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static text_field& specs(form& fm, const std::string& path);

			protected:
				class impl;
				impl& _d;

				class password_field_builder;
				friend class password_field_builder;

				// Default constructor and copying an object of this class are not allowed
				text_field_builder() = delete;
				text_field_builder(const text_field_builder&) = delete;
				text_field_builder& operator=(const text_field_builder&) = delete;
			};
		}

		class widgets::text_field_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_text_field(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_prompt(defaults::color(_page._d_page._fm._d._theme, item::text_field_prompt))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::text_field))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::text_field_border))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::text_field_disabled))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::text_field_selected))
					.color_caret(defaults::color(_page._d_page._fm._d._theme, item::text_field_caret))
					.color_input_error(defaults::color(_page._d_page._fm._d._theme, item::text_field_input_error));
			}
			containers::page& _page;
			text_field& _specs;
		};

		widgets::text_field_builder::text_field_builder(containers::page& page) :
			text_field_builder(page, "") {}

		widgets::text_field_builder::text_field_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::text_field_builder::~text_field_builder() { delete& _d; }

		widgets::text_field&
			widgets::text_field_builder::specs() {
			return _d._specs;
		}

		widgets::text_field&
			widgets::text_field_builder::operator()() {
			return specs();
		}

		widgets::text_field&
			widgets::text_field_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_text_field_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_text_field_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		namespace widgets {
			/// <summary>Password field widget builder.</summary>
			/// <remarks>Recommended default size is the same as the text_field widget.</remarks>
			class password_field_builder : public text_field_builder {
			public:
				/// <summary>Password field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				password_field_builder(containers::page& page);

				/// <summary>Password field builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password".</param>
				password_field_builder(containers::page& page, const std::string& alias);
				~password_field_builder();

			private:
				// Default constructor and copying an object of this class are not allowed
				password_field_builder() = delete;
				password_field_builder(const password_field_builder&) = delete;
				password_field_builder& operator=(const password_field_builder&) = delete;
			};
		}

		widgets::password_field_builder::password_field_builder(containers::page& page) :
			password_field_builder(page, "") {}

		widgets::password_field_builder::password_field_builder(containers::page& page, const std::string& alias) :
			text_field_builder(page, alias) {
			_d._specs.mask = '•';
			_d._specs.prompt("Enter password here");
		}

		widgets::password_field_builder::~password_field_builder() {}

		widgets::text_field& liblec::lecui::widgets::password_field::add(containers::page& page, const std::string& alias) {
			return password_field_builder(page, alias).specs();
		}

		widgets::text_field& widgets::password_field::get(form& fm, const std::string& path) {
			return password_field_builder::specs(fm, path);
		}

		widgets::text_field& widgets::text_field::add(containers::page& page, const std::string& alias) {
			return text_field_builder(page, alias).specs();
		}

		widgets::text_field& widgets::text_field::get(form& fm, const std::string& path) {
			return text_field_builder::specs(fm, path);
		}
	}
}
