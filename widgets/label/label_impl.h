//
// label_impl.h - label_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../widget_impl.h"
#include "../label.h"
#include "../../formatted_text_parser/formatted_text_parser.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			void parse_formatted_text(const std::string& formatted_text,
				std::string& plain_text_,
				D2D1_COLOR_F default_color,
				std::vector<formatted_text_parser::text_range_properties>& formatting_);

			void apply_formatting(const std::vector<formatted_text_parser::text_range_properties>& formatting_,
				ID2D1HwndRenderTarget* p_render_target,
				IDWriteTextLayout* p_text_layout_,
				bool is_enabled,
				ID2D1SolidColorBrush* p_brush_disabled);

			D2D1_RECT_F measure_label(IDWriteFactory* p_directwrite_factory_,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				const D2D1_RECT_F max_rect);

			D2D1_RECT_F measure_text(IDWriteFactory* p_directwrite_factory_,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				bool allow_h_overflow,
				bool allow_v_overflow,
				const D2D1_RECT_F max_rect);

			class label_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return specs_;
				}

			public:
				/// constructor and destructor
				label_impl(containers::page& page,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~label_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::label::label_specs& specs();
				widgets::label::label_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				label_impl() = delete;
				label_impl(const label_impl&) = delete;
				label_impl& operator=(const label_impl&) = delete;

				/// Private variables
				widgets::label::label_specs specs_, specs_old_;
				ID2D1SolidColorBrush* p_brush_;
				ID2D1SolidColorBrush* p_brush_hot_;
				ID2D1SolidColorBrush* p_brush_hot_pressed_;
				ID2D1SolidColorBrush* p_brush_disabled_;
				ID2D1SolidColorBrush* p_brush_selected_;
				IDWriteTextFormat* p_text_format_;

				IDWriteFactory* p_directwrite_factory_;
				IDWriteTextLayout* p_text_layout_;

				std::string text_;
				std::vector<formatted_text_parser::text_range_properties> formatting_;
			};
		}
	}
}
