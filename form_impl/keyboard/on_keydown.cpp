/*
** on_keydown.cpp - key down implementation
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
		void form::impl::on_keydown(WPARAM wParam) {
			switch (wParam) {
			case VK_LEFT:
			case VK_UP:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_BACK:
			case VK_DELETE: {
				bool change = false;

				class helper {
				public:
					static void check_widgets(containers::page& page, WPARAM wParam, bool& change) {
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.type() ==
								widgets::widget_type::textbox && widget.second.selected()) {
								change = true;
								try {
									auto& textbox = page.d_page_.get_textbox(widget.first);

									switch (wParam) {
									case VK_LEFT: textbox.key_left(); break;
									case VK_RIGHT: textbox.key_right(); break;
									case VK_BACK: textbox.key_backspace(); break;
									case VK_DELETE: textbox.key_delete(); break;
									default:
										break;
									}
								}
								catch (const std::exception& e) { log(e.what()); }
								break;
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::html_editor && widget.second.selected()) {
									change = true;
									try {
										auto& html_editor = page.d_page_.get_html_editor(widget.first);

										switch (wParam) {
										case VK_LEFT: html_editor.key_left(); break;
										case VK_UP:	html_editor.key_up(); break;
										case VK_RIGHT: html_editor.key_right(); break;
										case VK_DOWN: html_editor.key_down(); break;
										case VK_BACK: html_editor.key_backspace(); break;
										case VK_DELETE: html_editor.key_delete(); break;
										default:
											break;
										}
									}
									catch (const std::exception& e) { log(e.what()); }
									break;
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::combobox && widget.second.selected()) {
										change = true;
										try {
											auto& combobox = page.d_page_.get_combobox(widget.first);

											if (combobox().editable) {
												switch (wParam) {
												case VK_LEFT: combobox.key_left(); break;
												case VK_RIGHT: combobox.key_right(); break;
												case VK_BACK: combobox.key_backspace(); break;
												case VK_DELETE: combobox.key_delete(); break;
												default:
													break;
												}
											}
										}
										catch (const std::exception& e) { log(e.what()); }
										break;
									}
									else
										if (widget.second.type() ==
											widgets::widget_type::tab_pane) {
											// get this tab pane
											auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

											auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

											if (page_iterator != tab_pane.p_tabs_.end())
												helper::check_widgets(page_iterator->second, wParam, change);	// recursion
										}
										else
											if (widget.second.type() ==
												widgets::widget_type::pane) {
												// get this pane
												auto& pane = page.d_page_.get_pane(widget.first);

												auto page_iterator = pane.p_panes_.find(pane.current_pane_);

												if (page_iterator != pane.p_panes_.end())
													helper::check_widgets(page_iterator->second, wParam, change);	// recursion
											}
						}
					}
				};

				for (auto& it : p_status_panes_)
					helper::check_widgets(it.second, wParam, change);

				// get current page
				auto page_iterator = p_pages_.find(current_page_);

				if (page_iterator != p_pages_.end())
					helper::check_widgets(page_iterator->second, wParam, change);

				if (change)
					update();
			} break;

			case VK_SHIFT:
				if (shift_pressed_)
					break;
				shift_pressed_ = true;
				reverse_tab_navigation_ = true;
				break;

			case VK_SPACE: {
				if (space_pressed_)
					break;
				space_pressed_ = true;

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
						widgets::widget_type::minimize_button)
						widget.second.press(true);
					else
						widget.second.press(false);
				}

				class helper {
				public:
					static void check_widgets(containers::page& page) {
						// check widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							if (widget.second.selected() &&
								widget.second.type() != widgets::widget_type::textbox &&	// exclude text box from space bar presses
								widget.second.type() != widgets::widget_type::html_editor)	// exclude html editor from space bar presses
								widget.second.press(true);
							else
								widget.second.press(false);

							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

								if (page_iterator != tab_pane.p_tabs_.end())
									helper::check_widgets(page_iterator->second);
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second);
								}
						}
					}
				};

				for (auto& it : p_status_panes_)
					helper::check_widgets(it.second);

				auto page_iterator = p_pages_.find(current_page_);

				if (page_iterator != p_pages_.end())
					helper::check_widgets(page_iterator->second);

				update();
			}
						 break;

			case VK_RETURN: {
				bool update = false;
				std::function<void()> on_action_handler = nullptr;

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
						widgets::widget_type::minimize_button)
						on_action_handler = [&]() {
						widget.second.on_action();
						widget.second.on_click();
					};
				}

				class helper {
				public:
					static void check_widgets(containers::page& page,
						WPARAM wParam, bool& update, std::function<void()>& on_action_handler) {
						// check widgets
						for (auto& widget : page.d_page_.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							if (widget.second.type() == widgets::widget_type::html_editor)
								continue;	// this widget uses the enter key for paragraphs

							if (widget.second.selected()) {
								update = true;
								on_action_handler = [&]() {
									widget.second.on_action();
									widget.second.on_click(); 
								};
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

									auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

									if (page_iterator != tab_pane.p_tabs_.end())
										helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page.d_page_.get_pane(widget.first);

										auto page_iterator = pane.p_panes_.find(pane.current_pane_);

										if (page_iterator != pane.p_panes_.end())
											helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);
									}
						}
					}
				};

				for (auto& it : p_status_panes_)
					helper::check_widgets(it.second, wParam, update, on_action_handler);

				auto page_iterator = p_pages_.find(current_page_);

				if (page_iterator != p_pages_.end())
					helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);

				if (update) {
					(*this).update();

					if (on_action_handler) {
						on_action_handler();
					}
				}
			}
				break;

			default:
				break;
			}

			bool update = false;
			std::function<void()> on_click_handler = nullptr; // to-do: what is this doing here? is it because of the broken table widget that's pending a revamp? Eliminate in time!!!!

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
					widgets::widget_type::minimize_button)
					if (widget.second.on_keydown(wParam)) {
						update = true;
						on_click_handler = [&]() { widget.second.on_click(); };
					}
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					WPARAM wParam, bool& update, std::function<void()>& on_click_handler) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.selected()) {
							if (widget.second.on_keydown(wParam)) {
								update = true;
								on_click_handler = [&]() { widget.second.on_click(); };
							}
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

								if (page_iterator != tab_pane.p_tabs_.end())
									helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
								}
					}
				}
			};

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, wParam, update, on_click_handler);

			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);

			if (update) {
				(*this).update();

				if (on_click_handler)
					on_click_handler();
			}
		}
	}
}
