//
// strength_bar.cpp - strength bar widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../strength_bar.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::strength_bar::operator==(strength_bar& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				is_equal(_levels, param._levels);
		}

		bool widgets::strength_bar::operator!=(strength_bar& param) {
			return !operator==(param);
		}

		std::string& widgets::strength_bar::text() { return _text; }

		widgets::strength_bar& widgets::strength_bar::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::strength_bar::tooltip() { return _tooltip; }

		widgets::strength_bar& widgets::strength_bar::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::strength_bar::rect() { return _rect; }

		widgets::strength_bar& widgets::strength_bar::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::strength_bar::on_resize() { return _on_resize; }

		widgets::strength_bar& widgets::strength_bar::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::strength_bar::cursor() { return _cursor; }

		widgets::strength_bar& widgets::strength_bar::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::strength_bar::font() { return _font; }

		widgets::strength_bar& widgets::strength_bar::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::strength_bar::font_size() { return _font_size; }

		widgets::strength_bar& widgets::strength_bar::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::strength_bar::color_text() { return _color_text; }

		widgets::strength_bar& widgets::strength_bar::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::strength_bar::color_fill() { return _color_fill; }

		widgets::strength_bar& widgets::strength_bar::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::strength_bar::color_hot() { return _color_hot; }

		widgets::strength_bar& widgets::strength_bar::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::strength_bar::color_selected() { return _color_selected; }

		widgets::strength_bar& widgets::strength_bar::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::strength_bar::color_disabled() { return _color_disabled; }

		widgets::strength_bar& widgets::strength_bar::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::strength_bar::border() { return _border; }

		widgets::strength_bar& widgets::strength_bar::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& widgets::strength_bar::color_border() { return _color_border; }

		widgets::strength_bar& widgets::strength_bar::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& widgets::strength_bar::percentage() { return _percentage; }

		widgets::strength_bar& widgets::strength_bar::percentage(const float& percentage) {
			_percentage = percentage;
			return *this;
		}

		std::vector<widgets::strength_bar::strength_level>& widgets::strength_bar::levels() { return _levels; }

		widgets::strength_bar& widgets::strength_bar::levels(const std::vector<strength_level>& levels) {
			_levels = levels;
			return *this;
		}

		namespace widgets {
			/// <summary>strength bar widget builder.</summary>
			class strength_bar_builder {
			public:
				/// <summary>strength bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				strength_bar_builder(containers::page& page);

				/// <summary>strength bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "encryption_status".</param>
				strength_bar_builder(containers::page& page, const std::string& alias);
				~strength_bar_builder();

				/// <summary>Get the strength bar specifications.</summary>
				/// <returns>A reference to the strength bar specifications.</returns>
				[[nodiscard]]
				strength_bar& specs();

				/// <summary>Get the strength bar specifications.</summary>
				/// <returns>A reference to the strength bar specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				strength_bar& operator()();

				/// <summary>Get the specifications of a strength bar.</summary>
				/// <param name="fm">The form containing the strength bar.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/encryption_pane/encryption_status".</param>
				/// <returns>A reference to the strength bar specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static strength_bar& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				strength_bar_builder() = delete;
				strength_bar_builder(const strength_bar_builder&) = delete;
				strength_bar_builder& operator=(const strength_bar_builder&) = delete;
			};

			bool strength_bar::strength_level::operator==(const strength_level& param) {
				return
					(level == param.level) &&
					(color == param.color);
			}

			bool strength_bar::strength_level::operator!=(const strength_level& param) {
				return !operator==(param);
			}
		}

		class widgets::strength_bar_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_strength_bar(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, element::strength_bar))
					.color_border(defaults::color(_page._d_page._fm._d._theme, element::strength_bar_border));
			}
			containers::page& _page;
			strength_bar& _specs;
		};

		widgets::strength_bar_builder::strength_bar_builder(containers::page& page) :
			strength_bar_builder(page, "") {}

		widgets::strength_bar_builder::strength_bar_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::strength_bar_builder::~strength_bar_builder() { delete& _d; }

		widgets::strength_bar&
			widgets::strength_bar_builder::specs() {
			return _d._specs;
		}

		widgets::strength_bar&
			widgets::strength_bar_builder::operator()() {
			return specs();
		}

		widgets::strength_bar&
			widgets::strength_bar_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_strength_bar_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_strength_bar_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::strength_bar& liblec::lecui::widgets::strength_bar::add(containers::page& page, const std::string& alias) {
			return strength_bar_builder(page, alias).specs();
		}

		widgets::strength_bar& liblec::lecui::widgets::strength_bar::get(form& fm, const std::string& path) {
			return strength_bar_builder::specs(fm, path);
		}
	}
}
