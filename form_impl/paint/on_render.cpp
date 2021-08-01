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
				_p_render_target->BeginDraw();

				_p_render_target->SetTransform(D2D1::Matrix3x2F::Identity());

				// fill form background
				_p_render_target->Clear(convert_color(_clr_background));

				// get render target size
				const D2D1_SIZE_F rtSize = _p_render_target->GetSize();

				// fill titlebar background
				const D2D1_RECT_F _rect_titlebar = { 0.f, 0.f, rtSize.width, _caption_bar_height };
				_p_render_target->FillRectangle(&_rect_titlebar, _p_brush_titlebar);

#if defined(_DEBUG) and DESIGNLINES
				// Draw a grid background
				int width = static_cast<int>(rtSize.width);
				int height = static_cast<int>(rtSize.height);

				const float line_width = .05f;

				for (int x = 0; x < width; x += 10)
					_p_render_target->DrawLine(D2D1::Point2F(static_cast<FLOAT>(x), .0f),
						D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
						_p_brush_theme, line_width);

				for (int y = 0; y < height; y += 10)
					_p_render_target->DrawLine(D2D1::Point2F(.0f, static_cast<FLOAT>(y)),
						D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
						_p_brush_theme, line_width);
#endif

				class helper {
				public:
					static void render_page(bool allow_render,
						const std::string& page_alias,
						const std::string& current_page,
						containers::page& page,
						ID2D1HwndRenderTarget* _p_render_target,
						const D2D1_RECT_F& rectB,
						const D2D1_RECT_F& client_area,
						const D2D1_SIZE_F& change_in_size,
						const float& _dpi_scale,
						ID2D1SolidColorBrush* _p_brush_theme,
						ID2D1SolidColorBrush* _p_brush_theme_hot,
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
							auto_clip clip(render, _p_render_target, client_area, 1.f);

							do {
								// h_scrollbar
								{
									// impose limits
									if (page._d_page.h_scrollbar()._x_displacement < 0.f)
										page._d_page.h_scrollbar()._x_displacement =
										largest(page._d_page.h_scrollbar()._x_displacement,
											page._d_page.h_scrollbar()._max_displacement_left);
									else
										page._d_page.h_scrollbar()._x_displacement =
										smallest(page._d_page.h_scrollbar()._x_displacement,
											page._d_page.h_scrollbar()._max_displacement_right);

									// translate the displacement
									float _x_displacement_translated = 0.f;
									if (page._d_page.h_scrollbar().translate_x_displacement(
										page._d_page.h_scrollbar()._x_displacement,
										_x_displacement_translated,
										page._d_page.h_scrollbar()._force_translate)) {
										page._d_page.h_scrollbar()._force_translate = false;
										page._d_page.h_scrollbar()._x_off_set =
											_x_displacement_translated;
									}
								}

								// v_scrollbar
								{
									// impose limits
									if (page._d_page.v_scrollbar()._y_displacement < 0.f)
										page._d_page.v_scrollbar()._y_displacement =
										largest(page._d_page.v_scrollbar()._y_displacement,
											page._d_page.v_scrollbar()._max_displacement_top);
									else
										page._d_page.v_scrollbar()._y_displacement =
										smallest(page._d_page.v_scrollbar()._y_displacement,
											page._d_page.v_scrollbar()._max_displacement_bottom);

									// translate the displacement
									float _y_displacement_translated = 0.f;
									if (page._d_page.v_scrollbar().translate_y_displacement(
										page._d_page.v_scrollbar()._y_displacement,
										_y_displacement_translated,
										page._d_page.v_scrollbar()._force_translate)) {
										page._d_page.v_scrollbar()._force_translate = false;
										page._d_page.v_scrollbar()._y_off_set =
											_y_displacement_translated;
									}
								}

								// figure out furthest left and right

								// measure widgets
								bool initialized = false;
								D2D1_RECT_F _rect_widgets = { 0.f, 0.f, 0.f, 0.f };
								for (auto& widget : page._d_page.widgets()) {
									if (widget.second.type() ==
										widgets::widget_type::h_scrollbar ||
										widget.second.type() ==
										widgets::widget_type::v_scrollbar ||
										widget.second.type() ==
										widgets::widget_type::group)
										continue;

									_rect_widgets = widget.second.render(_p_render_target,
										change_in_size,
										{ (page._d_page.h_scrollbar()._x_off_set / _dpi_scale) - client_area.left,
										(page._d_page.v_scrollbar()._y_off_set / _dpi_scale) - client_area.top },
										false);

									if (!initialized) {
										initialized = true;
										rectA = _rect_widgets;
									}
									else {
										rectA.left = smallest(rectA.left, _rect_widgets.left);
										rectA.right = largest(rectA.right, _rect_widgets.right);
										rectA.top = smallest(rectA.top, _rect_widgets.top);
										rectA.bottom = largest(rectA.bottom, _rect_widgets.bottom);
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
										page._d_page.h_scrollbar()._x_displacement += x_overflow;
										page._d_page.h_scrollbar()._force_translate = true;
										correct = true;
									}

									// don't let widgets be needlessly hidden to the top while
									// there's room at the bottom!
									if ((rectA.top < rectB.top) && (rectA.bottom < rectB.bottom)) {
										const auto top = rectA.top - rectB.top;
										const auto bottom = rectA.bottom - rectB.bottom;
										const auto y_overflow = abs(top) < abs(bottom) ? top : bottom;

										// translate the environment
										page._d_page.v_scrollbar()._y_displacement += y_overflow;
										page._d_page.v_scrollbar()._force_translate = true;
										correct = true;
									}

									if (correct)
										continue;
								}

								break;
							} while (true);

							// resize groupboxes
							for (auto& widget : page._d_page.widgets()) {
								if (widget.second.type() !=
									widgets::widget_type::group)
									continue;

								try {
									// get the groupbox widget
									auto& specs = page._d_page.get_group_impl(widget.first).specs();

									bool groupbox_initialized = false;
									for (auto& widget_alias :
										page._d_page.get_group_impl(widget.first).specs().widgets()) {
										try {
											// get the rect for this widget
											const auto& _rect =
												page._d_page.widgets().at(widget_alias).get_rect();

											// adjust the groupbox rect
											if (!groupbox_initialized) {
												specs.rect(convert_rect(_rect));
												groupbox_initialized = true;
											}
											else {
												specs.rect()
													.left(smallest(specs.rect().left(), _rect.left))
													.top(smallest(specs.rect().top(), _rect.top))
													.right(largest(specs.rect().right(), _rect.right))
													.bottom(largest(specs.rect().bottom(), _rect.bottom));
											}
										}
										catch (const std::exception&) {}
									}

									// apply the margin
									specs.rect().left() -= specs.margin();
									specs.rect().top() -= specs.margin();
									specs.rect().right() += specs.margin();
									specs.rect().bottom() += specs.margin();

									// get bounding rect for all controls within this groupbox
								}
								catch (const std::exception&) {
									// to-do: make sure rect is within bounding rects for page
								}

							}

							// render groupboxes
							for (auto& widget : page._d_page.widgets()) {
								if (widget.second.type() !=
									widgets::widget_type::group)
									continue;

								try {
									// check if groupbox has widgets
									if (page._d_page.get_group_impl(widget.first).specs().widgets().empty())
										continue;

									// to-do: check if widgets actually exist and discontinue if they dont
								}
								catch (const std::exception&) {
									continue;
								}

								// render with no resizing or offset parameters because the rect for the
								// group is already properly set
								widget.second.render(_p_render_target,
									{ 0.f, 0.f }, { 0.f, 0.f }, render);
							}

							// render widgets
							for (auto& widget : page._d_page.widgets()) {
								if (widget.second.type() ==
									widgets::widget_type::h_scrollbar ||
									widget.second.type() ==
									widgets::widget_type::v_scrollbar ||
									widget.second.type() ==
									widgets::widget_type::group)
									continue;

								widget.second.render(_p_render_target,
									change_in_size,
									{ (page._d_page.h_scrollbar()._x_off_set / _dpi_scale) - client_area.left,
									(page._d_page.v_scrollbar()._y_off_set / _dpi_scale) - client_area.top },
									render);

								if (widget.second.type() ==
									widgets::widget_type::tab_pane) {
									try {
										// get this tab pane
										auto& tab_pane = page._d_page.get_tab_pane_impl(widget.first);

										// get client area for this tab pane
										const auto& client_area = tab_pane.client_area();

										const D2D1_SIZE_F change_in_size = {
											(tab_pane.tab_pane_area().right - tab_pane.tab_pane_area().left) - tab_pane().rect().width(),
											(tab_pane.tab_pane_area().bottom - tab_pane.tab_pane_area().top) - tab_pane().rect().height()
										};

										for (auto& tab : tab_pane._p_tabs) {
											const float _content_margin = tab_pane.content_margin();
											D2D1_RECT_F rect_page = client_area;
											rect_page.left += _content_margin;
											rect_page.top += _content_margin;
											rect_page.right -= _content_margin;
											rect_page.bottom -= _content_margin;

											render_page(render ? tab_pane.visible() : false, tab.first, tab_pane.specs().selected(), tab.second,
												_p_render_target, rect_page, rect_page, change_in_size,
												_dpi_scale, _p_brush_theme, _p_brush_theme_hot, lbutton_pressed);	// recursion
										}
									}
									catch (const std::exception&) {}
								}
								else
									if (widget.second.type() ==
										widgets::widget_type::pane) {
										try {
											// get this pane
											auto& pane = page._d_page.get_pane_impl(widget.first);

											// get client area for this pane
											const auto& client_area = pane.client_area();

											const D2D1_SIZE_F change_in_size = {
												(pane.pane_area().right - pane.pane_area().left) - pane().rect().width(),
												(pane.pane_area().bottom - pane.pane_area().top) - pane().rect().height()
											};

											for (auto& page : pane._p_panes) {
												const float _content_margin = pane.content_margin();
												D2D1_RECT_F rect_page = client_area;
												rect_page.left += _content_margin;
												rect_page.top += _content_margin;
												rect_page.right -= _content_margin;
												rect_page.bottom -= _content_margin;

												render_page(render ? pane.visible() : false, page.first, pane._current_pane, page.second,
													_p_render_target, rect_page, rect_page, change_in_size,
													_dpi_scale, _p_brush_theme, _p_brush_theme_hot, lbutton_pressed);	// recursion
											}
										}
										catch (const std::exception&) {}
									}
							}

#if defined(_DEBUG) and DESIGNLINES
							if (render) {
								// draw rectA and rectB
								_p_render_target->DrawRectangle(&rectA, _p_brush_theme, .5f);
								_p_render_target->DrawRectangle(&rectB, _p_brush_theme_hot, .5f);
							}
#endif
						}

						// setup horizontal scroll bar and render it
						page._d_page.h_scrollbar().setup(rectA, rectB);
						page._d_page.h_scrollbar().render(_p_render_target,
							change_in_size, { 0.f - client_area.left,
							0.f - client_area.top }, render);

						// setup vertical scroll bar and render it
						page._d_page.v_scrollbar().setup(rectA, rectB);
						page._d_page.v_scrollbar().render(_p_render_target,
							change_in_size, { 0.f - client_area.left,
							0.f - client_area.top }, render);
					}
				};

				// get status pane sizes
				const auto status_bottom = get_status_size(containers::status_pane_specs::pane_location::bottom);
				const auto status_top = get_status_size(containers::status_pane_specs::pane_location::top);
				const auto status_left = get_status_size(containers::status_pane_specs::pane_location::left);
				const auto status_right = get_status_size(containers::status_pane_specs::pane_location::right);

				const D2D1_SIZE_F change_in_size = { rtSize.width - _size.get_width(), rtSize.height - _size.get_height() };

				// render page
				for (auto& page : _p_pages) {
					const D2D1_RECT_F rect_page = { _content_margin + status_left.get_width(),
						_caption_bar_height + _content_margin + status_top.get_height(),
						rtSize.width - _content_margin - status_right.get_width(), rtSize.height - _content_margin - status_bottom.get_height() };

					const D2D1_RECT_F client_area = rect_page;

					helper::render_page(true, page.first, _current_page, page.second, _p_render_target,
						rect_page, client_area, change_in_size, _dpi_scale,
						_p_brush_theme, _p_brush_theme_hot, _lbutton_pressed);
				}

				// render status panes
				for (auto& page : _p_status_panes) {
					if (page.first == "status::bottom") {
						const auto left = _content_margin;
						const auto bottom = rtSize.height - _content_margin;
						const auto right = rtSize.width - _content_margin;
						const auto top = bottom - page.second.size().get_height();

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, _p_render_target,
							rect_page, client_area, change_in_size, _dpi_scale,
							_p_brush_theme, _p_brush_theme_hot, _lbutton_pressed);
					}

					if (page.first == "status::top") {
						const auto left = _content_margin;
						const auto top = _caption_bar_height + _content_margin;
						const auto bottom = top + page.second.size().get_height();
						const auto right = rtSize.width - _content_margin;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, _p_render_target,
							rect_page, client_area, change_in_size, _dpi_scale,
							_p_brush_theme, _p_brush_theme_hot, _lbutton_pressed);
					}

					if (page.first == "status::left") {
						const auto left = _content_margin;
						const auto top = _caption_bar_height + _content_margin;
						const auto bottom = rtSize.height - _content_margin;
						const auto right = left + page.second.size().get_width();

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, _p_render_target,
							rect_page, client_area, change_in_size, _dpi_scale,
							_p_brush_theme, _p_brush_theme_hot, _lbutton_pressed);
					}

					if (page.first == "status::right") {
						const auto right = rtSize.width - _content_margin;
						const auto left = right - page.second.size().get_width();
						const auto top = _caption_bar_height + _content_margin;
						const auto bottom = rtSize.height - _content_margin;

						const D2D1_RECT_F rect_page = { left, top, right, bottom };
						const D2D1_RECT_F client_area = rect_page;

						helper::render_page(true, page.first, page.first, page.second, _p_render_target,
							rect_page, client_area, change_in_size, _dpi_scale,
							_p_brush_theme, _p_brush_theme_hot, _lbutton_pressed);
					}
				}

				// render form widgets
				for (auto& widget : _widgets)
					widget.second.render(_p_render_target,
						change_in_size, { 0.f, 0.f }, true);

				// render form border
				if (!maximized(_hWnd)) {
					const D2D1_RECT_F form_rectangle =
						D2D1::RectF(.0f, .0f, rtSize.width, rtSize.height);
					_p_render_target->DrawRectangle(&form_rectangle,
						_p_brush_theme, _form_border_thickness);
				}

				hr = _p_render_target->EndDraw();
			}

			if (hr == D2DERR_RECREATE_TARGET) {
				hr = S_OK;
				discard_device_resources();
			}

			return hr;
		}
	}
}
