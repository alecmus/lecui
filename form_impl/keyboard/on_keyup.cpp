//
// on_keyup.cpp - key up implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../form_impl.h"

namespace liblec {
	namespace lecui {
		void form::impl::on_keyup(WPARAM wParam) {
			switch (wParam) {
			case VK_SPACE: {
				std::function<void()> on_space = nullptr;

				// check form widgets
				for (auto& widget : _widgets) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					// failsafe: for good measure
					widget.second.hide_tooltip();

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
						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							// failsafe: for good measure
							widget.second.hide_tooltip();

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
									auto& tab_pane = page._d_page.get_tab_pane(widget.first);

									auto page_iterator = tab_pane._p_tabs.find(tab_pane._current_tab);

									if (page_iterator != tab_pane._p_tabs.end())
										helper::check_widgets(page_iterator->second, on_space);
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane(widget.first);

										auto page_iterator = pane._p_panes.find(pane._current_pane);

										if (page_iterator != pane._p_panes.end())
											helper::check_widgets(page_iterator->second, on_space);
									}

							// reset pressed status
							widget.second.press(false);
						}
					}
				};

				for (auto& it : _p_status_panes)
					helper::check_widgets(it.second, on_space);

				auto page_iterator = _p_pages.find(_current_page);

				if (page_iterator != _p_pages.end())
					helper::check_widgets(page_iterator->second, on_space);

				if (on_space) {
					update();
					on_space();
				}

				_space_pressed = false;
			}
						 break;

			case VK_SHIFT:
				_reverse_tab_navigation = false;
				_shift_pressed = false;
				break;

			case VK_TAB: {
				class helper {
				public:
					static void check_widgets(containers::page& page,
						const bool& reverse_tab_navigation,
						bool& select_next, bool& selected) {
						// check widgets
						auto widgets = page._d_page.widgets_order();
						if (reverse_tab_navigation) std::reverse(widgets.begin(), widgets.end());

						for (const auto& alias : widgets) {
							try {
								auto& widget = page._d_page.widgets().at(alias);

								if (widget.is_static() || !widget.visible() || !widget.enabled())
									continue;

								if (widget.type() ==
									widgets::widget_type::tab_pane) {

									if (widget.selected()) {
										widget.select(false);
										select_next = true;
									}

									// get this tab pane
									auto& tab_pane = page._d_page.get_tab_pane(alias);

									auto page_iterator = tab_pane._p_tabs.find(tab_pane._current_tab);

									if (page_iterator != tab_pane._p_tabs.end())
										helper::check_widgets(page_iterator->second, reverse_tab_navigation,
											select_next, selected);
								}
								else
									if (widget.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane(alias);

										auto page_iterator = pane._p_panes.find(pane._current_pane);

										if (page_iterator != pane._p_panes.end())
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
					auto widgets = _widgets_order;
					if (_reverse_tab_navigation)
						std::reverse(widgets.begin(), widgets.end());

					for (auto& alias : widgets) {
						try {
							auto& widget = _widgets.at(alias);

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
					auto it_status_bar_left = _p_status_panes.find("status::left");
					if (it_status_bar_left != _p_status_panes.end())
						helper::check_widgets(it_status_bar_left->second,
							_reverse_tab_navigation, select_next, selected);

					/// 2. top status pane
					auto it_status_bar_top = _p_status_panes.find("status::top");
					if (it_status_bar_top != _p_status_panes.end())
						helper::check_widgets(it_status_bar_top->second,
							_reverse_tab_navigation, select_next, selected);

					/// 3. page
					auto page_iterator = _p_pages.find(_current_page);
					if (page_iterator != _p_pages.end())
						helper::check_widgets(page_iterator->second,
							_reverse_tab_navigation, select_next, selected);

					/// 4. right status pane
					auto it_status_bar_right = _p_status_panes.find("status::right");
					if (it_status_bar_right != _p_status_panes.end())
						helper::check_widgets(it_status_bar_right->second,
							_reverse_tab_navigation, select_next, selected);

					/// 5. bottom status pane
					auto it_status_bar_bottom = _p_status_panes.find("status::bottom");
					if (it_status_bar_bottom != _p_status_panes.end())
						helper::check_widgets(it_status_bar_bottom->second,
							_reverse_tab_navigation, select_next, selected);

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
