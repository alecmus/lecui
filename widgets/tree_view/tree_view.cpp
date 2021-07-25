//
// tree_view.cpp - tree widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../tree_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::tree_view_specs::operator==(const tree_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(_color_border == param._color_border);
		}

		bool widgets::tree_view_specs::operator!=(const tree_view_specs& param) {
			return !operator==(param);
		}

		widgets::tree_view_specs& widgets::tree_view_specs::operator=(const tree_view_specs& right) {
			// generic specs
			_text = right._text;
			_tooltip = right._tooltip;
			_rect = right._rect;
			_on_resize = right._on_resize;
			_cursor = right._cursor;
			_font = right._font;
			_font_size = right._font_size;
			_color_text = right._color_text;
			_color_fill = right._color_fill;
			_color_hot = right._color_hot;
			_color_selected = right._color_selected;
			_color_disabled = right._color_disabled;

			// events
			_tree_events = right._tree_events;

			// widget specific specs
			_border = right._border;
			_color_border = right._color_border;
			_corner_radius_x = right._corner_radius_x;
			_corner_radius_y = right._corner_radius_y;
			_root = right._root;

			// NOT copied (_alias, _p_special_pane_specs)

			return *this;
		}

		widgets::tree_view_specs::tree_view_specs(const tree_view_specs& right) {
			*this = right;
		}

		std::string& widgets::tree_view_specs::text() { return _text; }

		widgets::tree_view_specs& widgets::tree_view_specs::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::tree_view_specs::tooltip() { return _tooltip; }

		widgets::tree_view_specs& widgets::tree_view_specs::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::tree_view_specs::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		widgets::resize_params& widgets::tree_view_specs::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::tree_view_specs::cursor() { return _cursor; }

		widgets::tree_view_specs& widgets::tree_view_specs::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::tree_view_specs::font() { return _font; }

		widgets::tree_view_specs& widgets::tree_view_specs::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::tree_view_specs::font_size() { return _font_size; }

		widgets::tree_view_specs& widgets::tree_view_specs::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::tree_view_specs::color_text() { return _color_text; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::tree_view_specs::color_fill() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill();
			else
				return _color_fill;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::color_fill(const color& color_fill) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill(color_fill);
			else
				_color_fill = color_fill;

			return *this;
		}

		color& widgets::tree_view_specs::color_hot() { return _color_hot; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::tree_view_specs::color_selected() { return _color_selected; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::tree_view_specs::color_disabled() { return _color_disabled; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::tree_view_specs::border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border();
			else
				return _border;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::border(const float& border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border(border);
			else
				_border = border;

			return *this;
		}

		lecui::color& widgets::tree_view_specs::color_border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border();
			else
				return _color_border;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::color_border(const color& color_border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border(color_border);
			else
				_color_border = color_border;

			return *this;
		}

		float& widgets::tree_view_specs::corner_radius_x() { return _corner_radius_x; }

		widgets::tree_view_specs& widgets::tree_view_specs::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::tree_view_specs::corner_radius_y() { return _corner_radius_y; }

		widgets::tree_view_specs& widgets::tree_view_specs::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		std::map<std::string, widgets::tree_view_specs::node>& widgets::tree_view_specs::root() { return _root; }

		widgets::tree_view_specs& widgets::tree_view_specs::root(const std::map<std::string, widgets::tree_view_specs::node>& root) {
			_root = root;
			return *this;
		}

		class widgets::tree_view_builder::impl {
		public:
			// note: this placement is only temporary. the next time WM_PAINT is called
			// this tree widget will be moved into a special pane
			// this is important so we don't have to manually handle scroll bar issues in tree_view_impl.
			// all we need to do for scroll bars is dynamically change _specs.rect as the tree changes
			// and the pane will do all the scrolling for us
			impl(containers::page& page, const std::string& alias) :
				_page(page),
				_specs(_page._d_page.add_tree(alias)) {
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, item::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, item::tree_view))
					.color_border(defaults::color(_page._d_page._fm._d._theme, item::tree_view_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, item::tree_view_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, item::tree_view_selected));
			}
			containers::page& _page;
			tree_view_specs& _specs;
		};

		widgets::tree_view_builder::tree_view_builder(containers::page& page) :
			tree_view_builder(page, "") {}

		widgets::tree_view_builder::tree_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::tree_view_builder::~tree_view_builder() { delete& _d; }

		widgets::tree_view_specs&
			widgets::tree_view_builder::specs() {
			return _d._specs;
		}

		widgets::tree_view_specs&
			widgets::tree_view_builder::operator()() {
			return specs();
		}

		widgets::tree_view_specs&
			widgets::tree_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tree(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tree(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
