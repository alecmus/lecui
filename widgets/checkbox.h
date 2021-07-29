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
			class lecui_api checkbox : public widget {
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
				float _border = .5f;
				color _color_border;
				std::string _text_unchecked;
				std::string _text_indeterminate;
				color _color_check;
				checkbox_status _status = checkbox_status::indeterminate;

			public:
				checkbox() {
					_cursor = cursor_type::hand;
					_rect.size({ 20.f, 20.f });
				}

				/// <summary>Events specific to this widget.</summary>
				struct checkbox_events : basic_events {
					/// <summary>Called when the checkbox is clicked.</summary>
					/// <remarks>The parameter indicates the new checkbox status.</remarks>
					std::function<void(checkbox_status checked)> check = nullptr;
				};

				checkbox_events& events() {
					return _checkbox_events;
				}

				bool operator==(const checkbox&);
				bool operator!=(const checkbox&);

				// generic widget

				std::string& text() override;
				checkbox& text(const std::string& text);

				std::string& tooltip() override;
				checkbox& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				checkbox& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				checkbox& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				checkbox& cursor(const cursor_type cursor);

				std::string& font() override;
				checkbox& font(const std::string& font);

				float& font_size() override;
				checkbox& font_size(const float& font_size);

				color& color_text() override;
				checkbox& color_text(const color& color_text);

				color& color_fill() override;
				checkbox& color_fill(const color& color_fill);

				color& color_hot() override;
				checkbox& color_hot(const color& color_hot);

				color& color_selected() override;
				checkbox& color_selected(const color& color_selected);

				color& color_disabled() override;
				checkbox& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_border(const color& color_border);

				/// <summary>Get or set the text to display when the widget is unchecked.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_unchecked();

				/// <summary>Set the text to display when the widget is unchecked.</summary>
				/// <param name="text_unchecked">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& text_unchecked(const std::string& text_unchecked);

				/// <summary>Get or set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <returns>A reference to the text.</returns>
				std::string& text_indeterminate();

				/// <summary>Set the text to display when the widget's check status has not been explicitly set.</summary>
				/// <param name="text_unchecked">The text.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& text_indeterminate(const std::string& text_indeterminate);

				/// <summary>Get or set the color to use for the check symbol.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_check();

				/// <summary>Set the color to use for the check symbol.</summary>
				/// <param name="color_check">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				checkbox& color_check(const color& color_check);

				/// <summary>Get or set the status of the checkbox widget.</summary>
				/// <returns>The status of the widget as defined in <see cref="checkbox_status"></see>.</returns>
				checkbox_status& status();

				/// <summary>Set the status of the checkbox widget.</summary>
				/// <param name="status">The status of the widget as defined in <see cref="checkbox_status"></see>.</param>
				/// <returns></returns>
				checkbox& status(const checkbox_status& status);

			public:
				/// <summary>Add a checkbox to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "employment_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static checkbox& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a checkbox.</summary>
				/// <param name="fm"></param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/sample_tab_pane/tab_one/employment_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static checkbox& get(form& fm, const std::string& path);

			private:
				checkbox_events _checkbox_events;
			};
		}
	}
}

/// Helper for getting checkbox widget. Builder documentation applies.
#define get_checkbox(path) liblec::lecui::widgets::checkbox::get(*this, path)
