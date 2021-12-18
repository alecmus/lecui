//
// button_impl.h - button_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../widget_impl.h"
#include "../button.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class button_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				button_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~button_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::button& specs();
				widgets::button& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				button_impl() = delete;
				button_impl(const button_impl&) = delete;
				button_impl& operator=(const button_impl&) = delete;

				/// Private variables
				widgets::button _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;
			};
		}
	}
}
