//
// maximize_button_impl.h - maximize button widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class maximize_button_specs : public specs {
			public:
				// generic specs

				std::string& text() override { return _text; }
				maximize_button_specs& text(const std::string& text) {
					_text = text;
					return *this;
				}

				std::string& tooltip() override { return _tooltip; }
				maximize_button_specs& tooltip(const std::string& tooltip) {
					_tooltip = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return _rect; }
				maximize_button_specs& rect(const lecui::rect& rect) {
					_rect = rect;
					return *this;
				}

				resize_params& on_resize() override { return _on_resize; }
				maximize_button_specs& on_resize(const resize_params& on_resize) {
					_on_resize = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return _cursor; }
				maximize_button_specs& cursor(const cursor_type cursor) {
					_cursor = cursor;
					return *this;
				}

				std::string& font() override { return _font; }
				maximize_button_specs& font(const std::string& font) {
					_font = font;
					return *this;
				}

				float& font_size() override { return _font_size; }
				maximize_button_specs& font_size(const float& font_size) {
					_font_size = font_size;
					return *this;
				}

				color& color_text() override { return _color_text; }
				maximize_button_specs& color_text(const color& color_text) {
					_color_text = color_text;
					return *this;
				}

				color& color_fill() override { return _color_fill; }
				maximize_button_specs& color_fill(const color& color_fill) {
					_color_fill = color_fill;
					return *this;
				}

				color& color_hot() override { return _color_hot; }
				maximize_button_specs& color_hot(const color& color_hot) {
					_color_hot = color_hot;
					return *this;
				}

				color& color_selected() override { return _color_selected; }
				maximize_button_specs& color_selected(const color& color_selected) {
					_color_selected = color_selected;
					return *this;
				}

				color& color_disabled() override { return _color_disabled; }
				maximize_button_specs& color_disabled(const color& color_disabled) {
					_color_disabled = color_disabled;
					return *this;
				}

				// widget specific specs

			};

			class maximize_button_impl : public widget_impl {
				/// private virtual function overrides
				widgets::specs& generic_specs() override {
					return _specs;
				}

			public:
				maximize_button_impl(containers::page& page);
				~maximize_button_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;
				void on_click() override;

				/// widget specific methods
				void set_hwnd(HWND hWnd);
				widgets::maximize_button_specs& specs();
				widgets::maximize_button_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				maximize_button_impl() = delete;
				maximize_button_impl(const maximize_button_impl&) = delete;
				maximize_button_impl& operator=(const maximize_button_impl&) = delete;

				/// Private variables
				HWND _hWnd;
				widgets::maximize_button_specs _specs;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;

				/// Private methods
				bool maximized(HWND hwnd);
			};
		}
	}
}
