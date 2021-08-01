//
// custom.h - custom widget interface
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
			/// <summary>Custom widget. To instantiate and add to a container use the <see cref="add"></see> static method.</summary>
			class lecui_api custom : public widget {
			public:
				/// <summary>
				/// Handler for resource creation. The pointers need to be cast back to their
				/// Direct2D equivalents on the client side before they can be used. e.g.
				/// _ID2D1HwndRenderTarget can be cast using reinterpret_cast to a
				/// ID2D1HwndRenderTarget. This will be called internally by the library when
				/// device-specific resources associated with the Direct3D device need to be
				/// (re)created.
				/// </summary>
				std::function<void(
					void* _ID2D1HwndRenderTarget,
					void* _IDWriteFactory,
					void* _IWICImagingFactory)>
					on_create_resources = nullptr;

				/// <summary>
				/// Handler for discarding resources. This handler will be called internally
				/// by the library if the Direct3D device dissapears.
				/// </summary>
				std::function<void()> on_discard_resources = nullptr;

				/// <summary>
				/// Handler for rendering. All drawing should be done here. Information about
				/// the widget status are provided through the parameters, e.g. _selected is
				/// true if the widget is currently selected in the user interface and
				/// _pressed is true if the left mouse button has not yet been released after
				/// it was pressed down while the cursor was within the widget.
				/// </summary>
				std::function<void(
					void* _D2D1_RECT_F,
					bool _enabled,
					bool _hit,
					bool _pressed,
					bool _selected)>
					on_render = nullptr;

				bool operator==(const custom&);
				bool operator!=(const custom&);

				// generic widget

				std::string& text() override;
				custom& text(const std::string& text);

				std::string& tooltip() override;
				custom& tooltip(const std::string& tooltip);

				lecui::rect& rect() override;
				custom& rect(const lecui::rect& rect);

				resize_params& on_resize() override;
				custom& on_resize(const resize_params& on_resize);

				cursor_type& cursor() override;
				custom& cursor(const cursor_type cursor);

				std::string& font() override;
				custom& font(const std::string& font);

				float& font_size() override;
				custom& font_size(const float& font_size);

				color& color_text() override;
				custom& color_text(const color& color_text);

				color& color_fill() override;
				custom& color_fill(const color& color_fill);

				color& color_hot() override;
				custom& color_hot(const color& color_hot);

				color& color_selected() override;
				custom& color_selected(const color& color_selected);

				color& color_disabled() override;
				custom& color_disabled(const color& color_disabled);

			public:
				/// <summary>Add a custom widget to a container.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "diagram".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>If an empty alias is given an internally generated random
				/// alias will be assigned.</remarks>
				[[nodiscard]]
				static custom& add(containers::page& page, const std::string& alias = std::string());

				/// <summary>Get the specifications of a custom widget.</summary>
				/// <param name="fm">A reference to the form.</param>
				/// <param name="path">The full path to the widget, e.g. "sample_page/right_pane/tab_pane/tab_two/diagram".</param>
				/// <returns>A reference to the widget specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static custom& get(form& fm, const std::string& path);
			};
		}
	}
}

/// Helper for getting custom widget. Builder documentation applies.
#define get_custom(path) liblec::lecui::widgets::custom::get(*this, path)
