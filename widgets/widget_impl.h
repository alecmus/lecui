//
// widget_impl.h - widget_impl interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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
			class scrollbar_specs : public widget {
				color _color_scrollbar_border = { 180, 180, 180, 255 };
				color _color_hot_pressed = { 221, 221, 221, 255 };
				color _color_background = { 0, 0, 0, 0 };

			public:
				scrollbar_specs() {
					_color_fill = { 241, 241, 241, 255 };
					_color_hot = { 231, 231, 231, 255 };
				}

				// generic widget

				std::string& text() override { return _text; }
				scrollbar_specs& text(const std::string& text) {
					_text = text;
					return *this;
				}

				std::string& tooltip() override { return _tooltip; }
				scrollbar_specs& tooltip(const std::string& tooltip) {
					_tooltip = tooltip;
					return *this;
				}

				lecui::rect& rect() override { return _rect; }
				scrollbar_specs& rect(const lecui::rect& rect) {
					_rect = rect;
					return *this;
				}

				resize_params& on_resize() override { return _on_resize; }
				scrollbar_specs& on_resize(const resize_params& on_resize) {
					_on_resize = on_resize;
					return *this;
				}

				cursor_type& cursor() override { return _cursor; }
				scrollbar_specs& cursor(const cursor_type cursor) {
					_cursor = cursor;
					return *this;
				}

				std::string& font() override { return _font; }
				scrollbar_specs& font(const std::string& font) {
					_font = font;
					return *this;
				}

				float& font_size() override { return _font_size; }
				scrollbar_specs& font_size(const float& font_size) {
					_font_size = font_size;
					return *this;
				}

				color& color_text() override { return _color_text; }
				scrollbar_specs& color_text(const color& color_text) {
					_color_text = color_text;
					return *this;
				}

				color& color_fill() override { return _color_fill; }
				scrollbar_specs& color_fill(const color& color_fill) {
					_color_fill = color_fill;
					return *this;
				}

				color& color_hot() override { return _color_hot; }
				scrollbar_specs& color_hot(const color& color_hot) {
					_color_hot = color_hot;
					return *this;
				}

				color& color_selected() override { return _color_selected; }
				scrollbar_specs& color_selected(const color& color_selected) {
					_color_selected = color_selected;
					return *this;
				}

				color& color_disabled() override { return _color_disabled; }
				scrollbar_specs& color_disabled(const color& color_disabled) {
					_color_disabled = color_disabled;
					return *this;
				}

				// widget specific widget

				color& color_scrollbar_border() { return _color_scrollbar_border; }
				scrollbar_specs& color_scrollbar_border(const color& color_scrollbar_border) {
					_color_scrollbar_border = color_scrollbar_border;
					return *this;
				}

				color& color_hot_pressed() { return _color_hot_pressed; }
				scrollbar_specs& color_hot_pressed(const color& color_hot_pressed) {
					_color_hot_pressed = color_hot_pressed;
					return *this;
				}

				color& color_background() { return _color_background; }
				scrollbar_specs& color_background(const color& color_background) {
					_color_background = color_background;
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
				strength_bar,
			};

			static HCURSOR get_cursor(widgets::widget::cursor_type type) {
				switch (type) {
				case liblec::lecui::widgets::widget::cursor_type::arrow:
					return nullptr;
					break;
				case liblec::lecui::widgets::widget::cursor_type::hand:
					return LoadCursor(nullptr, IDC_HAND);
					break;
				case liblec::lecui::widgets::widget::cursor_type::caret:
					return LoadCursor(nullptr, IDC_IBEAM);
					break;
				default:
					return nullptr;
					break;
				}
			}

			struct badge_resources {
				ID2D1SolidColorBrush* _p_brush_badge = nullptr;
				ID2D1SolidColorBrush* _p_brush_badge_text = nullptr;
				IDWriteTextFormat* _p_text_format_badge = nullptr;
				IDWriteTextLayout* _p_text_layout_badge = nullptr;
			};

			class widget_impl {
				virtual widgets::widget& generic_specs() = 0;

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

				void create_badge_resources(badge_specs& badge,
					ID2D1HwndRenderTarget* p_render_target,
					IDWriteFactory* p_directwrite_factory,
					badge_resources& resources);

				void draw_badge(badge_specs& badge,
					D2D1_RECT_F rect,
					ID2D1HwndRenderTarget* p_render_target,
					IDWriteFactory* p_directwrite_factory,
					badge_resources& resources);

				void discard_badge_resources(badge_resources& resources);

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
					const resize_params& on_resize,
					const float& change_in_width,
					const float& change_in_height);

				/// <summary>For in-widget hit-testing of complex widgets. Called by default in
				/// the public overload.</summary>
				/// <returns>True if the point is over an essential area within the widget,
				/// false otherwise.</returns>
				/// <remarks>Useful for complex widgets with multiple hit points.</remarks>
				virtual bool contains();

				std::string _alias;
				bool _is_static, _hit, _pressed;
				D2D1_RECT_F _rect;
				bool _visible, _is_enabled;
				D2D1_POINT_2F _point;
				bool _selected;
				D2D1_POINT_2F _point_on_press, _point_on_release;

				/// <summary>Flag to track the status of widget resources. Enables the creation of
				/// widget resources on-the-fly and on-demand for best performance rather than
				/// always creating resources for all page widgets in one batch.</summary>
				bool _resources_created;

				/// <summary>The cursor to be displayed when the mouse is over the widget. When
				/// equal to a nullptr the default cursor is used. It is recommended to set within
				/// the widget's on_createresources() method as follows:
				/// _h_cursor = get_cursor(_specs.cursor);</summary>
				HCURSOR _h_cursor;

				containers::page& _page;

				std::string _tooltip_text;
				bool _tooltip_active;
			};
		}
	}
}
