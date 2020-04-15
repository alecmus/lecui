/*
** label_impl.h - label_impl interface
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
#include "../label.h"

namespace liblec {
	namespace lecui {
		namespace widgets_impl {
			struct text_range_properties {
				DWRITE_TEXT_RANGE text_range = { 0, 0 };
				std::string font;
				float size = .0f;
				bool bold = false;
				bool italic = false;
				bool underline = false;
				D2D1_COLOR_F color = { 0.f, 0.f, 0.f, 1.f };
			};

			void parse_formatted_text(const std::string& formatted_text,
				std::string& plain_text_,
				std::vector<text_range_properties>& formatting_);

			void apply_formatting(const std::vector<text_range_properties>& formatting_,
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

			class label : public widget {
			public:
				/// constructor and destructor
				label(const std::string& page_alias,
					const std::string& alias,
					IDWriteFactory* p_directwrite_factory);
				~label();

				/// virtual function overrides
				widgets_impl::widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				widgets::label::label_specs& specs();

			private:
				/// Prevent the use of the default constructor.
				label() :
					label(std::string(), std::string(), nullptr) {}

				/// Prevent copying an object of this class.
				label(const label&);
				label& operator=(const label&);

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
				std::vector<text_range_properties> formatting_;
			};
		}
	}
}
