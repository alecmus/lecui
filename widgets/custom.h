/*
** custom.h - custom widget interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

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
