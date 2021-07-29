//
// checkbox_impl.h - checkbox_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../checkbox.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class checkbox_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				checkbox_impl(containers::page& page,
					const std::string& alias,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~checkbox_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				void on_action() override;
				void on_right_click() override;

				/// widget specific methods
				widgets::checkbox& specs();
				widgets::checkbox& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				checkbox_impl() = delete;
				checkbox_impl(const checkbox_impl&) = delete;
				checkbox_impl& operator=(const checkbox_impl&) = delete;

				/// Private variables
				widgets::checkbox _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_check;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				ID2D1Factory* _p_direct2d_factory;
				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;
			};
		}
	}
}
