//
// html_editor_impl.h - html_editor_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include <optional>
#include "../widget_impl.h"
#include "../html_editor.h"
#include "../../formatted_text_parser/formatted_text_parser.h"
#include "../../containers/page.h"
#include "../../containers/page/page_impl.h"
#include "../../containers/pane/pane_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {

			class html_editor_impl : public widget_impl {
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
				static std::string alias_font_color();
				static std::string alias_font_color_bar();

				/// constructor and destructor
				html_editor_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~html_editor_impl();

				bool controls_initialized();
				void initialize_controls(bool init);

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_selection_change(const bool& selected) override;

				/// widget specific methods
				widgets::html_editor::html_editor_specs& specs();
				widgets::html_editor::html_editor_specs& operator()();
				void set_pane_specs(containers::pane::pane_specs& html_control,
					containers::pane::pane_specs& html);
				void insert_character(const char& c);
				void key_backspace();
				void key_delete();
				void key_left();
				void key_right();
				void key_up();
				void key_down();

				/// methods for editing the content
				void selection_font(const std::string& font_name);
				void selection_font_size(const float& font_size);
				void selection_bold();
				void selection_italic();
				void selection_underline();
				void selection_strikethrough();
				void selection_color();
				void selection_color(const color& font_color);

				color get_last_color();

			private:
				// Default constructor and copying an object of this class are not allowed
				html_editor_impl() = delete;
				html_editor_impl(const html_editor_impl&) = delete;
				html_editor_impl& operator=(const html_editor_impl&) = delete;

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
				const std::string autoscroll_timer_name_;
				UINT32 caret_position_;
				bool caret_visible_;
				bool skip_blink_;
				bool is_selecting_;
				bool is_selected_;

				bool key_up_scheduled_;
				bool key_down_scheduled_;

				struct selection_info {
					UINT32 start = 0;
					UINT32 end = 0;
				} selection_info_;

				color last_color_;

				float scroll_amount_;

				std::vector<formatted_text_parser::text_range_properties> formatting_;

				std::optional<std::reference_wrapper<containers::pane::pane_specs>>
					html_control_pane_specs_,
					html_pane_specs_;

				/// Private methods.
				void reset_selection();
				void set_selection(const UINT start, const UINT end);

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
