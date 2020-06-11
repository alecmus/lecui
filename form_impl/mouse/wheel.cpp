/*
** wheel.cpp - mouse wheel implementation
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
		void form::impl::on_wheel(WPARAM wParam) {
			const long delta = GET_WHEEL_DELTA_WPARAM(wParam);
			const float units = static_cast<float>(delta) / 120.f;
			bool update = false;

			log("units: " + std::to_string(units));

			// check form widgets
			for (auto& widget : widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.hit() &&
					widget.second.type() !=
					widgets_impl::widget_type::close_button &&
					widget.second.type() !=
					widgets_impl::widget_type::maximize_button &&
					widget.second.type() !=
					widgets_impl::widget_type::minimize_button)
					if (widget.second.on_mousewheel(units))
						update = true;
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					float units, bool& update) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.hit()) {
							if (widget.second.on_mousewheel(units))
								update = true;
						}
						else
							if (widget.second.type() ==
								widgets_impl::widget_type::tab_pane) {
								// get this tab pane
								auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

								auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

								if (page_iterator != tab_pane.p_tabs_.end())
									helper::check_widgets(page_iterator->second, units, update);
							}
							else
								if (widget.second.type() ==
									widgets_impl::widget_type::pane) {
									// get this pane
									auto& pane = page.d_page_.get_pane(widget.first);

									auto page_iterator = pane.p_panes_.find(pane.current_pane_);

									if (page_iterator != pane.p_panes_.end())
										helper::check_widgets(page_iterator->second, units, update);
								}
					}
				}
			};

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, units, update);

			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::check_widgets(page_iterator->second, units, update);

			if (update)
				(*this).update();
		}

		void form::impl::on_hwheel(WPARAM wParam) {
			log("horizontal");
		}
	}
}
