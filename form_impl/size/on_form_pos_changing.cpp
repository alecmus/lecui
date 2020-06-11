/*
** on_form_pos_changing.cpp - form position changing implementation
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
		void form::impl::on_form_pos_changing(LPARAM lParam) {
			WINDOWPOS* p_pos = (WINDOWPOS*)lParam;
			RECT rc;
			GetWindowRect(hWnd_, &rc);

			const float change_in_width =
				static_cast<float>(p_pos->cx - (rc.right - rc.left));
			const float change_in_height =
				static_cast<float>(p_pos->cy - (rc.bottom - rc.top));

			bool user_resizing = user_sizing_;
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
						if (page.d_page_.h_scrollbar().x_displacement_ > 0.f) {
							// check if width is increasing
							if (change_in_width > 0.f) {
								if (system_resizing)
									log("system: width increasing");

								// impose a limit on change_in_width
								float x_displacement =
									page.d_page_.h_scrollbar().x_displacement_ - change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									page.d_page_.h_scrollbar().x_displacement_ - x_displacement;

								// translate the environment
								page.d_page_.h_scrollbar().x_displacement_ -= change;
								page.d_page_.h_scrollbar().max_displacement_left_ -= change;
								page.d_page_.h_scrollbar().max_displacement_right_ -= change;
							}
							else {
								if (system_resizing)
									log("system: width decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_width
								float x_displacement =
									page.d_page_.h_scrollbar().x_displacement_ + change_in_width;
								x_displacement = largest(x_displacement, 0.f);

								const float change =
									x_displacement - page.d_page_.h_scrollbar().x_displacement_;

								// translate the environment
								page.d_page_.h_scrollbar().x_displacement_ += change;
								page.d_page_.h_scrollbar().max_displacement_left_ += change;
								page.d_page_.h_scrollbar().max_displacement_right_ += change;
							}

							// force translate in on_render()
							page.d_page_.h_scrollbar().force_translate_ = true;
						}

						// check if a vertical scroll bar exists
						if (page.d_page_.v_scrollbar().y_displacement_ > 0.f) {
							// check if height is increasing
							if (change_in_height > 0.f) {
								if (system_resizing)
									log("system: height increasing");

								// impose a limit on change_in_height
								float y_displacement =
									page.d_page_.v_scrollbar().y_displacement_ - change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									page.d_page_.v_scrollbar().y_displacement_ - y_displacement;

								// translate the environment
								page.d_page_.v_scrollbar().y_displacement_ -= change;
								page.d_page_.v_scrollbar().max_displacement_top_ -= change;
								page.d_page_.v_scrollbar().max_displacement_bottom_ -= change;
							}
							else {
								if (system_resizing)
									log("system: height decreasing");	// to-do: issue with scrollbar (sometimes) ...

								// impose a limit on change_in_height
								float y_displacement =
									page.d_page_.v_scrollbar().y_displacement_ + change_in_height;
								y_displacement = largest(y_displacement, 0.f);

								const float change =
									y_displacement - page.d_page_.v_scrollbar().y_displacement_;

								// translate the environment
								page.d_page_.v_scrollbar().y_displacement_ += change;
								page.d_page_.v_scrollbar().max_displacement_top_ += change;
								page.d_page_.v_scrollbar().max_displacement_bottom_ += change;
							}

							// force translate in on_render()
							page.d_page_.v_scrollbar().force_translate_ = true;
						}

						// to-do: check actual change in width and height of tab/pane instead of inheriting from page

						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() == widgets_impl::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								for (auto& tab : tab_pane.p_tabs_)
									helper::check_page(tab.second, change_in_width, change_in_height, system_resizing);	// recursion
							}
							else
								if (widget.second.type() == widgets_impl::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									for (auto& page : pane.p_panes_)
										helper::check_page(page.second, change_in_width, change_in_height, system_resizing);	// recursion
								}
						}
					}
				};

				for (auto& page : p_pages_)
					helper::check_page(page.second, change_in_width, change_in_height, system_resizing);
			}
		}
	}
}
