//
// appearance.cpp - appearance implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "appearance.h"
#include "form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		lecui::color defaults::color(themes theme, item wc) {
			switch (theme) {
				/// Light theme
			case themes::light: {
				switch (wc) {
				case item::form: return { 244, 244, 244, 255 };
				case item::titlebar: return defaults::color(theme, item::form);
				case item::accent: return { 0, 120, 170, 255 };
				case item::accent_hover: return { 255, 180, 0, 255 };
				case item::disabled: return { 225, 225, 225, 255 };

				case item::scrollbar: return { 241, 241, 241, 255 };
				case item::scrollbar_border: return { 180, 180, 180, 255 };
				case item::scrollbar_hover: return { 231, 231, 231, 255 };
				case item::scrollbar_pressed: return { 221, 221, 221, 255 };

				case item::label: return { 0, 0, 0, 255 };
				case item::label_selected: return { 0, 120, 170, 30 };
				case item::label_hover: return { 0, 120, 170, 20 };
				case item::label_pressed: return { 0, 120, 170, 40 };
				case item::label_disabled: return { 180, 180, 180, 255 };

				case item::button: return { 229, 229, 229, 255 };
				case item::button_border: return { 150, 150, 150, 255 };
				case item::button_hover: return { 240, 240, 240, 255 };
				case item::button_selected: return { 0, 120, 170, 100 };

				case item::checkbox: return { 229, 229, 229, 255 };
				case item::checkbox_border: return { 150, 150, 150, 255 };
				case item::checkbox_tick: return { 0, 120, 170, 255 };
				case item::checkbox_hover: return { 240, 240, 240, 255 };
				case item::checkbox_selected: return { 0, 120, 170, 100 };

				case item::progress_bar: return { 0, 120, 170, 255 };
				case item::progress_bar_border: return { 200, 200, 200, 255 };

				case item::progress_indicator: return { 0, 120, 170, 255 };
				case item::progress_indicator_text: return { 0, 120, 170, 255 };
				case item::progress_indicator_unfilled: return { 200, 200, 200, 255 };

				case item::text_field: return { 255, 255, 255, 255 };
				case item::text_field_border: return { 150, 150, 150, 255 };
				case item::text_field_disabled: return { 248, 248, 248, 255 };
				case item::text_field_prompt: return { 180, 180, 180, 255 };
				case item::text_field_selected: return { 0, 120, 170, 100 };
				case item::text_field_caret: return { 50, 50, 50, 255 };
				case item::text_field_input_error: return { 255, 0, 0, 150 };

				case item::table_view: return { 255, 255, 255, 255 };
				case item::table_view_border: return { 150, 150, 150, 255 };
				case item::table_view_alternate: return { 0, 120, 170, 15 };
				case item::table_view_row_hover: return { 225, 242, 255, 255 };
				case item::table_view_row_selected: return { 40, 160, 200, 255 };
				case item::table_view_text_header: return { 0, 0, 0, 255 };
				case item::table_view_text_header_hot: return defaults::color(theme, item::form);
				case item::table_view_text_selected: return defaults::color(theme, item::form);
				case item::table_view_header: return { 255, 255, 255, 255 };
				case item::table_view_grid: return { 215, 215, 215, 255 };

				case item::toggle: return { 255, 255, 255, 200 };
				case item::toggle_on: return { 0, 120, 170, 255 };
				case item::toggle_off: return { 150, 150, 150, 255 };
				case item::toggle_selected: return { 0, 120, 170, 30 };

				case item::tree_view: return { 255, 255, 255, 255 };
				case item::tree_view_border: return { 150, 150, 150, 255 };
				case item::tree_view_hover: return { 225, 242, 255, 255 };
				case item::tree_view_selected: return { 0, 120, 170, 100 };

				case item::tab_pane: return { 100, 100, 100, 15 };
				case item::tab_pane_border: return { 0, 120, 170, 50 };
				case item::tab_bar: return { 0, 120, 170, 15 };
				case item::tab_selected: return { 0, 120, 170, 255 };

				case item::pane: return { 100, 100, 100, 15 };
				case item::pane_border: return { 0, 120, 170, 50 };

				case item::image_view: return { 222, 222, 222, 0 };
				case item::image_view_border: return { 150, 150, 150, 0 };
				case item::image_view_hover: return { 0, 120, 170, 100 };
				case item::image_view_selected: return { 0, 120, 170, 150 };

				case item::rectangle: return { 222, 222, 222, 255 };
				case item::rectangle_border: return { 150, 150, 150, 255 };
				case item::rectangle_border_hot: return { 0, 120, 170, 50 };
				case item::rectangle_hover: return { 0, 120, 170, 100 };
				case item::rectangle_selected: return { 0, 120, 170, 100 };

				case item::group: return { 0, 120, 170, 25 };
				case item::group_border: return { 0, 120, 170, 100 };

				case item::slider: return { 234, 234, 234, 255 };
				case item::slider_border: return { 200, 200, 200, 255 };
				case item::slider_knob: return { 215, 215, 225, 255 };
				case item::slider_knob_hot: return { 225, 225, 235, 255 };
				case item::slider_knob_border: return { 0, 120, 170, 50 };
				case item::slider_tick: return { 120, 120, 120, 255 };
				case item::slider_selected: return { 0, 120, 170, 150 };

				case item::html_editor: return { 255, 255, 255, 255 };
				case item::html_editor_border: return { 150, 150, 150, 255 };
				case item::html_editor_disabled: return { 248, 248, 248, 255 };
				case item::html_editor_selected: return { 0, 120, 170, 100 };
				case item::html_editor_caret: return { 50, 50, 50, 255 };
				case item::html_control: return { 100, 100, 100, 7 };
				case item::html_control_border: return { 0, 120, 170, 50 };

				case item::combobox: return { 219, 219, 219, 255 };
				case item::combobox_editable: return { 255, 255, 255, 255 };
				case item::combobox_caret: return { 50, 50, 50, 255 };
				case item::combobox_border: return { 200, 200, 200, 255 };
				case item::combobox_hover: return { 240, 240, 240, 255 };
				case item::combobox_selected: return { 0, 120, 170, 100 };
				case item::combobox_dropdown: return { 219, 219, 219, 255 };
				case item::combobox_dropdown_hot: return { 0, 120, 170, 255 };
				case item::combobox_dropdown_arrow: return { 150, 150, 150, 255 };
				case item::combobox_dropdown_arrow_hot: return { 244, 244, 244, 255 };

				case item::line: return { 150, 150, 150, 255 };
				case item::line_hover: return { 0, 120, 170, 50 };
				case item::line_selected: return { 0, 120, 170, 100 };

				case item::icon: return { 100, 100, 100, 0 };
				case item::icon_border: return { 0, 120, 170, 0 };
				case item::icon_hot: return { 0, 120, 170, 100 };
				case item::icon_text: return defaults::color(theme, item::label);
				case item::icon_description_text: return { 150, 150, 150, 255 };
				default: break;
				}
			} break;

				/// Dark theme
			case themes::dark: {
				switch (wc) {
				case item::form: return { 16, 29, 36, 255 };
				case item::titlebar: return defaults::color(theme, item::form);
				case item::accent: return { 60, 190, 175, 255 };
				case item::accent_hover: return { 255, 255, 255, 255 };
				case item::disabled: return { 30, 30, 30, 255 };

				case item::scrollbar: return { 60, 65, 75, 255 };
				case item::scrollbar_border: return { 30, 35, 45, 255 };
				case item::scrollbar_hover: return { 80, 85, 95, 255 };
				case item::scrollbar_pressed: return { 100, 105, 115, 255 };

				case item::label: return { 155, 165, 180, 255 };
				case item::label_selected: return { 60, 190, 175, 40 };
				case item::label_hover: return { 60, 190, 175, 20 };
				case item::label_pressed: return { 60, 190, 175, 60 };
				case item::label_disabled: return { 85, 95, 100, 255 };

				case item::button: return { 60, 65, 75, 255 };
				case item::button_border: return { 150, 150, 150, 255 };
				case item::button_hover: return { 80, 85, 95, 255 };
				case item::button_selected: return { 60, 190, 175, 150 };

				case item::checkbox: return { 60, 65, 75, 255 };
				case item::checkbox_border: return { 150, 150, 150, 255 };
				case item::checkbox_tick: return { 60, 190, 175, 255 };
				case item::checkbox_hover: return { 80, 85, 95, 255 };
				case item::checkbox_selected: return { 60, 190, 175, 150 };

				case item::progress_bar: return { 60, 190, 175, 255 };
				case item::progress_bar_border: return { 85, 95, 100, 255 };

				case item::progress_indicator: return { 60, 190, 175, 255 };
				case item::progress_indicator_text: return { 60, 190, 175, 255 };
				case item::progress_indicator_unfilled: return { 85, 95, 100, 255 };

				case item::text_field: return { 40, 45, 50, 255 };
				case item::text_field_border: return { 150, 150, 150, 255 };
				case item::text_field_disabled: return { 30, 35, 40, 255 };
				case item::text_field_prompt: return { 70, 75, 80, 255 };
				case item::text_field_selected: return { 60, 190, 175, 150 };
				case item::text_field_caret: return { 60, 190, 175, 255 };
				case item::text_field_input_error: return { 255, 0, 0, 220 };

				case item::table_view: return { 16, 29, 36, 255 };
				case item::table_view_border: return { 150, 150, 150, 255 };
				case item::table_view_alternate: return { 60, 190, 175, 15 };
				case item::table_view_row_hover: return { 40, 160, 200, 100 };
				case item::table_view_row_selected: return { 40, 160, 200, 255 };
				case item::table_view_text_header: return { 155, 165, 180, 255 };
				case item::table_view_text_header_hot: return defaults::color(theme, item::form);
				case item::table_view_text_selected: return defaults::color(theme, item::form);
				case item::table_view_header: return { 16, 29, 36, 255 };
				case item::table_view_grid: return { 55, 60, 65, 255 };

				case item::toggle: return { 255, 255, 255, 200 };
				case item::toggle_on: return { 0, 120, 170, 255 };
				case item::toggle_off: return { 150, 150, 150, 255 };
				case item::toggle_selected: return { 60, 190, 175, 60 };

				case item::tree_view: return { 16, 29, 36, 255 };
				case item::tree_view_border: return { 150, 150, 150, 255 };
				case item::tree_view_hover: return { 60, 65, 70, 255 };
				case item::tree_view_selected: return { 0, 120, 170, 100 };

				case item::tab_pane: return { 100, 100, 100, 15 };
				case item::tab_pane_border: return { 0, 120, 170, 50 };
				case item::tab_bar: return { 0, 120, 170, 15 };
				case item::tab_selected: return { 60, 190, 175, 255 };

				case item::pane: return { 100, 100, 100, 15 };
				case item::pane_border: return { 0, 120, 170, 50 };

				case item::image_view: return { 222, 222, 222, 0 };
				case item::image_view_border: return { 150, 150, 150, 0 };
				case item::image_view_hover: return { 60, 190, 175, 100 };
				case item::image_view_selected: return { 60, 190, 175, 150 };

				case item::rectangle: return { 222, 222, 222, 255 };
				case item::rectangle_border: return { 150, 150, 150, 255 };
				case item::rectangle_border_hot: return { 60, 190, 175, 50 };
				case item::rectangle_hover: return { 60, 190, 175, 100 };
				case item::rectangle_selected: return { 60, 190, 175, 100 };

				case item::group: return { 60, 190, 175, 25 };
				case item::group_border: return { 60, 190, 175, 100 };

				case item::slider: return { 26, 39, 46, 255 };
				case item::slider_border: return { 70, 70, 70, 255 };
				case item::slider_knob: return { 60, 65, 75, 255 };
				case item::slider_knob_hot: return { 80, 85, 95, 255 };
				case item::slider_knob_border: return { 90, 90, 90, 255 };
				case item::slider_tick: return { 85, 95, 100, 255 };
				case item::slider_selected: return { 60, 190, 175, 150 };

				case item::html_editor: return { 40, 45, 50, 255 };
				case item::html_editor_border: return { 150, 150, 150, 255 };
				case item::html_editor_disabled: return { 30, 35, 40, 255 };
				case item::html_editor_selected: return { 60, 190, 175, 150 };
				case item::html_editor_caret: return { 60, 190, 175, 255 };
				case item::html_control: return { 100, 100, 100, 7 };
				case item::html_control_border: return { 0, 120, 170, 50 };

				case item::combobox: return { 60, 65, 75, 255 };
				case item::combobox_editable: return { 40, 45, 50, 255 };
				case item::combobox_caret: return { 60, 190, 175, 255 };
				case item::combobox_border: return { 70, 70, 70, 255 };
				case item::combobox_hover: return { 80, 85, 95, 255 };
				case item::combobox_selected: return { 60, 190, 175, 150 };
				case item::combobox_dropdown: return { 60, 65, 75, 255 };
				case item::combobox_dropdown_hot: return { 60, 190, 175, 255 };
				case item::combobox_dropdown_arrow: return { 150, 150, 150, 255 };
				case item::combobox_dropdown_arrow_hot: return { 16, 29, 36, 255 };

				case item::line: return { 150, 150, 150, 255 };
				case item::line_hover: return { 60, 190, 175, 50 };
				case item::line_selected: return { 60, 190, 175, 100 };

				case item::icon: return { 100, 100, 100, 0 };
				case item::icon_border: return { 0, 120, 170, 0 };
				case item::icon_hot: return { 60, 190, 175, 100 };
				case item::icon_text: return defaults::color(theme, item::label);
				case item::icon_description_text: return { 125, 125, 125, 255 };
				default: break;
				}
			} break;

			default: break;
			}

			return { 0, 0, 0, 255 };
		}

		class appearance::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		appearance::appearance(form& fm) :
			_d(*(new impl(fm))) {}

		appearance::~appearance() { delete& _d; }

		appearance& appearance::shadow(const bool& enable) {
			if (IsWindow(_d._fm._d._hWnd))
				_d._fm._d.set_borderless_shadow(_d._fm._d._hWnd, enable);
			else
				_d._fm._d._borderless_shadow = enable;

			return *this;
		}

		appearance& appearance::theme(themes theme) {
			_d._fm._d._theme = theme;

			// change the background color
			background(defaults::color(theme, item::form),
				defaults::color(theme, item::titlebar));

			// change the theme colors
			appearance::theme(defaults::color(theme, item::accent),
				defaults::color(theme, item::accent_hover),
				defaults::color(theme, item::disabled),
				defaults::color(theme, item::label));

			return *this;
		}

		themes appearance::theme() {
			return _d._fm._d._theme;
		}

		appearance& appearance::theme(const lecui::color& color,
			const lecui::color& color_hot,
			const lecui::color& color_disabled,
			const lecui::color& color_text) {
			_d._fm._d._clr_theme = color;
			_d._fm._d._clr_theme_hot = color_hot;
			_d._fm._d._clr_theme_disabled = color_disabled;
			_d._fm._d._clr_theme_text = color_text;

			return *this;
		}

		appearance& appearance::background(const lecui::color& color,
			const lecui::color& color_titlebar) {
			_d._fm._d._clr_background = color;
			_d._fm._d._clr_titlebar_background = color_titlebar;

			return *this;
		}

		appearance& appearance::set_icons(int icon_resource,
			int small_icon_resource) {
			_d._fm._d._idi_icon = icon_resource;
			_d._fm._d._idi_icon_small = small_icon_resource;

			return *this;
		}
	}
}
