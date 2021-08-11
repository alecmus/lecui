//
// strength_bar_impl.h - strength_bar_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../strength_bar.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class strength_bar_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				strength_bar_impl(containers::page& page,
					const std::string& alias,
					ID2D1Factory* p_direct2d_factory,
					IDWriteFactory* p_directwrite_factory);
				~strength_bar_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::strength_bar& specs();
				widgets::strength_bar& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				strength_bar_impl() = delete;
				strength_bar_impl(const strength_bar_impl&) = delete;
				strength_bar_impl& operator=(const strength_bar_impl&) = delete;

				/// Private variables
				widgets::strength_bar _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_fill;
				std::map<float, ID2D1SolidColorBrush*> _fill_brushes;	// key is level
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
