//
// strength_bar.h - strength bar widget interface
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
			/// <summary>Strength bar widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			/// <remarks>Recommended size with defaults is 200x12px</remarks>
			class lecui_api strength_bar : public widget {
			public:
				/// <summary>Strength level as a percentage.</summary>
				struct strength_level {
					/// <summary>The upper limit of the level, e.g. 33.f for "low", 66.f for "medium" and 100.f for "high".</summary>
					float level = 0.f;

					/// <summary>The color to use for the level.</summary>
					lecui::color color;

					bool operator==(const strength_level&);
					bool operator!=(const strength_level&);
				};

			private:
				float _border = 1.f;
				color _color_border;
				float _percentage = 0.f;
				std::vector<strength_level> _levels;

			public:
				strength_bar() {
					_rect.size({ 200.f, 12.f });
				}

				// not using const here because of the need to compare the _levels vector
				bool operator==(strength_bar&);
				bool operator!=(strength_bar&);

				// generic widget

				std::string& text() override;
				strength_bar& text(const std::string& text);

				std::string& tooltip() override;
				strength_bar& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				strength_bar& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				strength_bar& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				strength_bar& cursor(const cursor_type cursor);

				std::string& font() override;
				strength_bar& font(const std::string& font);

				float& font_size() override;
				strength_bar& font_size(const float& font_size);

				color& color_text() override;
				strength_bar& color_text(const color& color_text);

				color& color_fill() override;
				strength_bar& color_fill(const color& color_fill);

				color& color_hot() override;
				strength_bar& color_hot(const color& color_hot);

				color& color_selected() override;
				strength_bar& color_selected(const color& color_selected);

				color& color_disabled() override;
				strength_bar& color_disabled(const color& color_disabled);

				// widget specific widget

				/// <summary>Get or set the thickness of the border.</summary>
				/// <returns>A reference to the border thickness, in pixels.</returns>
				float& border();

				/// <summary>Set the thickness of the border.</summary>
				/// <param name="border">The border thickness, in pixels.</param>
				/// <returns>A reference to the modified object.</returns>
				strength_bar& border(const float& border);

				/// <summary>Get or set the color of the border.</summary>
				/// <returns>A reference to the border color, as defined in <see cref="color"></see>.</returns>
				color& color_border();

				/// <summary>Set the color of the border.</summary>
				/// <param name="color_border">The border color, as defined in <see cref="color"></see>.</param>
				/// <returns>A reference to the modified object.</returns>
				strength_bar& color_border(const color& color_border);

				/// <summary>Get or set the percentage of the strength bar (0.f - 100.f).</summary>
				/// <returns>A reference to the percentage.</returns>
				float& percentage();

				/// <summary>Set the percentage of the strength bar (0.f - 100.f).</summary>
				/// <param name="percentage">The percentage.</param>
				/// <returns>A reference to the modified object.</returns>
				strength_bar& percentage(const float& percentage);

				/// <summary>Get or set the strength levels.</summary>
				/// <returns>A reference to the levels property.</returns>
				std::vector<strength_level>& levels();

				/// <summary>Set the strength levels.</summary>
				/// <param name="levels">The levels.</param>
				/// <returns>A reference to the modified object.</returns>
				strength_bar& levels(const std::vector<strength_level>& levels);

			public:
				/// <summary>Add a strength bar to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "password_quality".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static strength_bar& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a strength bar.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/new_user_pane/password_quality".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static strength_bar& get(form& fm, const std::string& path);
			};
		}
	}
}

#if defined(lecui_helper)
/// Helper for getting strength bar widget. 'Get' method documentation applies.
#define get_strength_bar(path) liblec::lecui::widgets::strength_bar::get(*this, path)
#endif
