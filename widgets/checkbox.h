//
// checkbox.h - checkbox widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "widget.h"
#else
#include <liblec/lecui/widgets/widget.h>
#endif

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// <summary>Checkbox widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 20x20px.</remarks>
			class lecui_api checkbox_specs : public specs {
			public:
				/// <summary>Checkbox status.</summary>
				enum class checkbox_status {
					/// <summary>'Checked' status.</summary>
					checked,

					/// <summary>'Unchecked' status.</summary>
					unchecked,

					/// <summary>'Indeterminate' status.</summary>
					indeterminate,
				};

			private:
				float border_ = .5f;
				color color_border_;
				std::string text_unchecked_;
				std::string text_indeterminate_;
				color color_check_;
				checkbox_status status_ = checkbox_status::indeterminate;

			public:
				checkbox_specs() {
					cursor_ = cursor_type::hand;
					rect_.size({ 20.f, 20.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct checkbox_events : basic_events {
					/// <summary>Called when the checkbox is clicked.</summary>
					/// <remarks>The parameter indicates the new checkbox status.</remarks>
					std::function<void(checkbox_status checked)> check = nullptr;
				};

				checkbox_events& events() {
					return checkbox_events_;
				}

				bool operator==(const checkbox_specs&);
				bool operator!=(const checkbox_specs&);

				// generic specs

				std::string& text() override { return text_; }
				checkbox_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				checkbox_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				checkbox_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				checkbox_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				checkbox_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				checkbox_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				checkbox_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				checkbox_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				checkbox_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				checkbox_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				checkbox_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				checkbox_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border() { return border_; }

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox_specs& border(const float& border) {
					border_ = border;
					return *this;
				}

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border() { return color_border_; }

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox_specs& color_border(const color& color_border) {
					color_border_ = color_border;
					return *this;
				}

				/// <summary>Get or set the text to display when the widget is unchecked.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_unchecked() { return text_unchecked_; }

				/// <summary>Set the text to display when the widget is unchecked.</summary>
				/// <param name="text_unchecked">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox_specs& text_unchecked(const std::string& text_unchecked) {
					text_unchecked_ = text_unchecked;
					return *this;
				}

				/// <summary>Get or set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_indeterminate() { return text_indeterminate_; }

				/// <summary>Set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <param name="text_unchecked">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox_specs& text_indeterminate(const std::string& text_indeterminate) {
					text_indeterminate_ = text_indeterminate;
					return *this;
				}

				/// <summary>Get or set the color to use for the check symbol.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_check() { return color_check_; }

				/// <summary>Set the color to use for the check symbol.</summary>
				/// <param name="color_check">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox_specs& color_check(const color& color_check) {
					color_check_ = color_check;
					return *this;
				}

				/// <summary>Get or set the status of the checkbox widget.</summary>
				/// <returns>The status of the widget as defined in <see cref="checkbox_status"></see>.</returns>
				checkbox_status& status() { return status_; }

				/// <summary>Set the status of the checkbox widget.</summary>
				/// <param name="status">The status of the widget as defined in <see cref="checkbox_status"></see>.</param>
				/// <returns></returns>
				checkbox_specs& status(const checkbox_status& status) {
					status_ = status;
					return *this;
				}

			private:
				checkbox_events checkbox_events_;
			};

			/// <summary>Checkbox widget builder.</summary>
			class lecui_api checkbox_builder {
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
				checkbox_specs& specs();

				/// <summary>Get the checkbox specifications.</summary>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				checkbox_specs& operator()();

				/// <summary>Get the specifications of a checkbox.</summary>
				/// <param name="fm">The form containing the checkbox.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/sample_tab_pane/tab_one/employment_status".</param>
				/// <returns>A reference to the checkbox specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static checkbox_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				checkbox_builder() = delete;
				checkbox_builder(const checkbox_builder&) = delete;
				checkbox_builder& operator=(const checkbox_builder&) = delete;
			};
		}
	}
}
