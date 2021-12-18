//
// on_form_pos_changing.cpp - form position changing implementation
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
		void form::impl::on_form_pos_changing(LPARAM lParam) {
			WINDOWPOS* p_pos = (WINDOWPOS*)lParam;
			RECT rc;
			GetWindowRect(_hWnd, &rc);

			const float change_in_width =
				static_cast<float>(p_pos->cx - (rc.right - rc.left));
			const float change_in_height =
				static_cast<float>(p_pos->cy - (rc.bottom - rc.top));

			bool user_resizing = _user_sizing;
			bool system_resizing = false;

			if (!user_resizing) {
				// check if system is resizing the window
				if (((p_pos->cx != 0) && (change_in_width != 0.f)) ||
					((p_pos->cy != 0) && (change_in_height != 0.f)))
					system_resizing = true;
			}

			if (user_resizing || system_resizing) {
				class helper {
				public:
					static void check_page(containers::page& page,
						float change_in_width, float change_in_height, bool system_resizing) {
						// check if a horizontal scroll bar exists
						if (page._d_page.h_scrollbar()._x_displacement > 0.f) {
							// check if width is increasing
							if (change_in_width > 0.f) {
								if (system_resizing)
									log("system: width increasing");

								// impose a limit on change_in_width
								float x_displacement =
									page._d_page.h_scrollbar()._x_displacement - change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									page._d_page.h_scrollbar()._x_displacement - x_displacement;

								// translate the environment
								page._d_page.h_scrollbar()._x_displacement -= change;
								page._d_page.h_scrollbar()._max_displacement_left -= change;
								page._d_page.h_scrollbar()._max_displacement_right -= change;
							}
							else {
								if (system_resizing)
									log("system: width decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_width
								float x_displacement =
									page._d_page.h_scrollbar()._x_displacement + change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									x_displacement - page._d_page.h_scrollbar()._x_displacement;

								// translate the environment
								page._d_page.h_scrollbar()._x_displacement += change;
								page._d_page.h_scrollbar()._max_displacement_left += change;
								page._d_page.h_scrollbar()._max_displacement_right += change;
							}

							// force translate in on_render()
							page._d_page.h_scrollbar()._force_translate = true;
						}

						// check if a vertical scroll bar exists
						if (page._d_page.v_scrollbar()._y_displacement > 0.f) {
							// check if height is increasing
							if (change_in_height > 0.f) {
								if (system_resizing)
									log("system: height increasing");

								// impose a limit on change_in_height
								float y_displacement =
									page._d_page.v_scrollbar()._y_displacement - change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									page._d_page.v_scrollbar()._y_displacement - y_displacement;

								// translate the environment
								page._d_page.v_scrollbar()._y_displacement -= change;
								page._d_page.v_scrollbar()._max_displacement_top -= change;
								page._d_page.v_scrollbar()._max_displacement_bottom -= change;
							}
							else {
								if (system_resizing)
									log("system: height decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_height
								float y_displacement =
									page._d_page.v_scrollbar()._y_displacement + change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									y_displacement - page._d_page.v_scrollbar()._y_displacement;

								// translate the environment
								page._d_page.v_scrollbar()._y_displacement += change;
								page._d_page.v_scrollbar()._max_displacement_top += change;
								page._d_page.v_scrollbar()._max_displacement_bottom += change;
							}

							// force translate in on_render()
							page._d_page.v_scrollbar()._force_translate = true;
						}

						// to-do: check actual change in width and height of tab/pane instead of inheriting from page

						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.type() == widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								for (auto& tab : tab_pane._p_tabs)
									helper::check_page(tab.second, change_in_width, change_in_height, system_resizing);	// recursion
							}
							else
								if (widget.second.type() == widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(widget.first);

									for (auto& page : pane._p_panes)
										helper::check_page(page.second, change_in_width, change_in_height, system_resizing);	// recursion
								}
						}
					}
				};

				for (auto& page : _p_pages)
					helper::check_page(page.second, change_in_width, change_in_height, system_resizing);
			}
		}
	}
}
