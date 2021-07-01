//
// widget_impl.h - widget_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../form_common.h"
#include "widget.h"
#include "../containers/page.h"
#include "tooltip.h"

#include <memory>

namespace liblec {
	namespace lecui {
		namespace widgets {
			class scrollbar_specs : public specs {
				color color_scrollbar_border_ = { 180, 180, 180, 255 };
				color color_hot_pressed_ = { 221, 221, 221, 255 };
				color color_background_ = { 0, 0, 0, 0 };

			public:
				scrollbar_specs() {
					color_fill_ = { 241, 241, 241, 255 };
					color_hot_ = { 231, 231, 231, 255 };
				}

				// generic specs

				std::string& text() override { return text_; }
				scrollbar_specs& text(const std::string& text) {
					text_ = text;
					return *this;
				}

				std::string& tooltip() override { return tooltip_; }
				scrollbar_specs& tooltip(const std::string& tooltip) {
					tooltip_ = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return rect_; }
				scrollbar_specs& rect(const lecui::rect& rect) {
					rect_ = rect;
					return *this;
				}

				resize_params& on_resize() override { return on_resize_; }
				scrollbar_specs& on_resize(const resize_params& on_resize) {
					on_resize_ = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return cursor_; }
				scrollbar_specs& cursor(const cursor_type cursor) {
					cursor_ = cursor;
					return *this;
				}

				std::string& font() override { return font_; }
				scrollbar_specs& font(const std::string& font) {
					font_ = font;
					return *this;
				}

				float& font_size() override { return font_size_; }
				scrollbar_specs& font_size(const float& font_size) {
					font_size_ = font_size;
					return *this;
				}

				color& color_text() override { return color_text_; }
				scrollbar_specs& color_text(const color& color_text) {
					color_text_ = color_text;
					return *this;
				}

				color& color_fill() override { return color_fill_; }
				scrollbar_specs& color_fill(const color& color_fill) {
					color_fill_ = color_fill;
					return *this;
				}

				color& color_hot() override { return color_hot_; }
				scrollbar_specs& color_hot(const color& color_hot) {
					color_hot_ = color_hot;
					return *this;
				}

				color& color_selected() override { return color_selected_; }
				scrollbar_specs& color_selected(const color& color_selected) {
					color_selected_ = color_selected;
					return *this;
				}

				color& color_disabled() override { return color_disabled_; }
				scrollbar_specs& color_disabled(const color& color_disabled) {
					color_disabled_ = color_disabled;
					return *this;
				}

				// widget specific specs

				color& color_scrollbar_border() { return color_scrollbar_border_; }
				scrollbar_specs& color_scrollbar_border(const color& color_scrollbar_border) {
					color_scrollbar_border_ = color_scrollbar_border;
					return *this;
				}

				color& color_hot_pressed() { return color_hot_pressed_; }
				scrollbar_specs& color_hot_pressed(const color& color_hot_pressed) {
					color_hot_pressed_ = color_hot_pressed;
					return *this;
				}

				color& color_background() { return color_background_; }
				scrollbar_specs& color_background(const color& color_background) {
					color_background_ = color_background;
					return *this;
				}
			};

			enum class widget_type {
				close_button,
				maximize_button,
				minimize_button,
				h_scrollbar,
				v_scrollbar,

				tab_pane,
				pane,

				rectangle,
				label,
				group,
				button,
				toggle,
				table_view,
				custom,
				image_view,
				progress_indicator,
				progress_bar,
				checkbox,
				text_field,
				tree_view,
				slider,
				html_editor,
				combobox,
				line,
				time,
				date,
				icon,
			};

			static HCURSOR get_cursor(widgets::specs::cursor_type type) {
				switch (type) {
				case liblec::lecui::widgets::specs::cursor_type::arrow:
					return nullptr;
					break;
				case liblec::lecui::widgets::specs::cursor_type::hand:
					return LoadCursor(nullptr, IDC_HAND);
					break;
				case liblec::lecui::widgets::specs::cursor_type::caret:
					return LoadCursor(nullptr, IDC_IBEAM);
					break;
				default:
					return nullptr;
					break;
				}
			}

			class widget_impl {
				virtual widgets::specs& generic_specs() = 0;

			public:
				widget_impl() = delete;
				widget_impl(containers::page& page, const std::string& alias);
				virtual ~widget_impl();

				const std::string& alias();
				bool pressed();
				bool is_static();
				void show(const bool& show);
				bool visible();
				void enable(const bool& enable);
				bool enabled();
				const D2D1_RECT_F& get_rect();
				void select(const bool& selected);
				bool selected();
				bool hit();
				HCURSOR cursor();
				float get_dpi_scale();
				form& get_form();
				void show_tooltip();
				void hide_tooltip();

				virtual void press(const bool& pressed);

				/// <summary>Check whether a point is within the widget.</summary>
				/// <param name="point">The point, in pixels.</param>
				/// <returns>True if point is within the widget, false otherwise.</returns>
				/// <remarks>For in-widget hit-testing avoid overriding this virtual function
				/// directly; rather use the protected overload. Also remember to factor in the
				/// dpi scale otherwise hit testing will not work properly in high-dpi
				/// environments.</remarks>
				virtual bool contains(const D2D1_POINT_2F& point);

				/// <summary>Set the widget hit status.</summary>
				/// <param name="hit">True if mouse is over widget, else false.</param>
				/// <returns>True if the hit status has changed, false otherwise.</returns>
				/// <remarks>When true is returned the UI is refreshed.</remarks>
				virtual bool hit(const bool& hit);
				virtual widget_type type() = 0;
				virtual HRESULT create_resources(ID2D1HwndRenderTarget* p_render_target) = 0;
				virtual void discard_resources() = 0;
				virtual D2D1_RECT_F& render(ID2D1HwndRenderTarget* p_render_target,
					const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset,
					const bool& render) = 0;
				virtual void on_click();
				virtual void on_right_click();
				virtual void on_action();
				virtual bool on_mousewheel(float units);
				virtual bool on_keydown(WPARAM wParam);
				virtual void on_selection_change(const bool& selected);

			protected:
				D2D1_RECT_F position(const rect& rect,
					const widgets::specs::resize_params& on_resize,
					const float& change_in_width,
					const float& change_in_height);

				/// <summary>For in-widget hit-testing of complex widgets. Called by default in
				/// the public overload.</summary>
				/// <returns>True if the point is over an essential area within the widget,
				/// false otherwise.</returns>
				/// <remarks>Useful for complex widgets with multiple hit points.</remarks>
				virtual bool contains();

				std::string alias_;
				bool is_static_, hit_, pressed_;
				D2D1_RECT_F rect_;
				bool visible_, is_enabled_;
				D2D1_POINT_2F point_;
				bool selected_;
				D2D1_POINT_2F point_on_press_, point_on_release_;

				/// <summary>Flag to track the status of widget resources. Enables the creation of
				/// widget resources on-the-fly and on-demand for best performance rather than
				/// always creating resources for all page widgets in one batch.</summary>
				bool resources_created_;

				/// <summary>The cursor to be displayed when the mouse is over the widget. When
				/// equal to a nullptr the default cursor is used. It is recommended to set within
				/// the widget's on_createresources() method as follows:
				/// h_cursor_ = get_cursor(specs_.cursor);</summary>
				HCURSOR h_cursor_;

				containers::page& page_;

				std::string tooltip_text_;
				bool tooltip_active_;
			};
		}
	}
}
