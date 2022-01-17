//
// right_button.cpp - right mouse button implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../form_impl.h"
#include <boost/range/adaptor/reversed.hpp>

namespace liblec {
	namespace lecui {
		void form::impl::on_rbuttondown(const D2D1_POINT_2F& point) {
			for (auto& [key, child] : _m_children) {
				if (child && IsWindow(child->_d._hWnd) && (child->_d._menu_form || child->_d._tooltip_form)) {
					// close child menu forms and tooltip forms
					child->close();
				}
			}
		}

		void form::impl::on_rbuttonup(const D2D1_POINT_2F& point) {
			bool right_clicked = false;
			std::function<void()> on_right_click_handler = nullptr;

			// check form widgets (in reverse order)
			for (const auto& alias : boost::adaptors::reverse(_widgets_order)) {
				auto& widget = _widgets.at(alias);

				if (widget.is_static() || !widget.visible() || !widget.enabled())
					continue;

				if (right_clicked)
					break;

				if (!right_clicked &&
					widget.contains(point)) {
					right_clicked = true;
					on_right_click_handler = [&]() { widget.on_right_click(); };
				}
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					const D2D1_POINT_2F& point, bool& clicked,
					std::function<void()>& on_right_click_handler) {

					// check widgets (in reverse order)
					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						if (widget.is_static() || !widget.visible() || !widget.enabled())
							continue;

						if (!clicked && widget.contains(point)) {
							clicked = true;
							on_right_click_handler = [&]() { widget.on_right_click(); };
						}

						// failsafe: for good measure
						widget.hide_tooltip();

						if (widget.type() == widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							if (page_iterator != tab_pane._p_tabs.end())
								check_widgets(page_iterator->second, point, clicked,
									on_right_click_handler);
						}
						else
							if (widget.type() == widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(alias);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								if (page_iterator != pane._p_panes.end())
									check_widgets(page_iterator->second, point, clicked,
										on_right_click_handler);
							}
					}
				}
			};

			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second, point, right_clicked,
					on_right_click_handler);

			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second, point, right_clicked,
					on_right_click_handler);

			if (right_clicked) {
				// invoke the on_right_click handler
				if (right_clicked && on_right_click_handler)
					on_right_click_handler();
			}
		}
	}
}
