//
// progress_bar.h - progress bar widget interface
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
			/// <summary>Progress bar widget specifications.</summary>
			/// <remarks>Recommended size with defaults is 200x12px</remarks>
			class lecui_api progress_bar_specs : public specs {
				float border_ = 1.f;
				color color_border_;
				float percentage_ = 0.f;

			public:
				progress_bar_specs() {
					rect_.size({ 200.f, 12.f });
				}

				bool operator==(const progress_bar_specs&);
				bool operator!=(const progress_bar_specs&);

				// generic specs

				std::string& text() override;
				progress_bar_specs& text(const std::string& text);

				std::string& tooltip() override;
				progress_bar_specs& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				progress_bar_specs& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				progress_bar_specs& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				progress_bar_specs& cursor(const cursor_type cursor);

				std::string& font() override;
				progress_bar_specs& font(const std::string& font);

				float& font_size() override;
				progress_bar_specs& font_size(const float& font_size);

				color& color_text() override;
				progress_bar_specs& color_text(const color& color_text);

				color& color_fill() override;
				progress_bar_specs& color_fill(const color& color_fill);

				color& color_hot() override;
				progress_bar_specs& color_hot(const color& color_hot);

				color& color_selected() override;
				progress_bar_specs& color_selected(const color& color_selected);

				color& color_disabled() override;
				progress_bar_specs& color_disabled(const color& color_disabled);

				// widget specific specs

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& color_border(const color& color_border);

				/// <summary>Get or set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage();

				/// <summary>Set the percentage of the progress bar (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				progress_bar_specs& percentage(const float& percentage);
			};

			/// <summary>Progress bar widget builder.</summary>
			class lecui_api progress_bar_builder {
			public:
				/// <summary>Progress bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				progress_bar_builder(containers::page& page);

				/// <summary>Progress bar builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "encryption_status".</param>
				progress_bar_builder(containers::page& page, const std::string& alias);
				~progress_bar_builder();

				/// <summary>Get the progress bar specifications.</summary>
				/// <returns>A reference to the progress bar specifications.</returns>
				[[nodiscard]]
				progress_bar_specs& specs();

				/// <summary>Get the progress bar specifications.</summary>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				progress_bar_specs& operator()();

				/// <summary>Get the specifications of a progress bar.</summary>
				/// <param name="fm">The form containing the progress bar.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/encryption_pane/encryption_status".</param>
				/// <returns>A reference to the progress bar specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static progress_bar_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				progress_bar_builder() = delete;
				progress_bar_builder(const progress_bar_builder&) = delete;
				progress_bar_builder& operator=(const progress_bar_builder&) = delete;
			};
		}
	}
}
