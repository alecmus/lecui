//
// left_button.cpp - left mouse button implementation
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
		void form::impl::on_lbuttondown(const D2D1_POINT_2F& point) {
			for (auto& [key, child] : m_children_) {
				if (child && IsWindow(child->d_.hWnd_) && (child->d_.menu_form_ || child->d_.tooltip_form_)) {
					// close child menu forms and tooltip forms
					child->close();
				}
			}

			point_before_ = point;

			bool pressed = false;
			bool selected = false;
			bool update_anyway = false;

			// check form widgets
			for (auto& widget : widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (widget.second.selected())
					update_anyway = true;

				if (!pressed) {
					// pressed widget not yet found
					pressed = widget.second.contains(point);
					widget.second.press(pressed);
					widget.second.select(pressed);
				}
				else {
					// pressed widget found
					widget.second.press(false);
					widget.second.select(false);
				}
			}

			class helper {
			public:
				static void check_scroll_bars(containers::page& page,
					const D2D1_POINT_2F& point, const float& dpi_scale,
					bool& scroll_bar_contains) {
					bool in_page = page.d_page_.contains(point);

					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						bool is_scroll_bar = (widget.second.type() ==
							widgets::widget_type::h_scrollbar) ||
							(widget.second.type() ==
								widgets::widget_type::v_scrollbar);

						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (in_page && is_scroll_bar && widget.second.contains(point))
							scroll_bar_contains = true;
					}

					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

							auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

							if (page_iterator != tab_pane.p_tabs_.end())
								helper::check_scroll_bars(page_iterator->second, point, dpi_scale, scroll_bar_contains);
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									helper::check_scroll_bars(page_iterator->second, point, dpi_scale, scroll_bar_contains);
							}
					}
				}

				static void check_widgets(containers::page& page,
					const D2D1_POINT_2F& point, const float& dpi_scale, bool& pressed,
					bool& update_anyway, const bool& scroll_bar_hit) {
					bool in_page = page.d_page_.contains(point);

					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						bool is_scroll_bar = (widget.second.type() ==
							widgets::widget_type::h_scrollbar) ||
							(widget.second.type() ==
								widgets::widget_type::v_scrollbar);

						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (widget.second.selected())
							update_anyway = true;

						widget.second.hide_tooltip();

						if (!pressed) {
							// pressed widget not yet found
							bool contains = widget.second.contains(point);

							if (!is_scroll_bar && scroll_bar_hit)
								contains = false;

							pressed = (in_page || is_scroll_bar) ? contains : false;

							widget.second.press(pressed);

							if (widget.second.type() !=
								widgets::widget_type::tab_pane)
								widget.second.select(pressed);
							else
								if (widget.second.type() !=
									widgets::widget_type::pane)
									widget.second.select(pressed);
						}
						else {
							// pressed widget found
							widget.second.press(false);
							widget.second.select(false);
						}
					}

					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

							auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

							if (page_iterator != tab_pane.p_tabs_.end())
								helper::check_widgets(page_iterator->second, point, dpi_scale, pressed,
									update_anyway, scroll_bar_hit);	// recursion
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									helper::check_widgets(page_iterator->second, point, dpi_scale, pressed,
										update_anyway, scroll_bar_hit);	// recursion
							}
					}

					// check scroll bars
					if (page.d_page_.h_scrollbar().pressed()) {
						page.d_page_.h_scrollbar().max_displacement(
							page.d_page_.h_scrollbar().max_displacement_left_,
							page.d_page_.h_scrollbar().max_displacement_right_);
						page.d_page_.h_scrollbar().max_displacement_left_ *= dpi_scale;
						page.d_page_.h_scrollbar().max_displacement_right_ *= dpi_scale;

						// translate the environment
						page.d_page_.h_scrollbar().x_displacement_previous_ =
							page.d_page_.h_scrollbar().x_displacement_;
						page.d_page_.h_scrollbar().max_displacement_left_ +=
							page.d_page_.h_scrollbar().x_displacement_previous_;
						page.d_page_.h_scrollbar().max_displacement_right_ +=
							page.d_page_.h_scrollbar().x_displacement_previous_;
					}
					else
						if (page.d_page_.v_scrollbar().pressed()) {
							page.d_page_.v_scrollbar().max_displacement(
								page.d_page_.v_scrollbar().max_displacement_top_,
								page.d_page_.v_scrollbar().max_displacement_bottom_);
							page.d_page_.v_scrollbar().max_displacement_top_ *= dpi_scale;
							page.d_page_.v_scrollbar().max_displacement_bottom_ *= dpi_scale;

							// translate the environment
							page.d_page_.v_scrollbar().y_displacement_previous_ =
								page.d_page_.v_scrollbar().y_displacement_;
							page.d_page_.v_scrollbar().max_displacement_top_ +=
								page.d_page_.v_scrollbar().y_displacement_previous_;
							page.d_page_.v_scrollbar().max_displacement_bottom_ +=
								page.d_page_.v_scrollbar().y_displacement_previous_;
						}
				}
			};

			// variable to ensure that once a scroll bar has been hit nothing else should
			bool scroll_bar_hit = false;

			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
				helper::check_scroll_bars(page_iterator->second, point, dpi_scale_, scroll_bar_hit);

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, point, dpi_scale_, pressed, update_anyway, scroll_bar_hit);

			if (page_iterator != p_pages_.end())
				helper::check_widgets(page_iterator->second, point, dpi_scale_, pressed, update_anyway, scroll_bar_hit);

			if (pressed || update_anyway)
				update();
		}

		void form::impl::on_lbuttonup(const D2D1_POINT_2F& point) {
			bool clicked = false;
			bool update_anyway = false;
			std::function<void()> on_click_handler = nullptr;

			// check form widgets
			for (auto& widget : widgets_) {
				if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
					continue;

				if (!clicked &&
					widget.second.pressed() &&
					widget.second.contains(point)) {
					clicked = true;
					on_click_handler = [&]() { widget.second.on_click(); };
				}

				if (widget.second.pressed())
					update_anyway = true;

				// reset pressed status
				widget.second.press(false);
			}

			class helper {
			public:
				static void check_widgets(containers::page& page,
					const D2D1_POINT_2F& point, bool& clicked, bool& update_anyway,
					std::function<void()>& on_click_handler) {
					// check widgets
					for (auto& widget : page.d_page_.widgets()) {
						if (widget.second.is_static() || !widget.second.visible() || !widget.second.enabled())
							continue;

						if (!clicked &&
							widget.second.pressed() &&
							widget.second.contains(point)) {
							clicked = true;
							on_click_handler = [&]() { widget.second.on_click(); };
						}

						if (widget.second.pressed())
							update_anyway = true;

						// reset pressed status
						widget.second.press(false);

						// failsafe: for good measure
						widget.second.hide_tooltip();

						if (widget.second.type() ==
							widgets::widget_type::tab_pane) {
							// get this tab pane
							auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

							auto page_iterator = tab_pane.p_tabs_.find(tab_pane.current_tab_);

							if (page_iterator != tab_pane.p_tabs_.end())
								check_widgets(page_iterator->second, point, clicked, update_anyway,
									on_click_handler);
						}
						else
							if (widget.second.type() ==
								widgets::widget_type::pane) {
								// get this pane
								auto& pane = page.d_page_.get_pane(widget.first);

								auto page_iterator = pane.p_panes_.find(pane.current_pane_);

								if (page_iterator != pane.p_panes_.end())
									check_widgets(page_iterator->second, point, clicked, update_anyway,
										on_click_handler);
							}
					}
				}
			};

			for (auto& it : p_status_panes_)
				helper::check_widgets(it.second, point, clicked, update_anyway,
					on_click_handler);

			auto page_iterator = p_pages_.find(current_page_);

			if (page_iterator != p_pages_.end())
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
