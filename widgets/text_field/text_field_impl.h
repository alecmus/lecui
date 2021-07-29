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
				widgets::widget& generic_specs() override {
					return _specs;
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
				widgets::text_field& specs();
				widgets::text_field& operator()();
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
				widgets::text_field _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_caret;
				ID2D1SolidColorBrush* _p_brush_prompt;
				ID2D1SolidColorBrush* _p_brush_border;
				ID2D1SolidColorBrush* _p_brush_fill;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				ID2D1SolidColorBrush* _p_brush_input_error;
				IDWriteTextFormat* _p_text_format;

				ID2D1Factory* _p_direct2d_factory;
				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;

				const float _margin_x;
				const float _margin_y;
				
				const std::string _caret_blink_timer_name;
				UINT32 _caret_position;
				bool _caret_visible;
				bool _skip_blink;
				float _text_off_set;
				bool _is_selecting;
				bool _is_selected;
				bool _input_error;

				struct selection_info {
					UINT32 start = 0;
					UINT32 end = 0;
				} _selection_info;

				/// Private methods.
				void reset_selection() {
					_selection_info = { 0, 0 };
					_is_selected = false;
				}

				void set_selection(const UINT start, const UINT end) {
					_selection_info.start = start;
					_selection_info.end = end;
					_is_selected = true;
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
