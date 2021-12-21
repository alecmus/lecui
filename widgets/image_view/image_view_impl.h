//
// image_view_impl.h - image_view_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../widget_impl.h"
#include "../image_view.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class image_view_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				/// constructor and destructor
				image_view_impl(containers::page& page,
					const std::string& alias,
					IWICImagingFactory* p_IWICFactory,
					IDWriteFactory* p_directwrite_factory);
				~image_view_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::image_view& specs();
				widgets::image_view& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				image_view_impl() = delete;
				image_view_impl(const image_view_impl&) = delete;
				image_view_impl& operator=(const image_view_impl&) = delete;

				/// Private variables
				widgets::image_view _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				ID2D1Bitmap* _p_bitmap;
				IWICImagingFactory* _p_IWICFactory;

				IDWriteFactory* _p_directwrite_factory;

				badge_resources _badge_resources;

				size _old_size;
			};
		}
	}
}
