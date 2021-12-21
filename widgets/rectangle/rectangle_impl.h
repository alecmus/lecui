//
// rectangle_impl.h - rectangle_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../widget_impl.h"
#include "../rectangle.h"

#include "../../widgets/h_scrollbar/h_scrollbar_impl.h"
#include "../../widgets/v_scrollbar/v_scrollbar_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class rectangle_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// <summary>Get the alias of the special rectangle used with pages. This
				/// rectangle is important as it keeps track of the page dimensions, and makes 
				/// the scroll bars work.</summary>
				/// <returns>The special alias. No other widget should have this alias.</returns>
				static std::string page_rect_alias();

				/// constructor and destructor
				rectangle_impl(containers::page& page,
					const std::string& alias,
					widgets::h_scrollbar_impl& h_scrollbar,
					widgets::v_scrollbar_impl& v_scrollbar,
					IDWriteFactory* p_directwrite_factory);
				~rectangle_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				bool contains(const D2D1_POINT_2F& point) override;
				D2D1_POINT_2F get_scrollbar_offset();

				/// widget specific methods
				widgets::rectangle& specs();
				widgets::rectangle& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				rectangle_impl() = delete;
				rectangle_impl(const rectangle_impl&) = delete;
				rectangle_impl& operator=(const rectangle_impl&) = delete;

				/// Private variables
				widgets::rectangle _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_border_hot;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;

				IDWriteFactory* _p_directwrite_factory;

				badge_resources _badge_resources;

				/// page scroll bars
				widgets::h_scrollbar_impl& _h_scrollbar;
				widgets::v_scrollbar_impl& _v_scrollbar;
			};
		}
	}
}
