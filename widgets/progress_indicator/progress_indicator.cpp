//
// progress_indicator.cpp - progress indicator widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../progress_indicator.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::progress_indicator::operator==(const progress_indicator& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_empty == param._color_empty);
		}

		bool widgets::progress_indicator::operator!=(const progress_indicator& param) {
			return !operator==(param);
		}

		std::string& widgets::progress_indicator::text() { return _text; }

		widgets::progress_indicator& widgets::progress_indicator::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::progress_indicator::tooltip() { return _tooltip; }

		widgets::progress_indicator& widgets::progress_indicator::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::progress_indicator::rect() { return _rect; }

		widgets::progress_indicator& widgets::progress_indicator::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& widgets::progress_indicator::on_resize() { return _on_resize; }

		widgets::progress_indicator& widgets::progress_indicator::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& widgets::progress_indicator::cursor() { return _cursor; }

		widgets::progress_indicator& widgets::progress_indicator::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::progress_indicator::font() { return _font; }

		widgets::progress_indicator& widgets::progress_indicator::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::progress_indicator::font_size() { return _font_size; }

		widgets::progress_indicator& widgets::progress_indicator::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::progress_indicator::color_text() { return _color_text; }

		widgets::progress_indicator& widgets::progress_indicator::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::progress_indicator::color_fill() { return _color_fill; }

		widgets::progress_indicator& widgets::progress_indicator::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::progress_indicator::color_hot() { return _color_hot; }

		widgets::progress_indicator& widgets::progress_indicator::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::progress_indicator::color_selected() { return _color_selected; }

		widgets::progress_indicator& widgets::progress_indicator::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::progress_indicator::color_disabled() { return _color_disabled; }

		widgets::progress_indicator& widgets::progress_indicator::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		color& widgets::progress_indicator::color_empty() { return _color_empty; }

		widgets::progress_indicator& widgets::progress_indicator::color_empty(const color& color_empty) {
			_color_empty = color_empty;
			return *this;
		}

		float& widgets::progress_indicator::line_thickness_empty() { return _line_thickness_empty; }

		widgets::progress_indicator& widgets::progress_indicator::line_thickness_empty(const float& line_thickness_empty) {
			_line_thickness_empty = line_thickness_empty;
			return *this;
		}

		float& widgets::progress_indicator::line_thickness_fill() { return _line_thickness_fill; }

		widgets::progress_indicator& widgets::progress_indicator::line_thickness_fill(const float& line_thickness_fill) {
			_line_thickness_fill = line_thickness_fill;
			return *this;
		}

		float& widgets::progress_indicator::percentage() { return _percentage; }

		widgets::progress_indicator& widgets::progress_indicator::percentage(const float& percentage) {
			_percentage = percentage;
			return *this;
		}

		int& widgets::progress_indicator::precision() { return _precision; }

		widgets::progress_indicator& widgets::progress_indicator::precision(const int& precision) {
			_precision = precision;
			return *this;
		}

		namespace widgets {
			/// <summary>Progress indicator widget builder.</summary>
			class progress_indicator_builder {
			public:
				/// <summary>Progress indicator builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				progress_indicator_builder(containers::page& page);

				/// <summary>Progress indicator builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "file_transfer".</param>
				progress_indicator_builder(containers::page& page, const std::string& alias);
				~progress_indicator_builder();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				[[nodiscard]]
				progress_indicator& specs();

				/// <summary>Get the progress indicator specifications.</summary>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				progress_indicator& operator()();

				/// <summary>Get the specifications of a progress indicator.</summary>
				/// <param name="fm">The form containing the progress indicator.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/files/file_transfer".</param>
				/// <returns>A reference to the progress indicator specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static progress_indicator& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				progress_indicator_builder() = delete;
				progress_indicator_builder(const progress_indicator_builder&) = delete;
				progress_indicator_builder& operator=(const progress_indicator_builder&) = delete;
			};
		}

		class widgets::progress_indicator_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_progress_indicator(alias)) {
				_specs
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator))
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator_text))
					.color_empty(defaults::color(_page._d_page._fm._d._theme, item::progress_indicator_unfilled));
			}
			containers::page& _page;
			progress_indicator& _specs;
		};

		widgets::progress_indicator_builder::progress_indicator_builder(containers::page& page) :
			progress_indicator_builder(page, "") {}

		widgets::progress_indicator_builder::progress_indicator_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::progress_indicator_builder::~progress_indicator_builder() { delete& _d; }

		widgets::progress_indicator&
			widgets::progress_indicator_builder::specs() {
			return _d._specs;
		}

		widgets::progress_indicator&
			widgets::progress_indicator_builder::operator()() {
			return specs();
		}

		widgets::progress_indicator&
			widgets::progress_indicator_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_progress_indicator_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_progress_indicator_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::progress_indicator& liblec::lecui::widgets::progress_indicator::add(containers::page& page, const std::string& alias) {
			return progress_indicator_builder(page, alias).specs();
		}

		widgets::progress_indicator& liblec::lecui::widgets::progress_indicator::get(form& fm, const std::string& path) {
			return progress_indicator_builder::specs(fm, path);
		}
	}
}
