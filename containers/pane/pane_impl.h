//
// pane_impl.h - pane_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../../widgets/widget_impl.h"
#include "../pane.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class pane_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _p_panes.at(_current_pane);
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

				/// <summary>Get the prefix of the special pane used to encase date.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string date_pane_alias_prefix();

				/// <summary>Get the prefix of the special pane used to encase an icon.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string icon_pane_alias_prefix();

				/// <summary>Get the prefix of the special pane used to encase a table.</summary>
				/// <returns>The special prefix. No other alias should have this prefix.</returns>
				static std::string table_pane_alias_prefix();

				// pages <K = pane alias, T>
				std::map<std::string, containers::pane> _p_panes;
				std::string _current_pane;

				// pane rect and page
				containers::page::impl* _p_page_impl = nullptr;
				rect* _p_pane_rect = nullptr;
				bool _size_initialized = false;

				/// constructor and destructor
				pane_impl(containers::page& page,
					const std::string& alias, const float& content_margin);
				~pane_impl();

				/// virtual function override
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				containers::pane_specs& specs();
				containers::pane_specs& operator()();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& pane_area();
				const float& content_margin();

			private:
				// Default constructor and copying an object of this class are not allowed
				pane_impl() = delete;
				pane_impl(const pane_impl&) = delete;
				pane_impl& operator=(const pane_impl&) = delete;

				/// Private variables
				containers::pane_specs _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_disabled;
				const float _margin;
				const float _content_margin;
				D2D1_RECT_F _rect_client_area;
				D2D1_RECT_F _rect_pane;

				bool _initial_capture_done = false;
				rect _rect_previous = { 0.f, 0.f, 0.f, 0.f };

				/// Important override for pane to work properly.
				bool contains() override;
			};
		}
	}
}
