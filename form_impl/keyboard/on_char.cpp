//
// on_char.cpp - character press implementation
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
		void form::impl::on_char(WPARAM wParam) {
			const char c = (char)wParam;
			bool change = false;

			class helper {
			public:
				static void check_widgets(containers::page& page, const char& c, bool& change) {
					for (auto& widget : page._d_page.widgets()) {
						// failsafe: for good measure
						widget.second.hide_tooltip();

						if (widget.second.type() ==
							widgets::widget_type::text_field && widget.second.selected()) {
							change = true;
							try {
								// ignore backspace, tab and return
								if (c == '\b' ||
									c == '\t' ||
									c == '\r')
									break;

								// insert character
								auto& text_field = page._d_page.get_text_field_impl(widget.first);
								text_field.insert_character(c);
							}
							catch (const std::exception& e) { log(e.what()); }
							break;
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::html_editor && widget.second.selected()) {
								change = true;
								try {
									// ignore backspace, tab and return
									if (c == '\b' ||
										c == '\t' ||
										c == '\r')
										break;

									// insert character
									auto& html_editor = page._d_page.get_html_editor_impl(widget.first);
									html_editor.insert_character(c);
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

										// ignore backspace, tab and return
										if (c == '\b' ||
											c == '\t' ||
											c == '\r') {

											if (c == '\r' && combobox().editable())
												combobox.key_return();

											break;
										}

										// insert character

										if (combobox().editable())
											combobox.insert_character(c);
									}
									catch (const std::exception& e) { log(e.what()); }
									break;
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::tab_pane) {
										// get this tab pane
										auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

										auto page_iterator = tab_pane._p_tabs.find(tab_pane._current_tab);

										if (page_iterator != tab_pane._p_tabs.end())
											helper::check_widgets(page_iterator->second, c, change);	// recursion
									}
									else
										if (widget.second.type() ==
											widgets::widget_type::pane) {
											// get this pane
											auto& pane = page._d_page.get_pane_impl(widget.first);

											auto page_iterator = pane._p_panes.find(pane._current_pane);

											if (page_iterator != pane._p_panes.end())
												helper::check_widgets(page_iterator->second, c, change);	// recursion
										}
					}
				}
			};

			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second, c, change);

			// get current page
			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second, c, change);

			if (change)
				(*this).update();
		}
	}
}
