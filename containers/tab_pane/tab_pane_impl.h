//
// tab_pane_impl.h - tab_pane_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../../widgets/widget_impl.h"
#include "../tab_pane.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class tab_pane_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				// pages <K = tab name, T>
				std::map<std::string, containers::tab> _p_tabs;
				std::vector<std::string> _tab_order;

				std::map<std::string, badge_resources> _tab_badge_resources;
				std::map<std::string, widgets::badge_specs> _badge_specs_old;

				// for ensuring tab resources are recreated when there's any changes in the tabs,
				// i.e. either deletion or removal (using a flag is more efficient than checking in
				// every loop if there's a change in the _p_tabs map, but this calls for
				// greater care as we have to remember to toggle this flag or introduce null
				// reference bugs especially relating to the badges
				bool _tabs_changed;

				// pane rect and page
				containers::page::impl* _p_page_impl = nullptr;
				rect* _p_tab_pane_rect = nullptr;

				/// constructor and destructor
				tab_pane_impl(containers::page& page,
					const std::string& alias,
					const float& content_margin,
					IDWriteFactory* p_directwrite_factory);
				~tab_pane_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				containers::tab_pane& specs();
				containers::tab_pane& operator()();
				const D2D1_RECT_F& client_area();
				const D2D1_RECT_F& tab_pane_area();
				const float& content_margin();
				float caption_bar_height();
				float tab_height();
				float bar_height();
				float padding();
				float margin();
				void set_tab_height(const float height);
				bool tab_height_set();
				void close_tab(const std::string& tab_name);
				const D2D1_SIZE_F programmatic_change_in_size();

			private:
				// Default constructor and copying an object of this class are not allowed
				tab_pane_impl() = delete;
				tab_pane_impl(const tab_pane_impl&) = delete;
				tab_pane_impl& operator=(const tab_pane_impl&) = delete;

				/// Private variables
				containers::tab_pane _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_tabs;
				ID2D1SolidColorBrush* _p_brush_tabs_border;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;
				const float _margin, _padding, _content_margin;
				const float _tab_gap, _tab_text_toggle;
				float _tab_height;
				bool _tab_height_set;
				const float _bar_height;
				D2D1_RECT_F _rect_tabs;
				D2D1_RECT_F _rect_client_area;
				D2D1_RECT_F _rect_tab_pane;

				// tab rectangles <K = tab name, T>
				std::map<std::string, D2D1_RECT_F> _p_tab_rects;

				bool _initial_capture_done = false;
				rect _rect_previous = { 0.f, 0.f, 0.f, 0.f };
				rect _rect_original = { 0.f, 0.f, 0.f, 0.f };

				/// Important override for tab pane to work properly.
				bool contains();
			};
		}
	}
}
