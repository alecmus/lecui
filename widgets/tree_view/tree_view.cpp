//
// tree_view.cpp - tree widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../tree_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::tree_view::operator==(const tree_view& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border);
		}

		bool widgets::tree_view::operator!=(const tree_view& param) {
			return !operator==(param);
		}

		widgets::tree_view& widgets::tree_view::operator=(const tree_view& right) {
			// generic widget
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

			// widget specific widget
			_border = right._border;
			_color_border = right._color_border;
			_corner_radius_x = right._corner_radius_x;
			_corner_radius_y = right._corner_radius_y;
			_root = right._root;

			// NOT copied (_alias, _p_special_pane_specs)

			return *this;
		}

		widgets::tree_view::tree_view(const tree_view& right) {
			*this = right;
		}

		std::string& widgets::tree_view::text() { return _text; }

		widgets::tree_view& widgets::tree_view::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& widgets::tree_view::tooltip() { return _tooltip; }

		widgets::tree_view& widgets::tree_view::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& widgets::tree_view::rect() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect();
			else
				return _rect;
		}

		widgets::tree_view& widgets::tree_view::rect(const lecui::rect& rect) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->rect(rect);
			else
				_rect = rect;

			return *this;
		}

		resize_params& widgets::tree_view::on_resize() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize();
			else
				return _on_resize;
		}

		widgets::tree_view& widgets::tree_view::on_resize(const resize_params& on_resize) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->on_resize(on_resize);
			else
				_on_resize = on_resize;

			return *this;
		}

		widgets::widget::cursor_type& widgets::tree_view::cursor() { return _cursor; }

		widgets::tree_view& widgets::tree_view::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& widgets::tree_view::font() { return _font; }

		widgets::tree_view& widgets::tree_view::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& widgets::tree_view::font_size() { return _font_size; }

		widgets::tree_view& widgets::tree_view::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& widgets::tree_view::color_text() { return _color_text; }

		widgets::tree_view& widgets::tree_view::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& widgets::tree_view::color_fill() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill();
			else
				return _color_fill;
		}

		widgets::tree_view& widgets::tree_view::color_fill(const color& color_fill) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_fill(color_fill);
			else
				_color_fill = color_fill;

			return *this;
		}

		color& widgets::tree_view::color_hot() { return _color_hot; }

		widgets::tree_view& widgets::tree_view::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& widgets::tree_view::color_selected() { return _color_selected; }

		widgets::tree_view& widgets::tree_view::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& widgets::tree_view::color_disabled() { return _color_disabled; }

		widgets::tree_view& widgets::tree_view::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& widgets::tree_view::border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border();
			else
				return _border;
		}

		widgets::tree_view& widgets::tree_view::border(const float& border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->border(border);
			else
				_border = border;

			return *this;
		}

		lecui::color& widgets::tree_view::color_border() {
			if (_p_special_pane_specs)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border();
			else
				return _color_border;
		}

		widgets::tree_view& widgets::tree_view::color_border(const color& color_border) {
			if (_p_special_pane_specs)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(_p_special_pane_specs)->color_border(color_border);
			else
				_color_border = color_border;

			return *this;
		}

		float& widgets::tree_view::corner_radius_x() { return _corner_radius_x; }

		widgets::tree_view& widgets::tree_view::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& widgets::tree_view::corner_radius_y() { return _corner_radius_y; }

		widgets::tree_view& widgets::tree_view::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		std::map<std::string, widgets::tree_view::node>& widgets::tree_view::root() { return _root; }

		widgets::tree_view& widgets::tree_view::root(const std::map<std::string, widgets::tree_view::node>& root) {
			_root = root;
			return *this;
		}

		namespace widgets {
			/// <summary>Tree view widget builder.</summary>
			class tree_view_builder {
			public:
				/// <summary>Tree view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <remarks>This constructs the widget with an internally generated random
				/// alias.</remarks>
				tree_view_builder(containers::page& page);

				/// <summary>Tree view builder constructor.</summary>
				/// <param name="page">The container to place the widget in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "database_browser".</param>
				tree_view_builder(containers::page& page, const std::string& alias);
				~tree_view_builder();

				/// <summary>Get the tree view specifications.</summary>
				/// <returns>A reference to the tree view specifications.</returns>
				[[nodiscard]]
				tree_view& specs();

				/// <summary>Get the tree view specifications.</summary>
				/// <returns>A reference to the tree view specifications.</returns>
				/// <remarks>Alternative to widget() for more terse code.</remarks>
				[[nodiscard]]
				tree_view& operator()();

				/// <summary>Get the specifications of a tree view.</summary>
				/// <param name="fm">The form containing the tree view.</param>
				/// <param name="path">The full path to the widget, e.g.
				/// "admin_page/system_pane/database_browser".</param>
				/// <returns>A reference to the tree specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided.</remarks>
				[[nodiscard]]
				static tree_view& specs(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				tree_view_builder() = delete;
				tree_view_builder(const tree_view_builder&) = delete;
				tree_view_builder& operator=(const tree_view_builder&) = delete;
			};
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
					.color_text(defaults::color(_page._d_page._fm._d._theme, element::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, element::tree_view))
					.color_border(defaults::color(_page._d_page._fm._d._theme, element::tree_view_border))
					.color_hot(defaults::color(_page._d_page._fm._d._theme, element::tree_view_hover))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, element::tree_view_selected));
			}
			containers::page& _page;
			tree_view& _specs;
		};

		widgets::tree_view_builder::tree_view_builder(containers::page& page) :
			tree_view_builder(page, "") {}

		widgets::tree_view_builder::tree_view_builder(containers::page& page, const std::string& alias) :
			_d(*(new impl(page, alias))) {}

		widgets::tree_view_builder::~tree_view_builder() { delete& _d; }

		widgets::tree_view&
			widgets::tree_view_builder::specs() {
			return _d._specs;
		}

		widgets::tree_view&
			widgets::tree_view_builder::operator()() {
			return specs();
		}

		widgets::tree_view&
			widgets::tree_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tree_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tree_view_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		widgets::tree_view& liblec::lecui::widgets::tree_view::add(containers::page& page, const std::string& alias) {
			return tree_view_builder(page, alias).specs();
		}

		widgets::tree_view& liblec::lecui::widgets::tree_view::get(form& fm, const std::string& path) {
			return tree_view_builder::specs(fm, path);
		}
	}
}
