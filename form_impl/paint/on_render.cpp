//
// on_render.cpp - render implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../form_impl.h"

#define DESIGNLINES	0	// set to 1 to show design lines, 0 otherwise

namespace liblec {
	namespace lecui {
		/// This method discards device-specific resources if the Direct3D device dissapears during
		/// execution and recreates the resources the next time it's invoked
		HRESULT form::impl::on_render() {
			HRESULT hr = S_OK;

			hr = create_device_resources();

			if (SUCCEEDED(hr)) {
				p_render_target_->BeginDraw();

				p_render_target_->SetTransform(D2D1::Matrix3x2F::Identity());

				// fill form background
				p_render_target_->Clear(convert_color(clr_background_));

				// get render target size
				const D2D1_SIZE_F rtSize = p_render_target_->GetSize();

				// fill titlebar background
				const D2D1_RECT_F rect_titlebar_ = { 0.f, 0.f, rtSize.width, caption_bar_height_ };
				p_render_target_->FillRectangle(&rect_titlebar_, p_brush_titlebar_);

#if defined(_DEBUG) and DESIGNLINES
				// Draw a grid background
				int width = static_cast<int>(rtSize.width);
				int height = static_cast<int>(rtSize.height);

				const float line_width = .05f;

				for (int x = 0; x < width; x += 10)
					p_render_target_->DrawLine(D2D1::Point2F(static_cast<FLOAT>(x), .0f),
						D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
						p_brush_theme_, line_width);

				for (int y = 0; y < height; y += 10)
					p_render_target_->DrawLine(D2D1::Point2F(.0f, static_cast<FLOAT>(y)),
						D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
						p_brush_theme_, line_width);
#endif

				class helper {
				public:
					static void render_page(bool allow_render,
						const std::string& page_alias,
						const std::string& current_page,
						containers::page& page,
						ID2D1HwndRenderTarget* p_render_target_,
						const D2D1_RECT_F& rectB,
						const D2D1_RECT_F& client_area,
						const D2D1_SIZE_F& change_in_size,
						const float& dpi_scale_,
						ID2D1SolidColorBrush* p_brush_theme_,
						ID2D1SolidColorBrush* p_brush_theme_hot_,
						bool lbutton_pressed) {
						bool render = page_alias == current_page;

						if (!allow_render)
							render = false;

						// define horizontal scroll parameters. rectA defines the area that contains
						// the widgets, while rectB defines the area in the main form outside of which
						// scrolling should kick in
						D2D1_RECT_F rectA = { 0.f, 0.f, 0.f, 0.f };

						{
							// clip
							auto_clip clip(render, p_render_target_, client_area, 1.f);

							do {
								// h_scrollbar
								{
									// impose limits
									if (page.d_page_.h_scrollbar().x_displacement_ < 0.f)
										page.d_page_.h_scrollbar().x_displacement_ =
										largest(page.d_page_.h_scrollbar().x_displacement_,
											page.d_page_.h_scrollbar().max_displacement_left_);
									else
										page.d_page_.h_scrollbar().x_displacement_ =
										smallest(page.d_page_.h_scrollbar().x_displacement_,
											page.d_page_.h_scrollbar().max_displacement_right_);

									// translate the displacement
									float x_displacement_translated_ = 0.f;
									if (page.d_page_.h_scrollbar().translate_x_displacement(
										page.d_page_.h_scrollbar().x_displacement_,
										x_displacement_translated_,
										page.d_page_.h_scrollbar().force_translate_)) {
										page.d_page_.h_scrollbar().force_translate_ = false;
										page.d_page_.h_scrollbar().x_off_set_ =
											x_displacement_translated_;
									}
								}

								// v_scrollbar
								{
									// impose limits
									if (page.d_page_.v_scrollbar().y_displacement_ < 0.f)
										page.d_page_.v_scrollbar().y_displacement_ =
										largest(page.d_page_.v_scrollbar().y_displacement_,
											page.d_page_.v_scrollbar().max_displacement_top_);
									else
										page.d_page_.v_scrollbar().y_displacement_ =
										smallest(page.d_page_.v_scrollbar().y_displacement_,
											page.d_page_.v_scrollbar().max_displacement_bottom_);

									// translate the displacement
									float y_displacement_translated_ = 0.f;
									if (page.d_page_.v_scrollbar().translate_y_displacement(
										page.d_page_.v_scrollbar().y_displacement_,
										y_displacement_translated_,
										page.d_page_.v_scrollbar().force_translate_)) {
										page.d_page_.v_scrollbar().force_translate_ = false;
										page.d_page_.v_scrollbar().y_off_set_ =
											y_displacement_translated_;
									}
								}

								// figure out furthest left and right

								// measure widgets
								bool initialized = false;
								D2D1_RECT_F rect_widgets_ = { 0.f, 0.f, 0.f, 0.f };
								for (auto& widget : page.d_page_.widgets()) {
									if (widget.second.type() ==
										widgets::widget_type::h_scrollbar ||
										widget.second.type() ==
										widgets::widget_type::v_scrollbar ||
										widget.second.type() ==
										widgets::widget_type::group)
										continue;

									rect_widgets_ = widget.second.render(p_render_target_,
										change_in_size,
										{ (page.d_page_.h_scrollbar().x_off_set_ / dpi_scale_) - client_area.left,
										(page.d_page_.v_scrollbar().y_off_set_ / dpi_scale_) - client_area.top },
										false);

									if (!initialized) {
										initialized = true;
										rectA = rect_widgets_;
									}
									else {
										rectA.left = smallest(rectA.left, rect_widgets_.left);
										rectA.right = largest(rectA.right, rect_widgets_.right);
										rectA.top = smallest(rectA.top, rect_widgets_.top);
										rectA.bottom = largest(rectA.bottom, rect_widgets_.bottom);
									}
								}

								// only attempt to correct needlessly hidden widgets if the left
								// mouse button is not pressed
								if (!lbutton_pressed) {
									bool correct = false;

									// don't let widgets be needlessly hidden to the left while
									// there's room on the right!
									if ((rectA.left < rectB.left) && (rectA.right < rectB.right)) {
										const auto left = rectA.left - rectB.left;
										const auto right = rectA.right - rectB.right;
										const auto x_overflow = abs(left) < abs(right) ? left : right;

										// translate the environment
										page.d_page_.h_scrollbar().x_displacement_ += x_overflow;
										page.d_page_.h_scrollbar().force_translate_ = true;
										correct = true;
									}

									// don't let widgets be needlessly hidden to the top while
									// there's room at the bottom!
									if ((rectA.top < rectB.top) && (rectA.bottom < rectB.bottom)) {
										const auto top = rectA.top - rectB.top;
										const auto bottom = rectA.bottom - rectB.bottom;
										const auto y_overflow = abs(top) < abs(bottom) ? top : bottom;

										// translate the environment
										page.d_page_.v_scrollbar().y_displacement_ += y_overflow;
										page.d_page_.v_scrollbar().force_translate_ = true;
										correct = true;
									}

									if (correct)
										continue;
								}

								break;
							} while (true);

							// resize groupboxes
							for (auto& widget : page.d_page_.widgets()) {
								if (widget.second.type() !=
									widgets::widget_type::group)
									continue;

								try {
									// get the groupbox specs
									auto& specs = page.d_page_.get_group(widget.first).specs();

									bool groupbox_initialized = false;
									for (auto& widget_alias :
										page.d_page_.get_group(widget.first).specs().widgets) {
										try {
											// get the rect for this widget
											const auto& rect_ =
												page.d_page_.widgets().at(widget_alias).get_rect();

											// adjust the groupbox rect
											if (!groupbox_initialized) {
												specs.rect = convert_rect(rect_);
												groupbox_initialized = true;
											}
											else {
												specs.rect.left = smallest(specs.rect.left,
													rect_.left);
												specs.rect.top = smallest(specs.rect.top,
													rect_.top);
												specs.rect.right = largest(specs.rect.right,
													rect_.right);
												specs.rect.bottom = largest(specs.rect.bottom,
													rect_.bottom);
											}
										}
										catch (const std::exception&) {}
									}

									// apply the margin
									specs.rect.left -= specs.margin;
									specs.rect.top -= specs.margin;
									specs.rect.right += specs.margin;
									specs.rect.bottom += specs.margin;

									// get bounding rect for all controls within this groupbox
								}
								catch (const std::exception&) {
									// to-do: make sure rect is within bounding rects for page
								}

							}

							// render groupboxes
							for (auto& widget : page.d_page_.widgets()) {
								if (widget.second.type() !=
									widgets::widget_type::group)
									continue;

								try {
									// check if groupbox has widgets
									if (page.d_page_.get_group(widget.first).specs().widgets.empty())
										continue;

									// to-do: check if widgets actually exist and discontinue if they dont
								}
								catch (const std::exception&) {
									continue;
								}

								// render with no resizing or offset parameters because the rect for the
								// group is already properly set
								widget.second.render(p_render_target_,
									{ 0.f, 0.f }, { 0.f, 0.f }, render);
							}

							// render widgets
							for (auto& widget : page.d_page_.widgets()) {
								if (widget.second.type() ==
									widgets::widget_type::h_scrollbar ||
									widget.second.type() ==
									widgets::widget_type::v_scrollbar ||
									widget.second.type() ==
									widgets::widget_type::group)
									continue;

								widget.second.render(p_render_target_,
									change_in_size,
									{ (page.d_page_.h_scrollbar().x_off_set_ / dpi_scale_) - client_area.left,
									(page.d_page_.v_scrollbar().y_off_set_ / dpi_scale_) - client_area.top },
									render);

								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									try {
										// get this tab pane
										auto& tab_pane = page.d_page_.get_tab_pane(widget.first);

										// get client area for this tab pane
										const auto& client_area = tab_pane.client_area();

										const D2D1_SIZE_F change_in_size =
										{ (tab_pane.tab_pane_area().right - tab_pane.tab_pane_area().left) -
											(tab_pane().rect.right - tab_pane().rect.left),
											(tab_pane.tab_pane_area().bottom - tab_pane.tab_pane_area().top) -
											(tab_pane().rect.bottom - tab_pane().rect.top) };

										for (auto& tab : tab_pane.p_tabs_) {
											const float page_tolerance_ = 10.f;
											D2D1_RECT_F rect_page = client_area;
											rect_page.left += page_tolerance_;
											rect_page.top += page_tolerance_;
											rect_page.right -= page_tolerance_;
											rect_page.bottom -= page_tolerance_;

											render_page(render, tab.first, tab_pane.current_tab_, tab.second,
												p_render_target_, rect_page, rect_page, change_in_size,
												dpi_scale_, p_brush_theme_, p_brush_theme_hot_, lbutton_pressed);	// recursion
										}
									}
									catch (const std::exception&) {}
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										try {
											// get this pane
											auto& pane = page.d_page_.get_pane(widget.first);

											// get client area for this pane
											const auto& client_area = pane.client_area();

											const D2D1_SIZE_F change_in_size =
											{ (pane.pane_area().right - pane.pane_area().left) -
												(pane().rect.right - pane().rect.left),
												(pane.pane_area().bottom - pane.pane_area().top) -
												(pane().rect.bottom - pane().rect.top) };

											for (auto& page : pane.p_panes_) {
												const float page_tolerance_ = 10.f;
												D2D1_RECT_F rect_page = client_area;
												rect_page.left += page_tolerance_;
												rect_page.top += page_tolerance_;
												rect_page.right -= page_tolerance_;
												rect_page.bottom -= page_tolerance_;

												render_page(render, page.first, pane.current_pane_, page.second,
													p_render_target_, rect_page, rect_page, change_in_size,
													dpi_scale_, p_brush_theme_, p_brush_theme_hot_, lbutton_pressed);	// recursion
											}
										}
										catch (const std::exception&) {}
									}
							}

#if defined(_DEBUG) and DESIGNLINES
							if (render) {
								// draw rectA and rectB
								p_render_target_->DrawRectangle(&rectA, p_brush_theme_, .5f);
								p_render_target_->DrawRectangle(&rectB, p_brush_theme_hot_, .5f);
							}
#endif
						}

						// setup horizontal scroll bar and render it
						page.d_page_.h_scrollbar().setup(rectA, rectB);
						page.d_page_.h_scrollbar().render(p_render_target_,
							change_in_size, { 0.f - client_area.left,
							0.f - client_area.top }, render);

						// setup vertical scroll bar and render it
						page.d_page_.v_scrollbar().setup(rectA, rectB);
						page.d_page_.v_scrollbar().render(p_render_target_,
							change_in_size, { 0.f - client_area.left,
							0.f - client_area.top }, render);
					}
				};

				// get status pane sizes
				const auto status_bottom = get_status_size(containers::status_pane::location::bottom);
				const auto status_top = get_status_size(containers::status_pane::location::top);
				const auto status_left = get_status_size(containers::status_pane::location::left);
				const auto status_right = get_status_size(containers::status_pane::location::right);

				const D2D1_SIZE_F change_in_size = { rtSize.width - size_.width, rtSize.height - size_.height };

				// render page
				for (auto& page : p_pages_) {
					const D2D1_RECT_F rect_page = { page_tolerance_ + status_left.width,
						caption_bar_height_ + page_tolerance_ + status_top.height,
						rtSize.width - page_tolerance_ - status_right.width, rtSize.height - page_tolerance_ - status_bottom.height };

					const D2D1_RECT_F client_area = rect_page;

					helper::render_page(true, page.first, current_page_, page.second, p_render_target_,
						rect_page, client_area, change_in_size, dpi_scale_,
						p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
				}

				// render status panes
				for (auto& page : p_status_panes_) {
					if (page.first == "status::bottom") {
						const auto left = page_tolerance_;
						const auto bottom = rtSize.height - page_tolerance_;
						const auto right = rtSize.width - page_tolerance_;
						const auto top = bottom - page.second.size().height;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, p_render_target_,
							rect_page, client_area, change_in_size, dpi_scale_,
							p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
					}

					if (page.first == "status::top") {
						const auto left = page_tolerance_;
						const auto top = caption_bar_height_ + page_tolerance_;
						const auto bottom = top + page.second.size().height;
						const auto right = rtSize.width - page_tolerance_;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, p_render_target_,
							rect_page, client_area, change_in_size, dpi_scale_,
							p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
					}

					if (page.first == "status::left") {
						const auto left = page_tolerance_;
						const auto top = caption_bar_height_ + page_tolerance_;
						const auto bottom = rtSize.height - page_tolerance_;
						const auto right = left + page.second.size().width;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, p_render_target_,
							rect_page, client_area, change_in_size, dpi_scale_,
							p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
					}

					if (page.first == "status::right") {
						const auto right = rtSize.width - page_tolerance_;
						const auto left = right - page.second.size().width;
						const auto top = caption_bar_height_ + page_tolerance_;
						const auto bottom = rtSize.height - page_tolerance_;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, p_render_target_,
							rect_page, client_area, change_in_size, dpi_scale_,
							p_brush_theme_, p_brush_theme_hot_, lbutton_pressed_);
					}
				}

				// render form widgets
				for (auto& widget : widgets_)
					widget.second.render(p_render_target_,
						change_in_size, { 0.f, 0.f }, true);

				// render form border
				if (!maximized(hWnd_)) {
					const D2D1_RECT_F form_rectangle =
						D2D1::RectF(.0f, .0f, rtSize.width, rtSize.height);
					p_render_target_->DrawRectangle(&form_rectangle,
						p_brush_theme_, form_border_thickness_);
				}

				hr = p_render_target_->EndDraw();
			}

			if (hr == D2DERR_RECREATE_TARGET) {
				hr = S_OK;
				discard_device_resources();
			}

			return hr;
		}
	}
}
