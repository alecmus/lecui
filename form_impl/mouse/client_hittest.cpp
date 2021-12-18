//
// client_hittest.cpp - client hit test implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		void form::impl::client_hittest(const D2D1_POINT_2F& point) {
			for (auto& [key, child] : _m_children) {
				if (child && IsWindow(child->_d._hWnd) && child->_d._menu_form) {
					// ignore mouse movement on parent, there is a menu form open
					return;
				}
			}

			bool contains = false;
			bool change = false;
			HCURSOR h_cursor = nullptr;
			std::function<void()> tooltip_func = nullptr;

			class helper {
			public:
				static void hittest_hscrollbar(const std::string& page_alias,
					const std::string& current_page, containers::page& page,
					const D2D1_POINT_2F& point, const D2D1_POINT_2F& point_before,
					bool& contains, bool& change, bool& scroll_bar_hit) {
					// whether pressed or not
					if (page_alias == current_page &&
						page._d_page.h_scrollbar().visible() &&
						page._d_page.h_scrollbar().contains(point))
						scroll_bar_hit = true;

					// hit test horizontal scroll bar
					if (page._d_page.h_scrollbar().pressed()) {
						if (page_alias == current_page) {
							change = true;
							contains = page._d_page.h_scrollbar().contains(point);
							page._d_page.h_scrollbar().hit(contains);
						}

						// capture the displacement
						page._d_page.h_scrollbar()._x_displacement =
							page._d_page.h_scrollbar()._x_displacement_previous + (point.x - point_before.x);
					}

					if (!change) {
						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								for (auto& tab : tab_pane._p_tabs)
									hittest_hscrollbar(tab.first, tab_pane.specs().selected(), tab.second,
										point, point_before, contains, change, scroll_bar_hit);	// recursion
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(widget.first);

									for (auto& page : pane._p_panes)
										hittest_hscrollbar(page.first, pane._current_pane, page.second,
											point, point_before, contains, change, scroll_bar_hit);	// recursion
								}
						}
					}
				}

				static void hittest_vscrollbar(const std::string& page_alias,
					const std::string& current_page, containers::page& page,
					const D2D1_POINT_2F& point, const D2D1_POINT_2F& point_before,
					bool& contains, bool& change, bool& scroll_bar_hit) {
					// whether pressed or not
					if (page_alias == current_page &&
						page._d_page.v_scrollbar().visible() &&
						page._d_page.v_scrollbar().contains(point))
						scroll_bar_hit = true;

					// hit test vertical scroll bar
					if (page._d_page.v_scrollbar().pressed()) {
						if (page_alias == current_page) {
							change = true;
							contains = page._d_page.v_scrollbar().contains(point);
							page._d_page.v_scrollbar().hit(contains);
						}

						// capture the displacement
						page._d_page.v_scrollbar()._y_displacement =
							page._d_page.v_scrollbar()._y_displacement_previous + (point.y - point_before.y);
					}

					if (!change) {
						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								for (auto& tab : tab_pane._p_tabs)
									hittest_vscrollbar(tab.first, tab_pane.specs().selected(), tab.second,
										point, point_before, contains, change, scroll_bar_hit);	// recursion
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(widget.first);

									for (auto& page : pane._p_panes)
										hittest_vscrollbar(page.first, pane._current_pane, page.second,
											point, point_before, contains, change, scroll_bar_hit);	// recursion
								}
						}
					}
				}

				static void hittest_widgets(containers::page& page,
					const D2D1_POINT_2F& point, bool& contains, bool& change, bool lbutton_pressed, HCURSOR& h_cursor, const bool& scroll_bar_hit,
					std::function<void()>& tooltip_func) {
					bool in_page = page._d_page.contains(point);

					// hit test widgets
					for (auto& widget : page._d_page.widgets()) {
						bool is_scroll_bar = (widget.second.type() ==
							widgets::widget_type::h_scrollbar) ||
							(widget.second.type() ==
								widgets::widget_type::v_scrollbar);

						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						// failsafe: for good measure
						if (lbutton_pressed)
							widget.second.hide_tooltip();

						if (change)
							break;

						contains = (in_page || is_scroll_bar || lbutton_pressed) ? widget.second.contains(point) : false;

						if (!is_scroll_bar && scroll_bar_hit)
							contains = false;

						if (change = widget.second.hit(contains)) {
							if (widget.second.hit()) h_cursor = widget.second.cursor();

							if (contains && !lbutton_pressed)
								tooltip_func = [&]() { widget.second.show_tooltip(); };
							else
								widget.second.hide_tooltip();

							break;
						}

						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							if (page_iterator != tab_pane._p_tabs.end())
								helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor,
									scroll_bar_hit, tooltip_func);	// recursion
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(widget.first);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								if (page_iterator != pane._p_panes.end())
									helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor,
										scroll_bar_hit, tooltip_func);	// recursion
							}
					}
				}
			};

			// variable to ensure that once a scroll bar has been hit nothing else should
			bool scroll_bar_hit = false;

			if (!change) {
				for (auto& page : _p_pages)
					helper::hittest_hscrollbar(page.first, _current_page, page.second,
						point, _point_before, contains, change, scroll_bar_hit);
			}

			if (!change)
				for (auto& page : _p_pages)
					helper::hittest_vscrollbar(page.first, _current_page, page.second,
						point, _point_before, contains, change, scroll_bar_hit);

			if (!change) {
				for (auto& it : _p_status_panes)
					helper::hittest_widgets(it.second, point, contains, change, _lbutton_pressed, h_cursor,
						scroll_bar_hit, tooltip_func);
			}

			if (!change) {
				auto page_iterator = _p_pages.find(_current_page);

				if (page_iterator != _p_pages.end())
					helper::hittest_widgets(page_iterator->second, point, contains, change, _lbutton_pressed, h_cursor,
						scroll_bar_hit, tooltip_func);
			}

			if (!change) {
				// hit test form widgets
				for (auto& widget : _widgets) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					contains = widget.second.contains(point);
					if (change = widget.second.hit(contains)) {
						if (contains)
							tooltip_func = [&]() { widget.second.show_tooltip(); };
						else
							widget.second.hide_tooltip();

						break;
					}
				}
			}

			if (change) {
				_h_widget_cursor = h_cursor;
				update();

				if (tooltip_func)
					tooltip_func();
			}
		}
	}
}
