/*
** client_hittest.cpp - client hit test implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec T. Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		void form::impl::client_hittest(const D2D1_POINT_2F& point) {
			for (auto& [key, child] : m_children_) {
				if (child && IsWindow(child->d_.hWnd_) && child->d_.menu_form_) {
					// ignore mouse movement on parent, there is a menu form open
					return;
				}
			}

			bool contains = false;
			bool change = false;
			HCURSOR h_cursor = nullptr;

			class helper {
			public:
				static void hittest_hscrollbar(const std::string& page_alias,
					const std::string& current_page, containers::page& page,
					const D2D1_POINT_2F& point, const D2D1_POINT_2F& point_before,
					bool& contains, bool& change, bool& scroll_bar_hit) {
					// whether pressed or not
					if (page_alias == current_page &&
						page.d_page_.h_scrollbar().visible() &&
						page.d_page_.h_scrollbar().contains(point))
						scroll_bar_hit = true;

					// hit test horizontal scroll bar
					if (page.d_page_.h_scrollbar().pressed()) {
						if (page_alias == current_page) {
							change = true;
							contains = page.d_page_.h_scrollbar().contains(point);
							page.d_page_.h_scrollbar().hit(contains);
						}

						// capture the displacement
						page.d_page_.h_scrollbar().x_displacement_ =
							page.d_page_.h_scrollbar().x_displacement_previous_ + (point.x - point_before.x);
					}

					if (!change) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								for (auto& tab : tab_pane.p_tabs_)
									hittest_hscrollbar(tab.first, tab_pane.current_tab_, tab.second,
										point, point_before, contains, change, scroll_bar_hit);	// recursion
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									for (auto& page : pane.p_panes_)
										hittest_hscrollbar(page.first, pane.current_pane_, page.second,
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
						page.d_page_.v_scrollbar().visible() &&
						page.d_page_.v_scrollbar().contains(point))
						scroll_bar_hit = true;

					// hit test vertical scroll bar
					if (page.d_page_.v_scrollbar().pressed()) {
						if (page_alias == current_page) {
							change = true;
							contains = page.d_page_.v_scrollbar().contains(point);
							page.d_page_.v_scrollbar().hit(contains);
						}

						// capture the displacement
						page.d_page_.v_scrollbar().y_displacement_ =
							page.d_page_.v_scrollbar().y_displacement_previous_ + (point.y - point_before.y);
					}

					if (!change) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								for (auto& tab : tab_pane.p_tabs_)
									hittest_vscrollbar(tab.first, tab_pane.current_tab_, tab.second,
										point, point_before, contains, change, scroll_bar_hit);	// recursion
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									for (auto& page : pane.p_panes_)
										hittest_vscrollbar(page.first, pane.current_pane_, page.second,
											point, point_before, contains, change, scroll_bar_hit);	// recursion
								}
						}
					}
				}

				static void hittest_widgets(containers::page& page,
					const D2D1_POINT_2F& point, bool& contains, bool& change, bool lbutton_pressed, HCURSOR& h_cursor, const bool& scroll_bar_hit) {
					bool in_page = page.d_page_.contains(point);

					// hit test widgets
					for (auto& widget : page.d_page_.widgets()) {
						bool is_scroll_bar = (widget.second.type() ==
							widgets::widget_type::h_scrollbar) ||
							(widget.second.type() ==
								widgets::widget_type::v_scrollbar);

						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (change)
							break;

						contains = (in_page || is_scroll_bar || lbutton_pressed) ? widget.second.contains(point) : false;

						if (!is_scroll_bar && scroll_bar_hit)
							contains = false;

						if (change = widget.second.hit(contains)) {
							if (widget.second.hit()) h_cursor = widget.second.cursor();
							break;
						}

						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

							auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

							if (page_iterator != tab_pane.p_tabs_.end())
								helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor, scroll_bar_hit);	// recursion
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed, h_cursor, scroll_bar_hit);	// recursion
							}
					}
				}
			};

			// variable to ensure that once a scroll bar has been hit nothing else should
			bool scroll_bar_hit = false;

			if (!change) {
				for (auto& page : p_pages_)
					helper::hittest_hscrollbar(page.first, current_page_, page.second,
						point, point_before_, contains, change, scroll_bar_hit);
			}

			if (!change)
				for (auto& page : p_pages_)
					helper::hittest_vscrollbar(page.first, current_page_, page.second,
						point, point_before_, contains, change, scroll_bar_hit);

			if (!change) {
				for (auto& it : p_status_panes_)
					helper::hittest_widgets(it.second, point, contains, change, lbutton_pressed_, h_cursor, scroll_bar_hit);
			}

			if (!change) {
				auto page_iterator = p_pages_.find(current_page_);

				if (page_iterator != p_pages_.end())
					helper::hittest_widgets(page_iterator->second, point, contains, change, lbutton_pressed_, h_cursor, scroll_bar_hit);
			}

			if (!change) {
				// hit test form widgets
				for (auto& widget : widgets_) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					contains = widget.second.contains(point);
					if (change = widget.second.hit(contains))
						break;
				}
			}

			if (change) {
				h_widget_cursor_ = h_cursor;
				update();
			}
		}
	}
}
