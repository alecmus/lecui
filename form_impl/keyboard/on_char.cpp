/*
** on_char.cpp - character press implementation
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
		void form::impl::on_char(WPARAM wParam) {
			const char c = (char)wParam;
			bool change = false;

			class helper {
			public:
				static void check_widgets(containers::page& page, const char& c, bool& change) {
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							widgets::widget_type::textbox && widget.second.selected()) {
							change = true;
							try {
								// ignore backspace, tab and return
								if (c == '\b' ||
									c == '\t' ||
									c == '\r')
									break;

								// insert character
								auto& textbox = page.d_page_.get_textbox(widget.first);
								textbox.insert_character(c);
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
									auto& html_editor = page.d_page_.get_html_editor(widget.first);
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
										auto& combobox = page.d_page_.get_combobox(widget.first);

										// ignore backspace, tab and return
										if (c == '\b' ||
											c == '\t' ||
											c == '\r') {

											if (c == '\r' && combobox().editable)
												combobox.key_return();

											break;
										}

										// insert character

										if (combobox().editable)
											combobox.insert_character(c);
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
											helper::check_widgets(page_iterator->second, c, change);	// recursion
									}
									else
										if (widget.second.type() ==
											widgets::widget_type::pane) {
											// get this pane
											auto& pane = page.d_page_.get_pane(widget.first);

											auto page_iterator = pane.p_panes_.find(pane.current_pane_);

											if (page_iterator != pane.p_panes_.end())
												helper::check_widgets(page_iterator->second, c, change);	// recursion
										}
					}
				}
			};

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, c, change);

			// get current page
			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::check_widgets(page_iterator->second, c, change);

			if (change)
				(*this).update();
		}
	}
}
