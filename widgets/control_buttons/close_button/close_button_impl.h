//
// close_button_impl.h - close button widget interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../../widget_impl.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class close_button_specs : public widget {
			public:
				// generic widget

				std::string& text() override { return _text; }
				close_button_specs& text(const std::string& text) {
					_text = text;
					return *this;
				}

				std::string& tooltip() override { return _tooltip; }
				close_button_specs& tooltip(const std::string& tooltip) {
					_tooltip = tooltip;
					return *this;
				}

				badge_specs& badge() { return _badge; }

				close_button_specs& badge(const badge_specs& badge) {
					_badge = badge;
					return *this;
				}

				lecui::rect& rect() override { return _rect; }
				close_button_specs& rect(const lecui::rect& rect) {
					_rect = rect;
					return *this;
				}

				resize_params& on_resize() override { return _on_resize; }
				close_button_specs& on_resize(const resize_params& on_resize) {
					_on_resize = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return _cursor; }
				close_button_specs& cursor(const cursor_type cursor) {
					_cursor = cursor;
					return *this;
				}

				std::string& font() override { return _font; }
				close_button_specs& font(const std::string& font) {
					_font = font;
					return *this;
				}

				float& font_size() override { return _font_size; }
				close_button_specs& font_size(const float& font_size) {
					_font_size = font_size;
					return *this;
				}

				color& color_text() override { return _color_text; }
				close_button_specs& color_text(const color& color_text) {
					_color_text = color_text;
					return *this;
				}

				color& color_fill() override { return _color_fill; }
				close_button_specs& color_fill(const color& color_fill) {
					_color_fill = color_fill;
					return *this;
				}

				color& color_hot() override { return _color_hot; }
				close_button_specs& color_hot(const color& color_hot) {
					_color_hot = color_hot;
					return *this;
				}

				color& color_selected() override { return _color_selected; }
				close_button_specs& color_selected(const color& color_selected) {
					_color_selected = color_selected;
					return *this;
				}

				color& color_disabled() override { return _color_disabled; }
				close_button_specs& color_disabled(const color& color_disabled) {
					_color_disabled = color_disabled;
					return *this;
				}

				// widget specific widget

			};

			class close_button_impl : public widget_impl {
				/// private virtual function overrides
				widgets::widget& generic_specs() override {
					return _specs;
				}

			public:
				close_button_impl(containers::page& page);
				~close_button_impl();

				/// virtual function overrides
				widget_type type() override;
				HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) override;
				void discard_resources() override;
				D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) override;

				/// widget specific methods
				widgets::close_button_specs& specs();
				widgets::close_button_specs& operator()();

			private:
				// Default constructor and copying an object of this class are not allowed
				close_button_impl() = delete;
				close_button_impl(const close_button_impl&) = delete;
				close_button_impl& operator=(const close_button_impl&) = delete;

				widgets::close_button_specs _specs;
				ID2D1SolidColorBrush* _p_brush;
				ID2D1SolidColorBrush* _p_brush_hot;
				ID2D1SolidColorBrush* _p_brush_disabled;
			};
		}
	}
}
