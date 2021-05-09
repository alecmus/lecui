//
// toggle_impl.h - toggle_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../toggle.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class toggle_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				toggle_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~toggle_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				void on_action() override;
				bool contains(const D2D1_POINT_2F& point) override;
				bool contains() override;
				bool hit(const bool& hit) override;

				/// widget specific methods
				widgets::toggle::toggle_specs& specs();
				widgets::toggle::toggle_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				toggle_impl() = delete;
				toggle_impl(const toggle_impl&) = delete;
				toggle_impl& operator=(const toggle_impl&) = delete;

				/// Private variables
				widgets::toggle::toggle_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_on_;
				ID2D1SolidColorBrush* p_brush_off_;
				ID2D1SolidColorBrush* p_brush_on_hot_;
				ID2D1SolidColorBrush* p_brush_off_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;
				D2D1_RECT_F rect_toggle_;
				float perc_along_;
			};
		}
	}
}
