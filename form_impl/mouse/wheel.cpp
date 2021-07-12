//
// wheel.cpp - mouse wheel implementation
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
		void form::impl::on_wheel(WPARAM wParam) {
			const long delta = GET_WHEEL_DELTA_WPARAM(wParam);
			const float units = static_cast<float>(delta) / 120.f;
			bool update = false;

			// check form widgets
			for (auto& widget : _widgets) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.hit() &&
					widget.second.type() !=
					widgets::widget_type::close_button &&
					widget.second.type() !=
					widgets::widget_type::maximize_button &&
					widget.second.type() !=
					widgets::widget_type::minimize_button)
					if (widget.second.on_mousewheel(units))
						update = true;
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					float units, bool& update) {
					// check widgets
					for (auto& widget : page._d_page.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.hit()) {
							if (widget.second.on_mousewheel(units))
								update = true;
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page._d_page.get_tab_pane(widget.first);

								auto page_iterator = tab_pane._p_tabs.find(tab_pane._current_tab);

								if (page_iterator != tab_pane._p_tabs.end()) {
									helper::check_widgets(page_iterator->second, units, update);

									if (!update && page_iterator->second._d_page.hit())
										if (page_iterator->second._d_page.on_mousewheel(units))
											update = true;
								}
							}
							else
								if (widget.second.type() ==
									widgets::widget_type::pane) {
									// get this pane
									auto& pane = page._d_page.get_pane(widget.first);

									auto page_iterator = pane._p_panes.find(pane._current_pane);

									if (page_iterator != pane._p_panes.end()) {
										helper::check_widgets(page_iterator->second, units, update);

										if (!update && page_iterator->second._d_page.hit())
											if (page_iterator->second._d_page.on_mousewheel(units))
												update = true;
									}
								}
					}
				}
			};

			/// Check all the widgets in a page
			/// If none of the widgets handled the mousewheel check the page itself
			/// Checking is recursive, reaching all widgets in all containers

			for (auto& it : _p_status_panes) {
				helper::check_widgets(it.second, units, update);

				if (!update && it.second._d_page.hit())
					if (it.second._d_page.on_mousewheel(units))
						update = true;
			}

			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end()) {
				helper::check_widgets(page_iterator->second, units, update);

				if (!update && page_iterator->second._d_page.hit())
					if (page_iterator->second._d_page.on_mousewheel(units))
						update = true;
			}

			if (update)
				(*this).update();
		}

		void form::impl::on_hwheel(WPARAM wParam) {
			log("horizontal");
		}
	}
}
