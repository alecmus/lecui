//
// tab_pane.cpp - tab pane implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../tab_pane.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		namespace containers {
			/// <summary>Tab pane container builder.</summary>
			/// <remarks>Only tab containers can be added to the tab pane container.</remarks>
			class tab_pane_builder {
			public:
				/// <summary>Tab pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <param name="content_margin">The margin to use inside the tabs.</param>
				/// <remarks>This constructs the container with an internally generated random
				/// alias.</remarks>
				tab_pane_builder(containers::page& page, const float& content_margin = 10.f);

				/// <summary>Tab pane builder constructor.</summary>
				/// <param name="page">A reference to the container to place the control in.</param>
				/// <param name="alias">The in-page unique alias, e.g. "settings_tab_pane".</param>
				/// <param name="content_margin">The margin to use inside the tabs.</param>
				/// <remarks>Ensure that the alias is unique within the page. Reusing an alias
				/// in a tab pane leads to undefined behavior.</remarks>
				tab_pane_builder(containers::page& page, const std::string& alias, const float& content_margin = 10.f);
				~tab_pane_builder();

				/// <summary>Get the tab pane specifications.</summary>
				/// <returns>A reference to the control specifications.</returns>
				[[nodiscard]]
				tab_pane& get();

				/// <summary>Get the specifications of a tab pane.</summary>
				/// <param name="fm">The form the control is in.</param>
				/// <param name="path">The full path to the control, e.g.
				/// "sample_page/settings_tab_pane".</param>
				/// <returns>A reference to the control specifications.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_tab_pane_specs).</remarks>
				[[nodiscard]]
				static tab_pane& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				tab_pane_builder() = delete;
				tab_pane_builder(const tab_pane_builder&) = delete;
				tab_pane_builder& operator=(const tab_pane_builder&) = delete;

				friend class tab_builder;
			};
		}

		class containers::tab_pane_builder::impl {
		public:
			impl(containers::page& page,
				containers::tab_pane& specs,
				const std::string& alias) :
				_page(page), _specs(specs) {
				specs._p_page = &page;
				_specs
					.color_text(defaults::color(_page._d_page._fm._d._theme, element::label))
					.color_fill(defaults::color(_page._d_page._fm._d._theme, element::tab_pane))
					.color_border(defaults::color(_page._d_page._fm._d._theme, element::tab_pane_border))
					.color_tabs(defaults::color(_page._d_page._fm._d._theme, element::tab_pane_tabs))
					.color_tabs_border(defaults::color(_page._d_page._fm._d._theme, element::tab_pane_border))
					.color_selected(defaults::color(_page._d_page._fm._d._theme, element::tab_pane_selected_tab));
			}
			containers::page& _page;
			containers::tab_pane& _specs;
		};

		containers::tab_pane_builder::tab_pane_builder(containers::page& page, const float& content_margin) :
			tab_pane_builder(page, "", content_margin) {}

		containers::tab_pane_builder::tab_pane_builder(containers::page& page,
			const std::string& alias, const float& content_margin) :
			_d(*(new impl(page, page._d_page.add_tab_pane(alias, content_margin), alias))) {}

		containers::tab_pane_builder::~tab_pane_builder() { delete& _d; }

		containers::tab_pane& containers::tab_pane_builder::get() {
			return _d._specs;
		}

		containers::tab_pane&
			containers::tab_pane_builder::get(form& fm,
				const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tab_pane_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, path_remaining);
					return results.page._d_page.get_tab_pane_impl(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		namespace containers {
			/// <summary>Tab container builder.</summary>
			class tab_builder {
			public:
				/// <summary>Tab builder constructor.</summary>
				/// <param name="tp">The tab pane to place it in.</param>
				/// <param name="tab_name">The in-tab_pane unique name of the tab,
				/// e.g. "Options".</param>
				tab_builder(tab_pane& tp, const std::string& tab_name);
				~tab_builder();

				/// <summary>Get the tab container page.</summary>
				/// <returns>A reference to the tab container page.</returns>
				/// <remarks>Note that this is a container of type 'page', hence anything that can
				/// be added to a regular page can be added here as well. The page comes fully
				/// featured with scroll bars when widgets exceed the dimensions of the tab, just
				/// like a regular page.</remarks>
				[[nodiscard]]
				containers::tab& get();

				/// <summary>Get the tab container page of an existing tab.</summary>
				/// <param name="fm">The form the container is in.</param>
				/// <param name="path">The full path to the tab, e.g.
				/// "sample_page/settings_tab_pane/Options".
				/// </param>
				/// <returns>A reference to the tab container page.</returns>
				/// <remarks>Throws on failure. For faster coding and more readable code consider
				/// calling this static method through the helper macro provided (get_tab_page).</remarks>
				[[nodiscard]]
				static containers::tab& get(form& fm, const std::string& path);

			private:
				class impl;
				impl& _d;

				// Default constructor and copying an object of this class are not allowed
				tab_builder() = delete;
				tab_builder(const tab_builder&) = delete;
				tab_builder& operator=(const tab_builder&) = delete;
			};
		}

		class containers::tab_builder::impl {
		public:
			impl(containers::tab_pane& tp, const std::string& tab_name) :
				_tp(tp),
				_page(add(tp, tab_name)) {}
			containers::tab_pane& _tp;
			containers::tab& _page;

			static containers::tab& add(tab_pane& tp, const std::string& tab_name) {
				containers::page& _page = *tp._p_page;
				auto& _tab_pane = _page._d_page.get_tab_pane_impl(tp.alias());

				if (_tab_pane._p_tabs.count(tab_name)) {
					log("library usage error containers::tab::impl::add");

					/// to-do: find a mechanism that makes sense ...
					return _tab_pane._p_tabs.at(tab_name);
				}

				// set the tabs change flag
				_tab_pane._tabs_changed = true;

				_tab_pane._p_tabs.try_emplace(tab_name, _page._d_page._fm, tab_name);
				_tab_pane._tab_badge_resources.try_emplace(tab_name, widgets::badge_resources());
				_tab_pane._tab_order.push_back(tab_name);

				// set the badge defaults
				_tab_pane._p_tabs.at(tab_name).badge()
					.color(defaults::color(_page._d_page._fm._d._theme, element::badge))
					.color_border(defaults::color(_page._d_page._fm._d._theme, element::badge_border))
					.color_text(defaults::color(_page._d_page._fm._d._theme, element::badge_text));

				auto& page_impl = _tab_pane._p_tabs.at(tab_name)._d_page;

				// specify direct2d factory (used internally for geometries and stuff)
				page_impl.direct2d_factory(_page._d_page.direct2d_factory());

				// specify directwrite factory (used internally for text rendering)
				page_impl.directwrite_factory(_page._d_page.directwrite_factory());

				// specify iwic imaging factory (used internally for image rendering)
				page_impl.iwic_factory(_page._d_page.iwic_factory());

				// specify parent
				page_impl.parent(_page);

				const float thickness = 10.f;	// thickness of scroll bars
				const float _content_margin = _tab_pane.content_margin();
				float _tab_height = _tab_pane.tab_height();
				rect rect_client_area = _tab_pane().rect();

				bool tabs_perpendicular = false;

				switch (tp.tab_side()) {
				case tab_pane::side::left:
				case tab_pane::side::right:
					if (tp.caption_orientation() == tab_pane::orientation::horizontal)
						tabs_perpendicular = true;
					break;

				case tab_pane::side::top:
				case tab_pane::side::bottom:
				default: 
					if (tp.caption_orientation() == tab_pane::orientation::vertical)
						tabs_perpendicular = true;
					break;
				}

				if (tabs_perpendicular && !_tab_pane.tab_height_set()) {
					if (!_tab_pane.specs().caption_reserve().empty()) {
						log(tab_name + ": using caption reserve to compute dimensions");
						// compute the longest tab caption in the reserve
						for (const auto& alias : _tab_pane.specs().caption_reserve()) {
							D2D1_RECT_F max_rect = { 0.f, 0.f, _tab_pane.specs().rect().height(), _tab_pane.caption_bar_height() };
							auto caption_rect = widgets::measure_label(_page._d_page._p_directwrite_factory, alias, _tab_pane.specs().font(),
								_tab_pane.specs().font_size(), text_alignment::left, paragraph_alignment::middle, max_rect);

							_tab_height = largest(_tab_height, caption_rect.right - caption_rect.left + 3.f * _tab_pane.padding());
						}
					}
					else {
						log(tab_name + ": WARNING - no caption reserve for perpendicular tab captions!");
						// use current caption to set tab height
						D2D1_RECT_F max_rect = { 0.f, 0.f, _tab_pane.specs().rect().height(), _tab_pane.caption_bar_height() };
						auto caption_rect = widgets::measure_label(_page._d_page._p_directwrite_factory, tab_name, _tab_pane.specs().font(),
							_tab_pane.specs().font_size(), text_alignment::left, paragraph_alignment::middle, max_rect);

						_tab_height = largest(_tab_height, caption_rect.right - caption_rect.left + 3.f * _tab_pane.padding());
					}

					_tab_pane.set_tab_height(_tab_height);
				}

				const auto _caption_bar_height = _tab_pane.caption_bar_height();

				// set page size
				page_impl.size({ rect_client_area.width(), rect_client_area.height() });

				switch (tp.tab_side()) {
				case tab_pane::side::left:
				case tab_pane::side::right:
					page_impl.width(page_impl.width() - (2.f * _content_margin + _caption_bar_height));
					page_impl.height(page_impl.height() - (2.f * _content_margin));
					break;

				case tab_pane::side::top:
				case tab_pane::side::bottom:
				default:
					page_impl.width(page_impl.width() - (2.f * _content_margin));
					page_impl.height(page_impl.height() - (2.f * _content_margin + _caption_bar_height));
					break;
				}

				// add an invisible rect to bound the page. This is essential for scroll bars
				// to work appropriately when contents don't reach the page borders
				auto& rectangle = page_impl.add_rectangle(widgets::rectangle_impl::page_rect_alias());
				rectangle.color_fill().alpha(0);

				// make it transparent
				rectangle
					.color_border({ 255, 0, 0, 0 })
					.color_hot({ 255, 0, 0, 0 })
					.corner_radius_x(15.f)
					.corner_radius_y(15.f)

					// set its dimensions to exactly match the page
					.rect().size(page_impl.size());

				rectangle.on_resize()
					.width_rate(100.f)
					.height_rate(100.f);

				// capture pointer to pane rect and page
				_tab_pane._p_page_impl = &page_impl;
				_tab_pane._p_tab_pane_rect = &rectangle.rect();

				// define reference rect for scroll bars
				lecui::rect ref_rect = rectangle.rect();
				ref_rect.left() -= _content_margin;
				ref_rect.right() += _content_margin;
				ref_rect.top() -= _content_margin;
				ref_rect.bottom() += _content_margin;

				// initialize the page's horizontal scroll bar
				{
					auto& _specs = page_impl.h_scrollbar().specs();
					_specs.on_resize()
						.width_rate(100.f)
						.y_rate(100.f);

					auto width = rect_client_area.right() - rect_client_area.left() - 2.f * _content_margin;

					switch (tp.tab_side()) {
					case tab_pane::side::left:
					case tab_pane::side::right:
						width -= _caption_bar_height;
						break;

					case tab_pane::side::top:
					case tab_pane::side::bottom:
					default:
						break;
					}

					if (thickness > _content_margin)
						width -= (thickness - _content_margin);

					_specs.rect()
						.width(width)
						.height(thickness)
						.place(ref_rect, 50.f, 100.f);

					_specs
						.color_fill(defaults::color(_page._d_page._fm._d._theme, element::scrollbar))
						.color_scrollbar_border(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_border))
						.color_hot(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_hover))
						.color_hot_pressed(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_pressed));
				}

				// initialize the page's vertical scroll bar
				{
					auto& _specs = page_impl.v_scrollbar().specs();
					_specs.on_resize()
						.height_rate(100.f)
						.x_rate(100.f);

					auto height = rect_client_area.bottom() - rect_client_area.top() - 2.f * _content_margin;

					switch (tp.tab_side()) {
					case tab_pane::side::left:
					case tab_pane::side::right:
						break;

					case tab_pane::side::top:
					case tab_pane::side::bottom:
					default:
						height -= _caption_bar_height;
						break;
					}

					if (thickness > _content_margin)
						height -= (thickness - _content_margin);

					_specs.rect()
						.width(thickness)
						.height(height)
						.place(ref_rect, 100.f, 50.f);

					_specs
						.color_fill(defaults::color(_page._d_page._fm._d._theme, element::scrollbar))
						.color_scrollbar_border(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_border))
						.color_hot(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_hover))
						.color_hot_pressed(defaults::color(_page._d_page._fm._d._theme, element::scrollbar_pressed));
				}

				// return reference to page so caller can add widgets to it
				return _tab_pane._p_tabs.at(tab_name);
			}
		};

		containers::tab_builder::tab_builder(containers::tab_pane& tp, const std::string& tab_name) :
			_d(*(new impl(tp, tab_name))) {}

		containers::tab_builder::~tab_builder() { delete& _d; }

		containers::tab& containers::tab_builder::get() {
			return _d._page;
		}

		containers::tab&
			containers::tab_builder::get(form& fm, const std::string& path) {
			auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);

				std::string tab_name, tab_pane_path;
				idx = path_remaining.find_last_of("/");
				if (idx != std::string::npos) {
					tab_name = path_remaining.substr(idx + 1, path_remaining.length() - idx);
					tab_pane_path = path_remaining.substr(0, idx);
				}

				try {
					// check form pages
					auto& page = fm._d._p_pages.at(page_alias);
					auto results = fm._d.find_widget(page, tab_pane_path);

					auto& impl = results.page._d_page.get_tab_pane_impl(results.widget.alias());
					return impl._p_tabs.at(tab_name);
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm._d._p_status_panes.at(page_alias);
					auto results = fm._d.find_widget(page, tab_pane_path);

					auto& impl = results.page._d_page.get_tab_pane_impl(results.widget.alias());
					return impl._p_tabs.at(tab_name);
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}

		bool containers::tab_pane::operator==(const tab_pane& param) {
			return
				// generic widget
				widget::operator==(param) &&

				// widget specific widget
				(_color_border == param._color_border) &&
				(_color_tabs == param._color_tabs) &&
				(_color_tabs_border == param._color_tabs_border);
		}

		bool containers::tab_pane::operator!=(const tab_pane& param) {
			return !operator==(param);
		}

		std::string& containers::tab_pane::text() { return _text; }

		containers::tab_pane& containers::tab_pane::text(const std::string& text) {
			_text = text;
			return *this;
		}

		std::string& containers::tab_pane::tooltip() { return _tooltip; }

		containers::tab_pane& containers::tab_pane::tooltip(const std::string& tooltip) {
			_tooltip = tooltip;
			return *this;
		}

		lecui::rect& containers::tab_pane::rect() { return _rect; }

		containers::tab_pane& containers::tab_pane::rect(const lecui::rect& rect) {
			_rect = rect;
			return *this;
		}

		resize_params& containers::tab_pane::on_resize() { return _on_resize; }

		containers::tab_pane& containers::tab_pane::on_resize(const resize_params& on_resize) {
			_on_resize = on_resize;
			return *this;
		}

		widgets::widget::cursor_type& containers::tab_pane::cursor() { return _cursor; }

		containers::tab_pane& containers::tab_pane::cursor(const cursor_type cursor) {
			_cursor = cursor;
			return *this;
		}

		std::string& containers::tab_pane::font() { return _font; }

		containers::tab_pane& containers::tab_pane::font(const std::string& font) {
			_font = font;
			return *this;
		}

		float& containers::tab_pane::font_size() { return _font_size; }

		containers::tab_pane& containers::tab_pane::font_size(const float& font_size) {
			_font_size = font_size;
			return *this;
		}

		color& containers::tab_pane::color_text() { return _color_text; }

		containers::tab_pane& containers::tab_pane::color_text(const color& color_text) {
			_color_text = color_text;
			return *this;
		}

		color& containers::tab_pane::color_fill() { return _color_fill; }

		containers::tab_pane& containers::tab_pane::color_fill(const color& color_fill) {
			_color_fill = color_fill;
			return *this;
		}

		color& containers::tab_pane::color_hot() { return _color_hot; }

		containers::tab_pane& containers::tab_pane::color_hot(const color& color_hot) {
			_color_hot = color_hot;
			return *this;
		}

		color& containers::tab_pane::color_selected() { return _color_selected; }

		containers::tab_pane& containers::tab_pane::color_selected(const color& color_selected) {
			_color_selected = color_selected;
			return *this;
		}

		color& containers::tab_pane::color_disabled() { return _color_disabled; }

		containers::tab_pane& containers::tab_pane::color_disabled(const color& color_disabled) {
			_color_disabled = color_disabled;
			return *this;
		}

		float& containers::tab_pane::border() { return _border; }

		containers::tab_pane& containers::tab_pane::border(const float& border) {
			_border = border;
			return *this;
		}

		lecui::color& containers::tab_pane::color_border() { return _color_border; }

		containers::tab_pane& containers::tab_pane::color_border(const color& color_border) {
			_color_border = color_border;
			return *this;
		}

		float& containers::tab_pane::corner_radius_x() { return _corner_radius_x; }

		containers::tab_pane& containers::tab_pane::corner_radius_x(const float& corner_radius_x) {
			_corner_radius_x = corner_radius_x;
			return *this;
		}

		float& containers::tab_pane::corner_radius_y() { return _corner_radius_y; }

		containers::tab_pane& containers::tab_pane::corner_radius_y(const float& corner_radius_y) {
			_corner_radius_y = corner_radius_y;
			return *this;
		}

		containers::tab_pane::side& containers::tab_pane::tab_side() { return _tab_side; }

		containers::tab_pane& containers::tab_pane::tab_side(const side& tab_side) {
			_tab_side = tab_side;
			return *this;
		}

		std::vector<std::string>& containers::tab_pane::caption_reserve() { return _caption_reserve; }

		containers::tab_pane& containers::tab_pane::caption_reserve(const std::vector<std::string>& caption_reserve) {
			_caption_reserve = caption_reserve;
			return *this;
		}

		containers::tab_pane::orientation& containers::tab_pane::caption_orientation() { return _caption_orientation; }

		containers::tab_pane& containers::tab_pane::caption_orientation(const orientation& caption_orientation) {
			_caption_orientation = caption_orientation;
			return *this;
		}

		lecui::color& containers::tab_pane::color_tabs() { return _color_tabs; }

		containers::tab_pane& containers::tab_pane::color_tabs(const color& color_tabs) {
			_color_tabs = color_tabs;
			return *this;
		}

		lecui::color& containers::tab_pane::color_tabs_border() { return _color_tabs_border; }

		containers::tab_pane& containers::tab_pane::color_tabs_border(const color& color_tabs_border) {
			_color_tabs_border = color_tabs_border;
			return *this;
		}

		float& containers::tab_pane::tabs_border() { return _tabs_border; }

		containers::tab_pane& containers::tab_pane::tabs_border(const float& tabs_border) {
			_tabs_border = tabs_border;
			return *this;
		}

		std::string& containers::tab_pane::selected() { return _selected; }

		containers::tab_pane& containers::tab_pane::selected(const std::string& selected) {
			_selected = selected;
			return *this;
		}

		containers::tab_pane& containers::tab_pane::add(containers::page& page, const std::string& alias, const float& content_margin) {
			return containers::tab_pane_builder(page, alias, content_margin).get();
		}

		containers::tab_pane& containers::tab_pane::get(form& fm, const std::string& path) {
			return containers::tab_pane_builder::get(fm, path);
		}

		containers::tab::tab(form& fm, const std::string& tab_name) :
			page(fm, tab_name) {}

		bool containers::tab::operator==(const tab& param) {
			return
				_badge == param._badge;
		}

		bool containers::tab::operator!=(const tab& param) {
			return !operator==(param);
		}

		widgets::badge_specs& containers::tab::badge() { return _badge; }

		containers::tab& containers::tab::badge(const widgets::badge_specs& badge) {
			_badge = badge;
			return *this;
		}

		const lecui::size containers::tab::size() {
			return page::size();
		}

		containers::tab& containers::tab::add(containers::tab_pane& tp, const std::string& alias) {
			return containers::tab_builder(tp, alias).get();
		}

		containers::tab& containers::tab::get(form& fm, const std::string& path) {
			return containers::tab_builder::get(fm, path);
		}
	}
}
