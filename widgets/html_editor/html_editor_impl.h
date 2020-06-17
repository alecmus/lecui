/*
** html_editor_impl.h - html_editor_impl interface
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
#include "../html_editor.h"
#include "../../formatted_text_parser/formatted_text_parser.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {

			class html_editor : public widget {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				static std::string alias_font();
				static std::string alias_font_size();
				static std::string alias_bold();
				static std::string alias_italic();
				static std::string alias_underline();
				static std::string alias_strikethrough();

				/// constructor and destructor
				html_editor(const std::string& page_alias,
					const std::string& alias,
					form& fm,
					IDWriteFactory* p_directwrite_factory);
				~html_editor();

				bool controls_initialized();
				void initialize_controls(bool init);

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;
				void on_selection_change(const bool& selected) override;

				/// widget specific methods
				widgets::html_editor::html_editor_specs& specs();
				widgets::html_editor::html_editor_specs& operator()();
				void insert_character(const char& c);
				void key_backspace();
				void key_delete();
				void key_left();
				void key_right();

				/// methods for editing the content
				void selection_font(const std::string& font_name);
				void selection_font_size(const float& font_size);
				void selection_bold();
				void selection_italic();
				void selection_underline();
				void selection_strikethrough();

			private:
				/// Prevent copying an object of this class.
				html_editor(const html_editor&);
				html_editor& operator=(const html_editor&);

				/// Private variables
				bool controls_initialized_;
				widgets::html_editor::html_editor_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_caret_;
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
				bool is_selecting_;
				bool is_selected_;

				struct selection_info {
					UINT32 start = 0;
					UINT32 end = 0;
				} selection_info_;

				form& fm_;

				std::vector<formatted_text_parser::text_range_properties> formatting_;

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

				static std::vector<D2D1_RECT_F> get_selection_rects(IDWriteTextLayout* p_text_layout,
					const D2D1_RECT_F& rect_text, const UINT32& selection_start,
					const UINT32& selection_end);

				static float get_caret_width();

				static D2D1_RECT_F get_caret_rect(IDWriteTextLayout* p_text_layout,
					const D2D1_RECT_F& rect_text, const UINT32& caret_position);
			};
		}
	}
}