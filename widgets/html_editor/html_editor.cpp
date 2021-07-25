//
// html_editor.cpp - html editor widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../html_editor.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::html_editor_specs::operator==(const html_editor_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border) &&
				(_color_caret == param._color_caret) &&
				(_color_control_border == param._color_control_border) &&
				(_color_control_fill == param._color_control_fill);
		}

		bool widgets::html_editor_specs::operator!=(const html_editor_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::html_editor_specs::text() { return _text; }

		widgets::html_editor_specs& widgets::html_editor_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::html_editor_specs::tooltip() { return _tooltip; }

		widgets::html_editor_specs& widgets::html_editor_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::html_editor_specs::rect() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special rectangle
				return _rect_widget_overall;	// to-do: how to transfer changes to special panes
			else
				return _rect;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::rect(const lecui::rect& rect) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				_rect_widget_overall = rect;	// to-do: how to transfer changes to special panes
			else
				_rect = rect;

			return *this;
		}

		widgets::resize_params& widgets::html_editor_specs::on_resize() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return _on_resize_overall;	// to-do: how to transfer changes to special panes
			else
				return _on_resize;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::on_resize(const resize_params& on_resize) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				_on_resize_overall = on_resize;	// to-do: how to transfer changes to special panes
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::html_editor_specs::cursor() { return _cursor; }

		widgets::html_editor_specs& widgets::html_editor_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::html_editor_specs::font() { return _font; }

		widgets::html_editor_specs& widgets::html_editor_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::html_editor_specs::font_size() { return _font_size; }

		widgets::html_editor_specs& widgets::html_editor_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::html_editor_specs::color_text() { return _color_text; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::html_editor_specs::color_fill() { return _color_fill; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& widgets::html_editor_specs::color_hot() { return _color_hot; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::html_editor_specs::color_selected() { return _color_selected; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::html_editor_specs::color_disabled() { return _color_disabled; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::html_editor_specs::border() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_html_pane_specs)->border();
			else
				return _border;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::border(const float& border) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_html_pane_specs)->border(border);
			else
				_border = border;

			return *this;
		}

		float& widgets::html_editor_specs::control_border() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->border();
			else
				return _control_border;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::control_border(const float& control_border) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->border(control_border);
			else
				_control_border = control_border;

			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_border() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_html_pane_specs)->color_border();
			else
				return _color_border;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_border(const color& color_border) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_html_pane_specs)->color_border(color_border);
			else
				_color_border = color_border;

			return *this;
		}

		float& widgets::html_editor_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::html_editor_specs& widgets::html_editor_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::html_editor_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::html_editor_specs& widgets::html_editor_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_caret() { return _color_caret; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_caret(const color& color_caret) {
			_color_caret = color_caret;
			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_control_border() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->color_border();
			else
				return _color_control_border;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_control_border(const color& color_control_border) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->color_border(color_control_border);
			else
				_color_control_border = color_control_border;

			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_control_fill() {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->color_fill();
			else
				return _color_control_fill;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_control_fill(const color& color_control_fill) {
			if (_p_controls_pane_specs && _p_html_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_controls_pane_specs)->color_fill(color_control_fill);
			else
				_color_control_fill = color_control_fill;

			return *this;
		}

		class widgets::html_editor_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_html_editor(alias)) {
				_specs
					.font("Calibri")
					.font_size(11.f)
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::html_editor))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::html_editor_border))
					.color_disabled(defaults::color(_page._d_page._fm._d._theme, item::html_editor_disabled))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::html_editor_selected))
					.color_caret(defaults::color(_page._d_page._fm._d._theme, item::html_editor_caret))
					.color_control_fill(defaults::color(_page._d_page._fm._d._theme, item::html_control))
					.color_control_border(defaults::color(_page._d_page._fm._d._theme, item::html_control_border));
			}
			containers::page& _page;
			html_editor_specs& _specs;
		};

		widgets::html_editor_builder::html_editor_builder(containers::page& page) :
			html_editor_builder(page, "") {}

		widgets::html_editor_builder::html_editor_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::html_editor_builder::~html_editor_builder() { delete& _d; }

		widgets::html_editor_specs&
			widgets::html_editor_builder::specs() {
			return _d._specs;
		}

		widgets::html_editor_specs&
			widgets::html_editor_builder::operator()() {
			return specs();
		}

		widgets::html_editor_specs&
			widgets::html_editor_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
