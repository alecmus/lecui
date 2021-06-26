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
			/// <summary>Custom widget.</summary>
			class lecui_api custom {
			public:
				/// <summary>Custom widget specifications.</summary>
				class custom_specs : public specs {
				public:
					/// <summary>
					/// Handler for resource creation. The pointers need to be cast back to their
					/// Direct2D equivalents on the client side before they can be used. e.g.
					/// ID2D1HwndRenderTarget_ can be cast using reinterpret_cast to a
					/// ID2D1HwndRenderTarget. This will be called internally by the library when
					/// device-specific resources associated with the Direct3D device need to be
					/// (re)created.
					/// </summary>
					std::function<void(
						void* ID2D1HwndRenderTarget_,
						void* IDWriteFactory_,
						void* IWICImagingFactory_)>
						on_create_resources = nullptr;

					/// <summary>
					/// Handler for discarding resources. This handler will be called internally
					/// by the library if the Direct3D device dissapears.
					/// </summary>
					std::function<void()> on_discard_resources = nullptr;

					/// <summary>
					/// Handler for rendering. All drawing should be done here. Information about
					/// the widget status are provided through the parameters, e.g. selected_ is
					/// true if the widget is currently selected in the user interface and
					/// pressed_ is true if the left mouse button has not yet been released after
					/// it was pressed down while the cursor was within the widget.
					/// </summary>
					std::function<void(
						void* D2D1_RECT_F_,
						bool enabled_,
						bool hit_,
						bool pressed_,
						bool selected_)>
						on_render = nullptr;

					bool operator==(const custom_specs&);
					bool operator!=(const custom_specs&);

					// generic specs

					std::string& text() override { return text_; }
					custom_specs& text(const std::string& text) {
						text_ = text;
						return *this;
					}

					std::string& tooltip() override { return tooltip_; }
					custom_specs& tooltip(const std::string& tooltip) {
						tooltip_ = tooltip;
						return *this;
					}

					lecui::rect& rect() override { return rect_; }
					custom_specs& rect(const lecui::rect& rect) {
						rect_ = rect;
						return *this;
					}

					resize_params& on_resize() override { return on_resize_; }
					custom_specs& on_resize(const resize_params& on_resize) {
						on_resize_ = on_resize;
						return *this;
					}

					cursor_type& cursor() override { return cursor_; }
					custom_specs& cursor(const cursor_type cursor) {
						cursor_ = cursor;
						return *this;
					}

					std::string& font() override { return font_; }
					custom_specs& font(const std::string& font) {
						font_ = font;
						return *this;
					}

					float& font_size() override { return font_size_; }
					custom_specs& font_size(const float& font_size) {
						font_size_ = font_size;
						return *this;
					}

					color& color_text() override { return color_text_; }
					custom_specs& color_text(const color& color_text) {
						color_text_ = color_text;
						return *this;
					}

					color& color_fill() override { return color_fill_; }
					custom_specs& color_fill(const color& color_fill) {
						color_fill_ = color_fill;
						return *this;
					}

					color& color_hot() override { return color_hot_; }
					custom_specs& color_hot(const color& color_hot) {
						color_hot_ = color_hot;
						return *this;
					}

					color& color_selected() override { return color_selected_; }
					custom_specs& color_selected(const color& color_selected) {
						color_selected_ = color_selected;
						return *this;
					}

					color& color_disabled() override { return color_disabled_; }
					custom_specs& color_disabled(const color& color_disabled) {
						color_disabled_ = color_disabled;
						return *this;
					}
				};

				/// <summary>Custom widget constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				custom(containers::page& page);

				/// <summary>Custom widget constructor.</summary>
				/// <param name="page">The container to place it in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "diagram".</param>
				custom(containers::page& page, const std::string& alias);
				~custom();

				/// <summary>Get the custom widget specifications.</summary>
				/// <returns>A reference to the custom widget's specifications.</returns>
				[[nodiscard]]
				custom_specs& specs();

				/// <summary>Get the custom widget specifications.</summary>
				/// <returns>A reference to the custom widget's specifications.</returns>
				/// <remarks>Alternative to specs() for more terse code.</remarks>
				[[nodiscard]]
				custom_specs& operator()();

				/// <summary>Get the specifications of a custom widget.</summary>
				/// <param name="fm">The form containing the widget.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "sample_page/right_pane/tab_pane/tab_two/diagram".</param>
				/// <returns>A reference to the custom widget's specifications.</returns>
				/// <remarks>Throws on failure.</remarks>
				[[nodiscard]]
				static custom_specs& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& d_;

				// Default constructor and copying an object of this class are not allowed
				custom() = delete;
				custom(const custom&) = delete;
				custom& operator=(const custom&) = delete;
			};
		}
	}
}
