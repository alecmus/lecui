/*
** combobox_impl.h - combobox_impl interface
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#pragma once

#include "../widget_impl.h"
#include "../combobox.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			class combobox : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				combobox(const std::string& page_alias,
					const std::string& alias,
					form& fm,
					IDWriteFactory* p_directwrite_factory);
				~combobox();

				/// virtual function overrides
				void press(const bool& pressed) override;
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources();
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				bool hit(const bool& hit) override;
				void on_selection_change(const bool& selected) override;

				/// widget specific methods
				widgets::combobox::combobox_specs& specs();
				widgets::combobox::combobox_specs& operator()();
				void insert_character(const char& c);
				void key_backspace();
				void key_delete();
				void key_left();
				void key_right();
				void key_return();

			private:
				/// Prevent the use of the default constructor.
				combobox();

				/// Prevent copying an object of this class.
				combobox(const combobox&);
				combobox& operator=(const combobox&);

				/// Private variables
				widgets::combobox::combobox_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_caret_;
				ID2D1SolidColorBrush* p_brush_fill_;
				ID2D1SolidColorBrush* p_brush_fill_editable_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				ID2D1SolidColorBrush* p_brush_border_;
				ID2D1SolidColorBrush* p_brush_dropdown_;
				ID2D1SolidColorBrush* p_brush_dropdown_hot_;
				ID2D1SolidColorBrush* p_brush_dropdown_arrow_;
				ID2D1SolidColorBrush* p_brush_dropdown_arrow_hot_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				D2D1_RECT_F rect_dropdown_, rect_text_, rect_combobox_;

				form& fm_;
				HCURSOR h_cursor_edit_, h_cursor_dropdown_;

				const float margin_x_;
				const float margin_y_;

				const std::string caret_blink_timer_name_;
				UINT32 caret_position_;
				bool caret_visible_;
				bool skip_blink_;
				float text_off_set_;
				bool is_selecting_;
				bool is_selected_;
				bool dropdown_activated_;
				bool skip_nextdropdown_;

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

				static bool is_alpha(const std::string& s);
				void sort_items();

				std::string dropdown(D2D1_RECT_F rect);
			};
		}
	}
}
