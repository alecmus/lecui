//
// appearance.cpp - appearance implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "appearance.h"
#include "form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		lecui::color defaults::color(themes theme, element wc) {
			switch (theme) {
				/// Light theme
			case themes::light: {
				switch (wc) {
				case element::form: return { 244, 244, 244, 255 };
				case element::titlebar: return defaults::color(theme, element::form);
				case element::accent: return { 0, 120, 170, 255 };
				case element::accent_hover: return { 255, 180, 0, 255 };
				case element::disabled: return { 225, 225, 225, 255 };

				case element::scrollbar: return { 241, 241, 241, 255 };
				case element::scrollbar_border: return { 180, 180, 180, 255 };
				case element::scrollbar_hover: return { 231, 231, 231, 255 };
				case element::scrollbar_pressed: return { 221, 221, 221, 255 };

				case element::label: return { 0, 0, 0, 255 };
				case element::label_selected: return { 0, 120, 170, 30 };
				case element::label_hover: return { 0, 120, 170, 20 };
				case element::label_pressed: return { 0, 120, 170, 40 };
				case element::label_disabled: return { 180, 180, 180, 255 };

				case element::button: return { 229, 229, 229, 255 };
				case element::button_border: return { 150, 150, 150, 255 };
				case element::button_hover: return { 240, 240, 240, 255 };
				case element::button_selected: return { 0, 120, 170, 100 };

				case element::checkbox: return { 229, 229, 229, 255 };
				case element::checkbox_border: return { 150, 150, 150, 255 };
				case element::checkbox_tick: return { 0, 120, 170, 255 };
				case element::checkbox_hover: return { 240, 240, 240, 255 };
				case element::checkbox_selected: return { 0, 120, 170, 100 };

				case element::progress_bar: return { 0, 120, 170, 255 };
				case element::progress_bar_border: return { 200, 200, 200, 255 };
				case element::progress_bar_unfilled: return { 234, 234, 234, 255 };

				case element::progress_indicator: return { 0, 120, 170, 255 };
				case element::progress_indicator_text: return { 0, 120, 170, 255 };
				case element::progress_indicator_unfilled: return { 200, 200, 200, 255 };

				case element::text_field: return { 250, 250, 250, 255 };
				case element::text_field_border: return { 150, 150, 150, 255 };
				case element::text_field_disabled: return { 248, 248, 248, 255 };
				case element::text_field_prompt: return { 180, 180, 180, 255 };
				case element::text_field_selected: return { 0, 120, 170, 100 };
				case element::text_field_caret: return { 50, 50, 50, 255 };
				case element::text_field_input_error: return { 255, 0, 0, 150 };

				case element::table_view: return { 255, 255, 255, 255 };
				case element::table_view_border: return { 150, 150, 150, 255 };
				case element::table_view_alternate: return { 0, 120, 170, 15 };
				case element::table_view_row_hover: return { 225, 242, 255, 255 };
				case element::table_view_row_selected: return { 40, 160, 200, 255 };
				case element::table_view_text_header: return { 0, 0, 0, 255 };
				case element::table_view_text_header_hot: return defaults::color(theme, element::form);
				case element::table_view_text_selected: return defaults::color(theme, element::form);
				case element::table_view_header: return { 255, 255, 255, 255 };
				case element::table_view_grid: return { 215, 215, 215, 255 };

				case element::toggle: return { 255, 255, 255, 200 };
				case element::toggle_on: return { 0, 120, 170, 255 };
				case element::toggle_off: return { 150, 150, 150, 255 };
				case element::toggle_selected: return { 0, 120, 170, 30 };

				case element::tree_view: return { 255, 255, 255, 255 };
				case element::tree_view_border: return { 150, 150, 150, 255 };
				case element::tree_view_hover: return { 225, 242, 255, 255 };
				case element::tree_view_selected: return { 0, 120, 170, 100 };

				case element::tab_pane: return { 100, 100, 100, 15 };
				case element::tab_pane_border: return { 0, 120, 170, 50 };
				case element::tab_pane_tabs: return { 0, 120, 170, 15 };
				case element::tab_pane_selected_tab: return { 0, 120, 170, 255 };

				case element::pane: return { 100, 100, 100, 15 };
				case element::pane_border: return { 0, 120, 170, 50 };

				case element::image_view: return { 222, 222, 222, 0 };
				case element::image_view_border: return { 150, 150, 150, 0 };
				case element::image_view_hover: return { 0, 120, 170, 100 };
				case element::image_view_selected: return { 0, 120, 170, 150 };

				case element::rectangle: return { 222, 222, 222, 255 };
				case element::rectangle_border: return { 150, 150, 150, 255 };
				case element::rectangle_border_hot: return { 0, 120, 170, 50 };
				case element::rectangle_hover: return { 0, 120, 170, 100 };
				case element::rectangle_selected: return { 0, 120, 170, 100 };

				case element::group: return { 0, 120, 170, 25 };
				case element::group_border: return { 0, 120, 170, 100 };

				case element::slider: return { 234, 234, 234, 255 };
				case element::slider_border: return { 200, 200, 200, 255 };
				case element::slider_knob: return { 215, 215, 225, 255 };
				case element::slider_knob_hot: return { 225, 225, 235, 255 };
				case element::slider_knob_border: return { 0, 120, 170, 50 };
				case element::slider_tick: return { 120, 120, 120, 255 };
				case element::slider_selected: return { 0, 120, 170, 150 };

				case element::html_editor: return { 250, 250, 250, 255 };
				case element::html_editor_border: return { 150, 150, 150, 255 };
				case element::html_editor_disabled: return { 248, 248, 248, 255 };
				case element::html_editor_selected: return { 0, 120, 170, 100 };
				case element::html_editor_caret: return { 50, 50, 50, 255 };
				case element::html_control: return { 100, 100, 100, 7 };
				case element::html_control_border: return { 0, 120, 170, 50 };

				case element::combobox: return { 219, 219, 219, 255 };
				case element::combobox_editable: return { 250, 250, 250, 255 };
				case element::combobox_caret: return { 50, 50, 50, 255 };
				case element::combobox_border: return { 200, 200, 200, 255 };
				case element::combobox_hover: return { 240, 240, 240, 255 };
				case element::combobox_selected: return { 0, 120, 170, 100 };
				case element::combobox_dropdown: return { 219, 219, 219, 255 };
				case element::combobox_dropdown_hot: return { 0, 120, 170, 255 };
				case element::combobox_dropdown_arrow: return { 150, 150, 150, 255 };
				case element::combobox_dropdown_arrow_hot: return { 244, 244, 244, 255 };

				case element::line: return { 150, 150, 150, 255 };
				case element::line_hover: return { 0, 120, 170, 50 };
				case element::line_selected: return { 0, 120, 170, 100 };

				case element::icon: return { 100, 100, 100, 0 };
				case element::icon_border: return { 0, 120, 170, 0 };
				case element::icon_hot: return { 0, 120, 170, 60 };
				case element::icon_text: return defaults::color(theme, element::label);
				case element::icon_description_text: return { 135, 135, 135, 255 };

				case element::strength_bar: return { 234, 234, 234, 255 };
				case element::strength_bar_border: return { 200, 200, 200, 255 };

				case element::badge: return { 255, 0, 0, 180 };
				case element::badge_border: return { 255, 255, 255, 180 };
				case element::badge_text: return { 255, 255, 255, 255 };
				default: break;
				}
			} break;

				/// Dark theme
			case themes::dark: {
				switch (wc) {
				case element::form: return { 16, 29, 36, 255 };
				case element::titlebar: return defaults::color(theme, element::form);
				case element::accent: return { 60, 190, 175, 255 };
				case element::accent_hover: return { 255, 255, 255, 255 };
				case element::disabled: return { 30, 30, 30, 255 };

				case element::scrollbar: return { 60, 65, 75, 255 };
				case element::scrollbar_border: return { 30, 35, 45, 255 };
				case element::scrollbar_hover: return { 80, 85, 95, 255 };
				case element::scrollbar_pressed: return { 100, 105, 115, 255 };

				case element::label: return { 155, 165, 180, 255 };
				case element::label_selected: return { 60, 190, 175, 40 };
				case element::label_hover: return { 60, 190, 175, 20 };
				case element::label_pressed: return { 60, 190, 175, 60 };
				case element::label_disabled: return { 85, 95, 100, 255 };

				case element::button: return { 60, 65, 75, 255 };
				case element::button_border: return { 150, 150, 150, 255 };
				case element::button_hover: return { 80, 85, 95, 255 };
				case element::button_selected: return { 60, 190, 175, 150 };

				case element::checkbox: return { 60, 65, 75, 255 };
				case element::checkbox_border: return { 150, 150, 150, 255 };
				case element::checkbox_tick: return { 60, 190, 175, 255 };
				case element::checkbox_hover: return { 80, 85, 95, 255 };
				case element::checkbox_selected: return { 60, 190, 175, 150 };

				case element::progress_bar: return { 60, 190, 175, 255 };
				case element::progress_bar_border: return { 85, 95, 100, 255 };
				case element::progress_bar_unfilled: return { 26, 39, 46, 255 };

				case element::progress_indicator: return { 60, 190, 175, 255 };
				case element::progress_indicator_text: return { 60, 190, 175, 255 };
				case element::progress_indicator_unfilled: return { 85, 95, 100, 255 };

				case element::text_field: return { 14, 26, 32, 255 };
				case element::text_field_border: return { 150, 150, 150, 255 };
				case element::text_field_disabled: return { 30, 35, 40, 255 };
				case element::text_field_prompt: return { 70, 75, 80, 255 };
				case element::text_field_selected: return { 60, 190, 175, 150 };
				case element::text_field_caret: return { 60, 190, 175, 255 };
				case element::text_field_input_error: return { 255, 0, 0, 220 };

				case element::table_view: return { 16, 29, 36, 255 };
				case element::table_view_border: return { 150, 150, 150, 255 };
				case element::table_view_alternate: return { 60, 190, 175, 15 };
				case element::table_view_row_hover: return { 40, 160, 200, 100 };
				case element::table_view_row_selected: return { 40, 160, 200, 255 };
				case element::table_view_text_header: return { 155, 165, 180, 255 };
				case element::table_view_text_header_hot: return defaults::color(theme, element::form);
				case element::table_view_text_selected: return defaults::color(theme, element::form);
				case element::table_view_header: return { 16, 29, 36, 255 };
				case element::table_view_grid: return { 55, 60, 65, 255 };

				case element::toggle: return { 255, 255, 255, 200 };
				case element::toggle_on: return { 0, 120, 170, 255 };
				case element::toggle_off: return { 150, 150, 150, 255 };
				case element::toggle_selected: return { 60, 190, 175, 60 };

				case element::tree_view: return { 16, 29, 36, 255 };
				case element::tree_view_border: return { 150, 150, 150, 255 };
				case element::tree_view_hover: return { 60, 65, 70, 255 };
				case element::tree_view_selected: return { 0, 120, 170, 100 };

				case element::tab_pane: return { 100, 100, 100, 15 };
				case element::tab_pane_border: return { 0, 120, 170, 50 };
				case element::tab_pane_tabs: return { 0, 120, 170, 15 };
				case element::tab_pane_selected_tab: return { 60, 190, 175, 255 };

				case element::pane: return { 100, 100, 100, 15 };
				case element::pane_border: return { 0, 120, 170, 50 };

				case element::image_view: return { 222, 222, 222, 0 };
				case element::image_view_border: return { 150, 150, 150, 0 };
				case element::image_view_hover: return { 60, 190, 175, 100 };
				case element::image_view_selected: return { 60, 190, 175, 150 };

				case element::rectangle: return { 222, 222, 222, 255 };
				case element::rectangle_border: return { 150, 150, 150, 255 };
				case element::rectangle_border_hot: return { 60, 190, 175, 50 };
				case element::rectangle_hover: return { 60, 190, 175, 100 };
				case element::rectangle_selected: return { 60, 190, 175, 100 };

				case element::group: return { 60, 190, 175, 25 };
				case element::group_border: return { 60, 190, 175, 100 };

				case element::slider: return { 26, 39, 46, 255 };
				case element::slider_border: return { 70, 70, 70, 255 };
				case element::slider_knob: return { 60, 65, 75, 255 };
				case element::slider_knob_hot: return { 80, 85, 95, 255 };
				case element::slider_knob_border: return { 90, 90, 90, 255 };
				case element::slider_tick: return { 85, 95, 100, 255 };
				case element::slider_selected: return { 60, 190, 175, 150 };

				case element::html_editor: return { 14, 26, 32, 255 };
				case element::html_editor_border: return { 150, 150, 150, 255 };
				case element::html_editor_disabled: return { 30, 35, 40, 255 };
				case element::html_editor_selected: return { 60, 190, 175, 150 };
				case element::html_editor_caret: return { 60, 190, 175, 255 };
				case element::html_control: return { 100, 100, 100, 7 };
				case element::html_control_border: return { 0, 120, 170, 50 };

				case element::combobox: return { 60, 65, 75, 255 };
				case element::combobox_editable: return { 14, 26, 32, 255 };
				case element::combobox_caret: return { 60, 190, 175, 255 };
				case element::combobox_border: return { 70, 70, 70, 255 };
				case element::combobox_hover: return { 80, 85, 95, 255 };
				case element::combobox_selected: return { 60, 190, 175, 150 };
				case element::combobox_dropdown: return { 60, 65, 75, 255 };
				case element::combobox_dropdown_hot: return { 60, 190, 175, 255 };
				case element::combobox_dropdown_arrow: return { 150, 150, 150, 255 };
				case element::combobox_dropdown_arrow_hot: return { 16, 29, 36, 255 };

				case element::line: return { 150, 150, 150, 255 };
				case element::line_hover: return { 60, 190, 175, 50 };
				case element::line_selected: return { 60, 190, 175, 100 };

				case element::icon: return { 100, 100, 100, 0 };
				case element::icon_border: return { 0, 120, 170, 0 };
				case element::icon_hot: return { 60, 190, 175, 60 };
				case element::icon_text: return defaults::color(theme, element::label);
				case element::icon_description_text: return { 125, 125, 125, 255 };

				case element::strength_bar: return { 26, 39, 46, 255 };
				case element::strength_bar_border: return { 85, 95, 100, 255 };

				case element::badge: return { 255, 0, 0, 180 };
				case element::badge_border: return { 255, 255, 255, 180 };
				case element::badge_text: return { 255, 255, 255, 255 };
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

		bool& appearance::shadow() { return _d._fm._d._borderless_shadow; }
		bool appearance::get_shadow() const { return _d._fm._d._borderless_shadow; }

		appearance& appearance::shadow(const bool& enable) {
			_d._fm._d._borderless_shadow = enable;
			return *this;
		}

		appearance& appearance::theme(themes theme) {
			_d._fm._d._theme = theme;

			// change the background color of the form and title bar
			(*this)
				.background(defaults::color(theme, element::form))
				.title_bar(defaults::color(theme, element::titlebar));

			// change the theme colors
			appearance::theme(defaults::color(theme, element::accent),
				defaults::color(theme, element::accent_hover),
				defaults::color(theme, element::disabled),
				defaults::color(theme, element::label));

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

		lecui::color& appearance::background() { return _d._fm._d._clr_background; }
		lecui::color appearance::get_background() const { return _d._fm._d._clr_background; }

		appearance& appearance::background(const lecui::color& color) {
			_d._fm._d._clr_background = color;
			return *this;
		}

		lecui::color& appearance::title_bar() { return _d._fm._d._clr_titlebar_background; }
		lecui::color appearance::title_bar() const { return _d._fm._d._clr_titlebar_background; }

		appearance& appearance::title_bar(const lecui::color& color) {
			_d._fm._d._clr_titlebar_background = color;
			return *this;
		}

		int& appearance::main_icon() { return _d._fm._d._idi_icon; }
		int appearance::get_main_icon() const { return _d._fm._d._idi_icon; }

		appearance& appearance::main_icon(const int& main_icon) {
			_d._fm._d._idi_icon = main_icon;
			return *this;
		}

		int& appearance::mini_icon() { return _d._fm._d._idi_icon_small; }
		int appearance::get_mini_icon() const { return _d._fm._d._idi_icon_small; }

		appearance& appearance::mini_icon(const int& mini_icon) {
			_d._fm._d._idi_icon_small = mini_icon;
			return *this;
		}

		int& appearance::caption_icon() { return _d._fm._d._png_caption_icon; }
		int appearance::get_caption_icon() const { return _d._fm._d._png_caption_icon; }

		appearance& appearance::caption_icon(const int& caption_icon) {
			_d._fm._d._png_caption_icon = caption_icon;
			return *this;
		}
	}
}
