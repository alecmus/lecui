//
// text_field_impl.h - text_field_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../text_field.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class text_field_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				text_field_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~text_field_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_selection_change(const bool& selected) override;

				/// widget specific methods
				widgets::text_field_specs& specs();
				widgets::text_field_specs& operator()();
				void insert_character(const char& c);
				void key_backspace();
				void key_delete();
				void key_left();
				void key_right();

			private:
				// Default constructor and copying an object of this class are not allowed
				text_field_impl() = delete;
				text_field_impl(const text_field_impl&) = delete;
				text_field_impl& operator=(const text_field_impl&) = delete;

				/// Private variables
				widgets::text_field_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_caret_;
				ID2D1SolidColorBrush* p_brush_prompt_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				ID2D1Factory* p_direct2d_factory_;
				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				const float margin_x_;
				const float margin_y_;
				
				const std::string caret_blink_timer_name_;
				UINT32 caret_position_;
				bool caret_visible_;
				bool skip_blink_;
				float text_off_set_;
				bool is_selecting_;
				bool is_selected_;

				struct selection_info {
					UINT32 start = 0;
					UINT32 end = 0;
				} selection_info_;

				/// Private methods.
				void reset_selection() {
					selection_info_ = { 0, 0 };
					is_selected_ = false;
				}

				void set_selection(const UINT start, const UINT end) {
					selection_info_.start = start;
					selection_info_.end = end;
					is_selected_ = true;
				}

				static UINT32 count_characters(IDWriteTextLayout* p_text_layout,
					const std::string& text, const D2D1_RECT_F& rect_text,
					const D2D1_POINT_2F& point, const float& dpi_scale);

				static UINT32 get_caret_position(IDWriteTextLayout* p_text_layout,
					const std::string& text, const D2D1_RECT_F& rect_text,
					const D2D1_POINT_2F& point, const float& dpi_scale);

				static D2D1_RECT_F get_selection_rect(IDWriteTextLayout* p_text_layout,
					const D2D1_RECT_F& rect_text, const UINT32& selection_start,
					const UINT32& selection_end);

				static float get_caret_width();

				static D2D1_RECT_F get_caret_rect(IDWriteTextLayout* p_text_layout,
					const D2D1_RECT_F& rect_text, const UINT32& caret_position);
			};
		}
	}
}
