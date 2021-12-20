//
// progress_bar.h - progress bar widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			/// <summary>Progress bar widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 200x12px</remarks>
			class lecui_api progress_bar : public widget {
				float _border = 1.f;
				color _color_border;
				color _color_empty;
				float _percentage = 0.f;

			public:
				progress_bar() {
					_rect.size({ 200.f, 12.f });
				}

				bool operator==(const progress_bar&);
				bool operator!=(const progress_bar&);

				// generic widget

				std::string& text() override;
				progress_bar& text(const std::string& text);

				std::string& tooltip() override;
				progress_bar& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				progress_bar& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				progress_bar& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				progress_bar& cursor(const cursor_type cursor);

				std::string& font() override;
				progress_bar& font(const std::string& font);

				float& font_size() override;
				progress_bar& font_size(const float& font_size);

				color& color_text() override;
				progress_bar& color_text(const color& color_text);

				color& color_fill() override;
				progress_bar& color_fill(const color& color_fill);

				color& color_hot() override;
				progress_bar& color_hot(const color& color_hot);

				color& color_selected() override;
				progress_bar& color_selected(const color& color_selected);

				color& color_disabled() override;
				progress_bar& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar& color_border(const color& color_border);

				/// <summary>Get or set the color to use for the empty portion of the indicator.</summary>
				/// <returns>A reference to the color.</returns>
				color& color_empty();

				/// <summary>Set the color to use for the empty portion of the indicator</summary>
				/// <param name="color_empty">The color.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar& color_empty(const color& color_empty);

				/// <summary>Get or set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage();

				/// <summary>Set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar& percentage(const float& percentage);

			public:
				/// <summary>Add a progress bar to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "encryption_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static progress_bar& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a progress bar.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/encryption_pane/encryption_status".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static progress_bar& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting progress bar widget. Builder documentation applies.
#define get_progress_bar(path) liblec::lecui::widgets::progress_bar::get(*this, path)
