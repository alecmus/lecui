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
				std::string& _plain_text,
				D2D1_COLOR_F default_color,
				std::vector<formatted_text_parser::text_range_properties>& _formatting);

			void apply_formatting(const std::vector<formatted_text_parser::text_range_properties>& _formatting,
				ID2D1HwndRenderTarget* p_render_target,
				IDWriteTextLayout* _p_text_layout,
				bool is_enabled,
				ID2D1SolidColorBrush* p_brush_disabled);

			D2D1_RECT_F measure_label(IDWriteFactory* _p_directwrite_factory,
				const std::string& formatted_text,
				const std::string& font,
				const float font_size,
				bool center_h,
				bool center_v,
				const D2D1_RECT_F max_rect);

			D2D1_RECT_F measure_text(IDWriteFactory* _p_directwrite_factory,
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
				widgets::widget& generic_specs() override {
					return _specs;
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
				widgets::label& specs();
				widgets::label& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				label_impl() = delete;
				label_impl(const label_impl&) = delete;
				label_impl& operator=(const label_impl&) = delete;

				/// Private variables
				widgets::label _specs, _specs_old;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_hot_pressed;
				ID2D1SolidColorBrush* _p_brush_disabled;
				ID2D1SolidColorBrush* _p_brush_selected;
				IDWriteTextFormat* _p_text_format;

				IDWriteFactory* _p_directwrite_factory;
				IDWriteTextLayout* _p_text_layout;

				std::string _text;
				std::vector<formatted_text_parser::text_range_properties> _formatting;
			};
		}
	}
}
