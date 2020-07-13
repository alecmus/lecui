/*
** on_keyup.cpp - key up implementation
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
		void form::impl::on_keyup(WPARAM wParam) {
			switch (wParam) {
			case VK_SPACE: {
				std::function<void()> on_space = nullptr;

				// check form widgets
				for (auto& widget : widgets_) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					if (widget.second.selected() &&
						widget.second.type() !=
						widgets::widget_type::close_button &&
						widget.second.type() !=
						widgets::widget_type::maximize_button &&
						widget.second.type() !=
						widgets::widget_type::minimize_button) {
						on_space = [&]() { widget.second.on_action(); };
					}

					// reset pressed status
					widget.second.press(false);
				}

				class helper {
				public:
					static void check_widgets(containers::page& page,
						std::function<void()>& on_space) {
						// check widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							if (widget.second.type() == widgets::widget_type::text_field ||
								widget.second.type() == widgets::widget_type::html_editor ||
								widget.second.type() == widgets::widget_type::combobox)
								continue;	// these widgets use the space key

							if (widget.second.selected()) {
								on_space = [&]() { widget.second.on_action(); };
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

									auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

									if (page_iterator != tab_pane.p_tabs_.end())
										helper::check_widgets(page_iterator->second, on_space);
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										auto page_iterator = pane.p_panes_.find(pane.current_pane_);

										if (page_iterator != pane.p_panes_.end())
											helper::check_widgets(page_iterator->second, on_space);
									}

							// reset pressed status
							widget.second.press(false);
						}
					}
				};

				for (auto& it : p_status_panes_)
					helper::check_widgets(it.second, on_space);

				auto page_iterator = p_pages_.find(current_page_);

				if (page_iterator != p_pages_.end())
					helper::check_widgets(page_iterator->second, on_space);

				if (on_space) {
					update();
					on_space();
				}

				space_pressed_ = false;
			}
						 break;

			case VK_SHIFT:
				reverse_tab_navigation_ = false;
				shift_pressed_ = false;
				break;

			case VK_TAB: {
				class helper {
				public:
					static void check_widgets(containers::page& page,
						const bool& reverse_tab_navigation,
						bool& select_next, bool& selected) {
						// check widgets
						auto widgets = page.d_page_.widgets_order();
						if (reverse_tab_navigation) std::reverse(widgets.begin(), widgets.end());

						for (const auto& alias : widgets) {
							try {
								auto& widget = page.d_page_.widgets().at(alias);

								if (widget.is_static() || !widget.visible() || !widget.enabled())
									continue;

								if (widget.type() ==
									widgets::widget_type::tab_pane) {

									if (widget.selected()) {
										widget.select(false);
										select_next = true;
									}

									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(alias);

									auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

									if (page_iterator != tab_pane.p_tabs_.end())
										helper::check_widgets(page_iterator->second, reverse_tab_navigation,
											select_next, selected);
								}
								else
									if (widget.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(alias);

										auto page_iterator = pane.p_panes_.find(pane.current_pane_);

										if (page_iterator != pane.p_panes_.end())
											helper::check_widgets(page_iterator->second, reverse_tab_navigation,
												select_next, selected);
									}
									else {
										if (widget.selected()) {
											widget.select(false);
											select_next = true;
										}
										else
											if (select_next && !selected) {
												widget.select(true);
												selected = true;
											}
									}
							}
							catch (const std::exception& e) { log(e.what()); }
						}
					}
				};

				bool select_next = false;
				bool selected = false;

				// loop twice because the currently selected widget may be the last to be checked
				for (size_t i = 0; i < 2; i++) {
					// check form widgets
					auto widgets = widgets_order_;
					if (reverse_tab_navigation_)
						std::reverse(widgets.begin(), widgets.end());

					for (auto& alias : widgets) {
						try {
							auto& widget = widgets_.at(alias);

							if (widget.is_static() || !widget.visible() || !widget.enabled())
								continue;

							if (widget.type() !=
								widgets::widget_type::close_button &&
								widget.type() !=
								widgets::widget_type::maximize_button &&
								widget.type() !=
								widgets::widget_type::minimize_button) {
								if (widget.selected()) {
									widget.select(false);
									select_next = true;
								}
								else
									if (select_next && !selected) {
										widget.select(true);
										selected = true;
									}
							}
						}
						catch (const std::exception& e) { log(e.what()); }
					}

					/// navigation order
					/// 1. left status pane
					/// 2. top status pane
					/// 3. page
					/// 4. right status pane
					/// 5. bottom status pane

					/// 1. left status pane
					auto it_status_bar_left = p_status_panes_.find("status::left");
					if (it_status_bar_left != p_status_panes_.end())
						helper::check_widgets(it_status_bar_left->second,
							reverse_tab_navigation_, select_next, selected);

					/// 2. top status pane
					auto it_status_bar_top = p_status_panes_.find("status::top");
					if (it_status_bar_top != p_status_panes_.end())
						helper::check_widgets(it_status_bar_top->second,
							reverse_tab_navigation_, select_next, selected);

					/// 3. page
					auto page_iterator = p_pages_.find(current_page_);
					if (page_iterator != p_pages_.end())
						helper::check_widgets(page_iterator->second,
							reverse_tab_navigation_, select_next, selected);

					/// 4. right status pane
					auto it_status_bar_right = p_status_panes_.find("status::right");
					if (it_status_bar_right != p_status_panes_.end())
						helper::check_widgets(it_status_bar_right->second,
							reverse_tab_navigation_, select_next, selected);

					/// 5. bottom status pane
					auto it_status_bar_bottom = p_status_panes_.find("status::bottom");
					if (it_status_bar_bottom != p_status_panes_.end())
						helper::check_widgets(it_status_bar_bottom->second,
							reverse_tab_navigation_, select_next, selected);

					if (select_next)
						update();

					if (selected)
						break;

					select_next = true;	// no widget was selected
				}
			}
					   break;

			default:
				break;
			}
		}
	}
}
