//
// on_keydown.cpp - key down implementation
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
						for (auto& widget : page._d_page.widgets()) {
							// first things first
							widget.second.hide_tooltip();

							if (widget.second.type() ==
								widgets::widget_type::text_field && widget.second.selected()) {
								change = true;
								try {
									auto& text_field = page._d_page.get_text_field_impl(widget.first);

									switch (wParam) {
									case VK_LEFT: text_field.key_left(); break;
									case VK_RIGHT: text_field.key_right(); break;
									case VK_BACK: text_field.key_backspace(); break;
									case VK_DELETE: text_field.key_delete(); break;
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
										auto& html_editor = page._d_page.get_html_editor_impl(widget.first);

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
											auto& combobox = page._d_page.get_combobox_impl(widget.first);

											if (combobox().editable()) {
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
											auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

											auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

											if (page_iterator != tab_pane._p_tabs.end())
												helper::check_widgets(page_iterator->second, wParam, change);	// recursion
										}
										else
											if (widget.second.type() ==
												widgets::widget_type::pane) {
												// get this pane
												auto& pane = page._d_page.get_pane_impl(widget.first);

												auto page_iterator = pane._p_panes.find(pane._current_pane);

												if (page_iterator != pane._p_panes.end())
													helper::check_widgets(page_iterator->second, wParam, change);	// recursion
											}
						}
					}
				};

				for (auto& it : _p_status_panes)
					helper::check_widgets(it.second, wParam, change);

				// get current page
				auto page_iterator = _p_pages.find(_current_page);

				if (page_iterator != _p_pages.end())
					helper::check_widgets(page_iterator->second, wParam, change);

				if (change)
					update();
			} break;

			case VK_SHIFT:
				if (_shift_pressed)
					break;
				_shift_pressed = true;
				_reverse_tab_navigation = true;
				break;

			case VK_CONTROL:
				if (!_ctrl_pressed)
					log("Ctrl pressed");
				_ctrl_pressed = true;
				break;

			case VK_SPACE: {
				if (_space_pressed)
					break;
				_space_pressed = true;

				// check form widgets
				for (auto& widget : _widgets) {
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
						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							// first things first
							widget.second.hide_tooltip();

							if (widget.second.selected() &&
								widget.second.type() != widgets::widget_type::text_field &&	// exclude text box from space bar presses
								widget.second.type() != widgets::widget_type::html_editor)	// exclude html editor from space bar presses
								widget.second.press(true);
							else
								widget.second.press(false);

							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

								if (page_iterator != tab_pane._p_tabs.end())
									helper::check_widgets(page_iterator->second);
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(widget.first);

									auto page_iterator = pane._p_panes.find(pane._current_pane);

									if (page_iterator != pane._p_panes.end())
										helper::check_widgets(page_iterator->second);
								}
						}
					}
				};

				for (auto& it : _p_status_panes)
					helper::check_widgets(it.second);

				auto page_iterator = _p_pages.find(_current_page);

				if (page_iterator != _p_pages.end())
					helper::check_widgets(page_iterator->second);

				update();
			}
						 break;

			case VK_RETURN: {
				bool update = false;
				std::function<void()> on_action_handler = nullptr;

				// check form widgets
				for (auto& widget : _widgets) {
					if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
						continue;

					// first things first
					widget.second.hide_tooltip();

					if (widget.second.selected() &&
						widget.second.type() !=
						widgets::widget_type::close_button &&
						widget.second.type() !=
						widgets::widget_type::maximize_button &&
						widget.second.type() !=
						widgets::widget_type::minimize_button) {
						update = true;
						on_action_handler = [&]() { widget.second.on_action(); };
					}
				}

				class helper {
				public:
					static void check_widgets(containers::page& page,
						WPARAM wParam, bool& update, std::function<void()>& on_action_handler) {
						// check widgets
						for (auto& widget : page._d_page.widgets()) {
							if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
								continue;

							// first things first
							widget.second.hide_tooltip();

							if (widget.second.type() == widgets::widget_type::html_editor)
								continue;	// this widget uses the enter key for paragraphs

							if (widget.second.selected()) {
								update = true;
								on_action_handler = [&]() { widget.second.on_action(); };
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									// get this tab pane
									auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

									auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

									if (page_iterator != tab_pane._p_tabs.end())
										helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										// get this pane
										auto& pane = page._d_page.get_pane_impl(widget.first);

										auto page_iterator = pane._p_panes.find(pane._current_pane);

										if (page_iterator != pane._p_panes.end())
											helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);
									}
						}
					}
				};

				for (auto& it : _p_status_panes)
					helper::check_widgets(it.second, wParam, update, on_action_handler);

				auto page_iterator = _p_pages.find(_current_page);

				if (page_iterator != _p_pages.end())
					helper::check_widgets(page_iterator->second, wParam, update, on_action_handler);

				if (update) {
					(*this).update();

					if (on_action_handler)
						on_action_handler();
				}
			}
				break;

			default: {
				char c = (char)wParam;

				if (_ctrl_pressed && c == 'A') {
					// 'ctrl + a' pressed

					bool update = false;

					class helper {
					public:
						static void check_widgets(containers::page& page, bool& update) {
							// check widgets
							for (auto& widget : page._d_page.widgets()) {
								if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
									continue;

								// first things first
								widget.second.hide_tooltip();

								if (widget.second.type() != widgets::widget_type::html_editor &&
									widget.second.type() != widgets::widget_type::text_field)
									continue;

								if (widget.second.selected()) {
									update = true;

									log("'ctrl + a' pressed for " + widget.first);

									if (widget.second.type() == widgets::widget_type::text_field) {
										try {
											// select all
											auto& text_field = page._d_page.get_text_field_impl(widget.first);
											text_field.select_all();
										}
										catch (const std::exception& e) { log(e.what()); }
									}
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::tab_pane) {
										// get this tab pane
										auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

										auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

										if (page_iterator != tab_pane._p_tabs.end())
											helper::check_widgets(page_iterator->second, update);
									}
									else
										if (widget.second.type() ==
											widgets::widget_type::pane) {
											// get this pane
											auto& pane = page._d_page.get_pane_impl(widget.first);

											auto page_iterator = pane._p_panes.find(pane._current_pane);

											if (page_iterator != pane._p_panes.end())
												helper::check_widgets(page_iterator->second, update);
										}
							}
						}
					};

					for (auto& it : _p_status_panes)
						helper::check_widgets(it.second, update);

					auto page_iterator = _p_pages.find(_current_page);

					if (page_iterator != _p_pages.end())
						helper::check_widgets(page_iterator->second, update);

					if (update)
						(*this).update();
				}
			}
				break;
			}

			bool update = false;
			std::function<void()> on_click_handler = nullptr; // to-do: what is this doing here? is it because of the broken table widget that's pending a revamp? Eliminate in time!!!!

			// check form widgets
			for (auto& widget : _widgets) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				// first things first
				widget.second.hide_tooltip();

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
					for (auto& widget : page._d_page.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						// first things first
						widget.second.hide_tooltip();

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
								auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

								auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

								if (page_iterator != tab_pane._p_tabs.end())
									helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane_impl(widget.first);

									auto page_iterator = pane._p_panes.find(pane._current_pane);

									if (page_iterator != pane._p_panes.end())
										helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);
								}
					}
				}
			};

			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second, wParam, update, on_click_handler);

			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second, wParam, update, on_click_handler);

			if (update) {
				(*this).update();

				if (on_click_handler)
					on_click_handler();
			}
		}
	}
}
