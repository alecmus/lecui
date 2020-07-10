/*
** pane_impl.h - pane_impl interface
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

#include "../../widgets/widget_impl.h"
#include "../pane.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class pane_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// <summary>Get the prefix of the special pane used to encase trees. This pane is
				/// important for scrolling effects.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string tree_pane_alias_prefix();

				/// <summary>Get the prefix of the special pane used to encase html widgets. This
				/// pane is important for scrolling effects.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string html_pane_alias_prefix();

				/// <summary>Get the prefix of the special pane used to encase html controls. This
				/// pane is important for scrolling effects.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string html_controls_pane_alias_prefix();

				/// <summary>Get the prefix of the special pane used to encase time.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string time_pane_alias_prefix();

				// pages <K = pane alias, T>
				std::map<std::string, containers::page> p_panes_;
				std::string current_pane_;

				/// Prevent the use of the default constructor.
				pane_impl() = delete;

				/// constructor and destructor
				pane_impl(containers::page& page,
					const std::string& alias);
				~pane_impl();

				/// virtual function override
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				containers::pane::pane_specs& specs();
				containers::pane::pane_specs& operator()();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& pane_area();

			private:
				/// Prevent copying an object of this class.
				pane_impl(const pane_impl&);
				pane_impl& operator=(const pane_impl&);

				/// Private variables
				containers::pane::pane_specs specs_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				const float margin_;
				D2D1_RECT_F rect_client_area_;
				D2D1_RECT_F rect_pane_;

				/// Important override for pane to work properly.
				bool contains() override;
			};
		}
	}
}
