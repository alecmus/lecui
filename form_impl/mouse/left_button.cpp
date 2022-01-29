//
// left_button.cpp - left mouse button implementation
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
		void form::impl::on_lbuttondown(const D2D1_POINT_2F& point) {
			for (auto& [key, child] : _m_children) {
				if (child && IsWindow(child->_d._hWnd) && (child->_d._menu_form || child->_d._tooltip_form)) {
					// close child menu forms and tooltip forms
					child->close();
				}
			}

			_point_before = point;

			bool pressed = false;
			bool selected = false;
			bool update_anyway = false;

			// check form widgets (in reverse order)
			for (const auto& alias : boost::adaptors::reverse(_widgets_order)) {
				auto& widget = _widgets.at(alias);

				if (widget.is_static() || !widget.visible() || !widget.enabled())
					continue;

				if (widget.selected())
					update_anyway = true;

				widget.hide_tooltip();

				if (!pressed) {
					// pressed widget not yet found
					pressed = widget.contains(point);
					widget.press(pressed);
					widget.select(pressed);
				}
				else {
					// pressed widget found
					widget.press(false);
					widget.select(false);
				}
			}

			class helper {
			public:
				static void check_scroll_bars(containers::page& page,
					const D2D1_POINT_2F& point, const float& dpi_scale,
					bool& scroll_bar_contains) {
					bool in_page = page._d_page.contains(point);

					// check widgets (in reverse order)
					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						bool is_scroll_bar = (widget.type() == widgets::widget_type::h_scrollbar) ||
							(widget.type() == widgets::widget_type::v_scrollbar);

						if (widget.is_static() || !widget.visible() || !widget.enabled())
							continue;

						if (in_page && is_scroll_bar && widget.contains(point))
							scroll_bar_contains = true;
					}

					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						if (widget.type() == widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							if (page_iterator != tab_pane._p_tabs.end())
								helper::check_scroll_bars(page_iterator->second, point, dpi_scale, scroll_bar_contains);
						}
						else
							if (widget.type() == widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(alias);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								if (page_iterator != pane._p_panes.end())
									helper::check_scroll_bars(page_iterator->second, point, dpi_scale, scroll_bar_contains);
							}
					}
				}

				static void check_widgets(containers::page& page,
					const D2D1_POINT_2F& point, const float& dpi_scale, bool& pressed,
					bool& update_anyway, const bool& scroll_bar_hit) {
					bool in_page = page._d_page.contains(point);

					// check widgets (in reverse order)
					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						bool is_scroll_bar = (widget.type() == widgets::widget_type::h_scrollbar) ||
							(widget.type() == widgets::widget_type::v_scrollbar);

						if (widget.is_static() || !widget.visible() || !widget.enabled())
							continue;

						if (widget.selected())
							update_anyway = true;

						widget.hide_tooltip();

						if (!pressed) {
							// pressed widget not yet found
							bool contains = widget.contains(point);

							if (!is_scroll_bar && scroll_bar_hit)
								contains = false;

							pressed = (in_page || is_scroll_bar) ? contains : false;

							widget.press(pressed);

							if (widget.type() != widgets::widget_type::tab_pane &&
								widget.type() != widgets::widget_type::pane)
								widget.select(pressed);
						}
						else {
							// pressed widget found
							widget.press(false);
							widget.select(false);
						}
					}

					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						if (widget.type() == widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							bool local_pressed = widget.pressed() ? false : pressed;

							if (page_iterator != tab_pane._p_tabs.end())
								helper::check_widgets(page_iterator->second, point, dpi_scale, local_pressed,
									update_anyway, scroll_bar_hit);	// recursion

							if (local_pressed)
								pressed = local_pressed;

							if (widget.pressed() && local_pressed) {
								// prioritize widget over pane since widget is in pane
								widget.press(false);
							}
						}
						else
							if (widget.type() == widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(alias);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								bool local_pressed = widget.pressed() ? false : pressed;

								if (page_iterator != pane._p_panes.end())
									helper::check_widgets(page_iterator->second, point, dpi_scale, local_pressed,
										update_anyway, scroll_bar_hit);	// recursion

								if (local_pressed)
									pressed = local_pressed;

								if (widget.pressed() && local_pressed) {
									// prioritize widget over pane since widget is in pane
									widget.press(false);
								}
							}
					}

					// check scroll bars
					if (page._d_page.h_scrollbar().pressed()) {
						page._d_page.h_scrollbar().max_displacement(
							page._d_page.h_scrollbar()._max_displacement_left,
							page._d_page.h_scrollbar()._max_displacement_right);
						page._d_page.h_scrollbar()._max_displacement_left *= dpi_scale;
						page._d_page.h_scrollbar()._max_displacement_right *= dpi_scale;

						// translate the environment
						page._d_page.h_scrollbar()._x_displacement_previous =
							page._d_page.h_scrollbar()._x_displacement;
						page._d_page.h_scrollbar()._max_displacement_left +=
							page._d_page.h_scrollbar()._x_displacement_previous;
						page._d_page.h_scrollbar()._max_displacement_right +=
							page._d_page.h_scrollbar()._x_displacement_previous;
					}
					else
						if (page._d_page.v_scrollbar().pressed()) {
							page._d_page.v_scrollbar().max_displacement(
								page._d_page.v_scrollbar()._max_displacement_top,
								page._d_page.v_scrollbar()._max_displacement_bottom);
							page._d_page.v_scrollbar()._max_displacement_top *= dpi_scale;
							page._d_page.v_scrollbar()._max_displacement_bottom *= dpi_scale;

							// translate the environment
							page._d_page.v_scrollbar()._y_displacement_previous =
								page._d_page.v_scrollbar()._y_displacement;
							page._d_page.v_scrollbar()._max_displacement_top +=
								page._d_page.v_scrollbar()._y_displacement_previous;
							page._d_page.v_scrollbar()._max_displacement_bottom +=
								page._d_page.v_scrollbar()._y_displacement_previous;
						}
				}
			};

			// variable to ensure that once a scroll bar has been hit nothing else should
			bool scroll_bar_hit = false;

			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_scroll_bars(page_iterator->second, point, _dpi_scale, scroll_bar_hit);

			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second, point, _dpi_scale, pressed, update_anyway, scroll_bar_hit);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second, point, _dpi_scale, pressed, update_anyway, scroll_bar_hit);

			if (pressed || update_anyway)
				update();
		}

		void form::impl::on_lbuttonup(const D2D1_POINT_2F& point) {
			bool clicked = false;
			bool update_anyway = false;
			std::function<void()> on_click_handler = nullptr;

			// check form widgets (in reverse order)
			for (const auto& alias : boost::adaptors::reverse(_widgets_order)) {
				auto& widget = _widgets.at(alias);

				if (widget.is_static() || !widget.visible() || !widget.enabled())
					continue;

				if (!clicked &&
					widget.pressed() &&
					widget.contains(point)) {
					clicked = true;
					on_click_handler = [&]() { widget.on_click(); };
				}

				if (widget.pressed())
					update_anyway = true;

				// reset pressed status
				widget.press(false);
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					const D2D1_POINT_2F& point, bool& clicked, bool& update_anyway,
					std::function<void()>& on_click_handler) {

					// check widgets (in reverse order)
					for (const auto& alias : boost::adaptors::reverse(page._d_page.widgets_order())) {
						auto& widget = page._d_page.widgets().at(alias);

						if (widget.is_static() || !widget.visible() || !widget.enabled())
							continue;

						if (!clicked &&
							widget.pressed() &&
							widget.contains(point)) {
							clicked = true;
							on_click_handler = [&]() { widget.on_click(); };
						}

						if (widget.pressed())
							update_anyway = true;

						// reset pressed status
						widget.press(false);

						// failsafe: for good measure
						widget.hide_tooltip();

						if (widget.type() == widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page._d_page.get_tab_pane_impl(alias);

							auto page_iterator = tab_pane._p_tabs.find(tab_pane.specs().selected());

							if (page_iterator != tab_pane._p_tabs.end())
								check_widgets(page_iterator->second, point, clicked, update_anyway,
									on_click_handler);
						}
						else
							if (widget.type() == widgets::widget_type::pane) {
								// get this pane
								auto& pane = page._d_page.get_pane_impl(alias);

								auto page_iterator = pane._p_panes.find(pane._current_pane);

								if (page_iterator != pane._p_panes.end())
									check_widgets(page_iterator->second, point, clicked, update_anyway,
										on_click_handler);
							}
					}
				}
			};

			for (auto& it : _p_status_panes)
				helper::check_widgets(it.second, point, clicked, update_anyway,
					on_click_handler);

			auto page_iterator = _p_pages.find(_current_page);

			if (page_iterator != _p_pages.end())
				helper::check_widgets(page_iterator->second, point, clicked, update_anyway,
					on_click_handler);

			if (clicked || update_anyway) {
				update();

				// invoke the on_click handler
				if (clicked && on_click_handler)
					on_click_handler();
			}
		}
	}
}
