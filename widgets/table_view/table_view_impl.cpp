//
// table_view_impl.cpp - table_view_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "table_view_impl.h"
#include "../../containers/page/page_impl.h"
#include <algorithm>

namespace liblec {
	namespace lecui {
		widgets::table_view_impl::table_view_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_text_header(nullptr),
			_p_brush_text_header_hot(nullptr),
			_p_brush_text_selected(nullptr),
			_p_brush_fill_header(nullptr),
			_p_brush_fill_alternate(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_brush_border(nullptr),
			_p_brush_grid(nullptr),
			_p_brush_row_hot(nullptr),
			_p_brush_row_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_rectA({ 0, 0, 0, 0 }),
			_rectB({ 0, 0, 0, 0 }),
			_row_height(20.f),
			_margin(_row_height / 4.f),
			_rect_header({ 0.f, 0.f, 0.f, 0.f }),
			_last_selected(0UL),
			_book_on_selection(false) {}

		widgets::table_view_impl::~table_view_impl() { discard_resources(); }

		widgets::widget_type
			widgets::table_view_impl::type() {
			return lecui::widgets::widget_type::table_view;
		}

		HRESULT widgets::table_view_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text_header()),
					&_p_brush_text_header);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text_header_hot()),
					&_p_brush_text_header_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text_selected()),
					&_p_brush_text_selected);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill_header()),
					&_p_brush_fill_header);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill_alternate()),
					&_p_brush_fill_alternate);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_grid()),
					&_p_brush_grid);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_row_hot()),
					&_p_brush_row_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_row_selected()),
					&_p_brush_row_selected);

			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = _p_directwrite_factory->CreateTextFormat(
					convert_string(_specs.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(_specs.font_size()),
					L"", //locale
					&_p_text_format
					);
			}
			if (SUCCEEDED(hr)) {
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::table_view_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_text_header);
			safe_release(&_p_brush_text_header_hot);
			safe_release(&_p_brush_text_selected);
			safe_release(&_p_brush_fill_header);
			safe_release(&_p_brush_fill_alternate);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_grid);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::table_view_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// check if user requested a fixed numbered column
			if (_specs.fixed_number_column()) {
				// failsafe
				if (_specs.fixed_number_column_name().empty())
					_specs.fixed_number_column_name("#");

				// check if we already have it
				bool exists = false;

				for (const auto& it : _specs.columns()) {
					if (it.name == _specs.fixed_number_column_name()) {
						exists = true;
						break;
					}
				}

				if (!exists)
					_specs.columns().insert(_specs.columns().begin(), { _specs.fixed_number_column_name(), 35 });
			}

			const int precision = 3;	// to prevent false-positives (4 is enough, 3 is a failsafe)
			auto equal = [&](const D2D1_RECT_F& rect_1, const D2D1_RECT_F& rect_2) {
				if (round_off::to_float(rect_1.left, precision) == round_off::to_float(rect_2.left, precision) &&
					round_off::to_float(rect_1.top, precision) == round_off::to_float(rect_2.top, precision) &&
					round_off::to_float(rect_1.right, precision) == round_off::to_float(rect_2.right, precision) &&
					round_off::to_float(rect_1.bottom, precision) == round_off::to_float(rect_2.bottom, precision))
					return true;
				else
					return false;
			};

			// use _specs._rect not _specs.rect() and _specs._on_resize not _specs.on_resize() due to redirection to special pane
			_rect = position(_specs._rect, _specs._on_resize, change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			const auto rect_page = _page._d_page.get_rect();

			// step3: draw widget background
			{
				D2D1_ROUNDED_RECT rounded_rect{ _rect,
					_specs.corner_radius_x(), _specs.corner_radius_y() };

				if (render && _visible)
					p_render_target->FillRoundedRectangle(&rounded_rect, !_is_enabled ?
						_p_brush_disabled : _p_brush_fill);
			}

			_rectA = { _rect.left, _rect.top + _row_height, _rect.right, _rect.bottom };

			// step4: define _rectB (the table area)
			_rectB = { _rect.left, _rect.top + _row_height, _rect.right, _rect.bottom };

			// for table rows to fill up horizontally (aesthetics)
			_rectA.right = largest(_rectA.right, rect_page.right);
			_rectB.right = largest(_rectB.right, rect_page.right);

			// for table rows to fill up vertically (aesthetics)
			_rectA.bottom = largest(_rectA.bottom, rect_page.bottom);
			_rectB.bottom = largest(_rectB.bottom, rect_page.bottom);

			// step5: draw header background
			{
				_rect_header = _rect;
				_rect_header.top = rect_page.top;
				_rect_header.bottom = _rect_header.top + _row_height;
				_rect_header.right = _rectA.right;

				D2D1_ROUNDED_RECT rounded_rect{ _rect_header,
					_specs.corner_radius_x(), _specs.corner_radius_y() };

				if (render && _visible)
					p_render_target->FillRoundedRectangle(&rounded_rect, _p_brush_fill_header);
			}

			float table_width = 0.f;
			for (const auto& it : _specs.columns()) table_width += static_cast<float>(it.width);
			float table_height = _row_height * _specs.data().size();

			// adjust _rect (what will be written back) to match the table's used area
			_rect.bottom = _rectA.top + table_height;
			_rect.right = _rectA.left + table_width;

			// step9: define _rectA (the area containing all the table contents)

			// step10: draw header
			{
				auto rect_header_cell = _rect_header;
				rect_header_cell.left = _rectA.left;
				rect_header_cell.right = rect_header_cell.left;

				for (const auto& it : _specs.columns()) {
					rect_header_cell.left = rect_header_cell.right;
					rect_header_cell.right = rect_header_cell.left + static_cast<float>(it.width);

					bool hot = false;

					if (specs().user_sort() && !(specs().fixed_number_column() && it.name == _specs.fixed_number_column_name())) {
						// check if mouse is within this cell
						auto rect = rect_header_cell;

						_header_hot_spots[it.name] = rect;

						scale_RECT(rect, get_dpi_scale());

						// handle hit status
						if (_hit &&
							_point.x >= rect.left && _point.x <= rect.right &&
							_point.y >= rect.top && _point.y <= rect.bottom)
							hot = true;
						else
							hot = false;

						if (hot) {
							// show that mouse is over this header by drawing the "hot" rectangle
							D2D1_ROUNDED_RECT rounded_rect{ rect_header_cell,
								_specs.corner_radius_x(), _specs.corner_radius_y() };

							if (render && _visible)
								p_render_target->FillRoundedRectangle(&rounded_rect,
									_p_brush_hot);
						}
					}

					// draw the vertical divider
					if (render && _visible)
						p_render_target->DrawLine(D2D1::Point2F(rect_header_cell.right, rect_header_cell.top),
							D2D1::Point2F(rect_header_cell.right, rect_header_cell.bottom), _p_brush_grid, _specs.grid_line());

					auto right_limit = rect_header_cell.right;

					// check if there are sort options
					if (_header_sort_options.count(it.name)) {
						// make a reference rectangle
						auto reference = rect_header_cell;
						const auto side = reference.bottom - reference.top;

						// make it a square
						reference.left = reference.right - side;

						// make our target rectangle
						// width 40%
						// height 25%
						D2D1_RECT_F rect = { 0.f, 0.f, 8.f, 5.f };

						// position the square to the middle of the reference
						pos_rect(reference, rect, 50.f, 50.f);

						switch (_header_sort_options.at(it.name)) {
						case liblec::lecui::sort_options::ascending: {
							// draw marker pointing upwards to show ascent

							D2D1_POINT_2F bottom_left, bottom_right, top;
							bottom_left = { rect.left, rect.bottom };
							bottom_right = { rect.right, rect.bottom };
							top = { rect.left + (rect.right - rect.left) / 2.f, rect.top };

							// draw arrow
							if (render && _visible) {
								p_render_target->DrawLine(bottom_left, top, hot ? _p_brush_text_header_hot : _p_brush_text_header);
								p_render_target->DrawLine(bottom_right, top, hot ? _p_brush_text_header_hot : _p_brush_text_header);
							}

							right_limit = rect.left;
						} break;

						case liblec::lecui::sort_options::descending: {
							// draw marker pointing downwards to show descent

							D2D1_POINT_2F top_left, top_right, bottom;
							top_left = { rect.left, rect.top };
							top_right = { rect.right, rect.top };
							bottom = { rect.left + (rect.right - rect.left) / 2.f, rect.bottom };

							// draw arrow
							if (render && _visible) {
								p_render_target->DrawLine(top_left, bottom, hot ? _p_brush_text_header_hot : _p_brush_text_header);
								p_render_target->DrawLine(top_right, bottom, hot ? _p_brush_text_header_hot : _p_brush_text_header);
							}

							right_limit = rect.left;
						} break;
						case liblec::lecui::sort_options::none:
						default:
							break;
						}
					}

					auto rect_text = rect_header_cell;
					rect_text.right = right_limit;
					rect_text.left += _margin;
					rect_text.right -= _margin;

					auto hr = _p_directwrite_factory->CreateTextLayout(
						convert_string(it.name).c_str(),
						(UINT32)it.name.length(),
						_p_text_format,
						rect_text.right - rect_text.left,
						rect_text.bottom - rect_text.top,
						&_p_text_layout);

					DWRITE_TEXT_RANGE text_range = { 0, static_cast<UINT32>(it.name.length()) };

					_p_text_layout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, text_range);

					if (SUCCEEDED(hr) && render && _visible)
						p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text.left, rect_text.top },
							_p_text_layout, hot ? _p_brush_text_header_hot : _p_brush_text_header, D2D1_DRAW_TEXT_OPTIONS_CLIP);

					safe_release(&_p_text_layout);
				}
			}

			// draw the horizontal divider
			if (render && _visible)
				p_render_target->DrawLine(D2D1::Point2F(_rect_header.left, _rect_header.bottom),
					D2D1::Point2F(_rect_header.right, _rect_header.bottom), _p_brush_grid, _specs.grid_line());

			// step11: draw table
			{
				_hot_spots.clear();

				// step11a: clip to table area
				auto rect_clip = _rectB;
				rect_clip.top = _rect_header.bottom;
				auto_clip clip(render, p_render_target, rect_clip, 0.f);

				// step11b: figure out which rows are hidden from view and exclude them from the rendering
				// this gives a major performance boost when dealing with very large tables
				unsigned long hidden_above = (_rectB.top > _rectA.top) ?
					(static_cast<unsigned long>((_rectB.top - _rectA.top) / _row_height)) : 0;
				unsigned long hidden_below = (_rectA.bottom > _rectB.bottom) ?
					(static_cast<unsigned long>((_rectA.bottom - _rectB.bottom) / _row_height)) : 0;

				// step11c: do the drawing
				{
					auto rect_row = _rectA;
					rect_row.bottom = rect_row.top + _row_height * (hidden_above);

					for (unsigned long row_number = hidden_above;
						row_number < (_specs.data().size() - hidden_below); row_number++) {
						rect_row.top = rect_row.bottom;
						rect_row.bottom = rect_row.top + _row_height;

						bool selected = std::find(_specs.selected().begin(), _specs.selected().end(),
							row_number) != _specs.selected().end();
						bool hot = false;

						{
							auto rect = rect_row;
							rect.left = _rectB.left;
							rect.right = _rectB.right;

							_hot_spots[row_number] = rect;

							scale_RECT(rect, get_dpi_scale());

							// handle hit status
							if (_hit &&
								_point.x >= rect.left && _point.x <= rect.right &&
								_point.y >= rect.top && _point.y <= rect.bottom)
								hot = true;
							else
								hot = false;
						}

						if (selected) {
							auto rect = rect_row;
							rect.left = _rectB.left;
							rect.right = _rectB.right;

							D2D1_ROUNDED_RECT rounded_rect{ rect,
								_specs.corner_radius_x(), _specs.corner_radius_y() };

							if (render && _visible)
								p_render_target->FillRoundedRectangle(&rounded_rect,
									_p_brush_row_selected);
						}
						else
							if (hot) {
								auto rect = rect_row;
								rect.left = _rectB.left;
								rect.right = _rectB.right;

								D2D1_ROUNDED_RECT rounded_rect{ rect,
									_specs.corner_radius_x(), _specs.corner_radius_y() };

								if (render && _visible)
									p_render_target->FillRoundedRectangle(&rounded_rect,
										_p_brush_row_hot);
							}
							else
								if (row_number % 2 == 0) {
									// step11ci: draw alternate background
									auto rect = rect_row;
									rect.left = _rectB.left;
									rect.right = _rectB.right;

									D2D1_ROUNDED_RECT rounded_rect{ rect,
										_specs.corner_radius_x(), _specs.corner_radius_y() };

									if (render && _visible)
										p_render_target->FillRoundedRectangle(&rounded_rect,
											_p_brush_fill_alternate);
								}

						// step11cii: draw row
						auto rect_cell = rect_row;
						rect_cell.right = rect_cell.left;

						for (const auto& it : _specs.columns()) {
							rect_cell.left = rect_cell.right;
							rect_cell.right = rect_cell.left + static_cast<float>(it.width);

							auto rect_text = rect_cell;
							rect_text.left += _margin;
							rect_text.right -= _margin;

							try {
								std::string text;

								if (_specs.fixed_number_column() && it.name == _specs.fixed_number_column_name()) {
									text = std::to_string(row_number + 1);
								}
								else {
									auto value = _specs.data().at(row_number).at(it.name);
									if (value.has_value()) {
										// integer
										if (value.type() == typeid(int))
											text = std::to_string(get::integer(value));

										// float, double
										if (value.type() == typeid(float) || value.type() == typeid(double))
											text = round_off::to_string(get::real(value), it.precision);

										// const char*, string
										if (value.type() == typeid(const char*) || value.type() == typeid(std::string))
											text = get::text(value);
									}
								}

								// create a text layout
								HRESULT hr = _p_directwrite_factory->CreateTextLayout(
									convert_string(text).c_str(),
									(UINT32)text.length(),
									_p_text_format,
									rect_text.right - rect_text.left,
									rect_text.bottom - rect_text.top,
									&_p_text_layout);

								if (SUCCEEDED(hr) && render && _visible) {
									// draw the text layout
									p_render_target->DrawTextLayout(
										D2D1_POINT_2F{ rect_text.left, rect_text.top },
										_p_text_layout,
										selected ? _p_brush_text_selected : _p_brush,
										D2D1_DRAW_TEXT_OPTIONS_CLIP);
								}

								// release the text layout
								safe_release(&_p_text_layout);
							}
							catch (const std::exception&) {}
						}
					}
				}

				// step12: draw grid
				if (render && _visible) {
					// step12a: draw horizontal lines
					{
						auto rect_row = _rectA;
						rect_row.bottom = rect_row.top + _row_height * (hidden_above);

						do {
							rect_row.top = rect_row.bottom;
							rect_row.bottom = rect_row.top + _row_height;

							if (rect_row.bottom > _rectB.bottom)
								break;

							p_render_target->DrawLine(D2D1::Point2F(_rectB.left, rect_row.bottom),
								D2D1::Point2F(_rectB.right, rect_row.bottom), _p_brush_grid, _specs.grid_line());
						} while (true);
					}

					// step12b: draw vertical lines
					{
						auto x = _rectA.left;
						for (auto& it : _specs.columns()) {
							x += it.width;

							p_render_target->DrawLine(D2D1::Point2F(x, _rectB.top),
								D2D1::Point2F(x, _rectB.bottom), _p_brush_grid, _specs.grid_line());
						}
					}
				}
			}

			return _rect;
		}

		void widgets::table_view_impl::on_click() {
			if (true) {
				if (_book_on_selection) {
					if (_specs.events().selection)
						on_selection();

					_book_on_selection = false;
					return;
				}

				bool ctrl_pressed = (GetKeyState(VK_CONTROL) & 0x8000);
				bool shift_pressed = (GetKeyState(VK_SHIFT) & 0x8000);

				// check if any of the rows have been clicked
				bool selection_made = false;
				auto selected_previous = _specs.selected();
				_specs.selected().clear();

				for (auto& it : _hot_spots) {
					auto rect = it.second;
					scale_RECT(rect, get_dpi_scale());

					if (_point.x >= rect.left && _point.x <= rect.right &&
						_point.y >= rect.top && _point.y <= rect.bottom) {
						// change the selection
						selection_made = true;

						if (ctrl_pressed) {
							if (std::find(selected_previous.begin(), selected_previous.end(),
								it.first) == selected_previous.end()) {
								// add this row to the current selection, using the
								// order in which items appear in the table
								for (unsigned long row_number = 0; row_number < _specs.data().size();
									row_number++) {
									if (row_number == it.first)
										_specs.selected().push_back(row_number);
									else
										if (std::find(selected_previous.begin(), selected_previous.end(),
											row_number) != selected_previous.end())
											_specs.selected().push_back(row_number);
								}
							}
							else
							{
								// de-select this item
								for (const auto& m_it : selected_previous) {
									if (m_it == it.first)
										continue;
									_specs.selected().push_back(m_it);
								}
							}
						}
						else
							if (shift_pressed && !selected_previous.empty()) {
								// select all items beginning with the last selected item and this one
								if (it.first > _last_selected)
									for (unsigned long current_row = _last_selected;
										current_row <= it.first; current_row++)
										_specs.selected().push_back(current_row);
								else
									for (unsigned long current_row = it.first;
										current_row <= _last_selected; current_row++)
										_specs.selected().push_back(current_row);
							}
							else
								_specs.selected().push_back(it.first);

						/// Last selected item algorithm:
						/// 
						/// It's the item that's selected when when there is no selection or the item
						/// that's selected with shift is not pressed
						if (selected_previous.empty() || !shift_pressed)
							_last_selected = it.first;
					}
				}

				if (selection_made) {
					if (_specs.events().selection)
						on_selection();

					if (_specs.events().click)
						_specs.events().click();

					if (_specs.events().action)
						_specs.events().action();
				}

				for (auto& [name, rectangle] : _header_hot_spots) {
					if (_specs.fixed_number_column() && name == _specs.fixed_number_column_name())
						continue;	// failsafe ... shouldn't be in _header_hot_spots anyway

					auto rect = rectangle;
					scale_RECT(rect, get_dpi_scale());

					if (_point.x >= rect.left && _point.x <= rect.right &&
						_point.y >= rect.top && _point.y <= rect.bottom) {
						// column has been clicked for sorting
						try {
							if (_header_sort_options.count(name)) {
								// sort option entry already exists ... flip it
								auto& sort_option = _header_sort_options.at(name);

								switch (sort_option) {
								case sort_options::ascending:
									sort_option = sort_options::descending;
									break;
								case sort_options::descending:
								case sort_options::none:
								default:
									sort_option = sort_options::ascending;
									break;
								}
							}
							else
								_header_sort_options.insert(std::make_pair(name, sort_options::ascending));

							// do the sorting
							switch (_header_sort_options.at(name)) {
							case sort_options::ascending: {
								log(name + ": sort ascending");

								const std::string column_name(name);

								auto compare_by_column_for_ascending = [&](const lecui::table_row& a, const lecui::table_row& b)->bool {
									const auto& a_value = a.at(column_name);
									const auto& b_value = b.at(column_name);

									if (!a_value.has_value() || !b_value.has_value())
										return true;

									if (a_value.type() != b_value.type())
										return true;

									if (a_value.type() == typeid(int))
										return get::integer(a_value) < get::integer(b_value);

									if (a_value.type() == typeid(float) || a_value.type() == typeid(double))
										return get::real(a_value) < get::real(b_value);

									if (a_value.type() == typeid(std::string) || a_value.type() == typeid(const char*))
										return get::text(a_value) < get::text(b_value);

									return true;
								};

								// sort
								std::sort(_specs.data().begin(), _specs.data().end(), compare_by_column_for_ascending);
							} break;

							case sort_options::descending: {
								log(name + ": sort descending");

								const std::string column_name(name);

								auto compare_by_column_for_descending = [&](const lecui::table_row& a, const lecui::table_row& b)->bool {
									const auto& a_value = a.at(column_name);
									const auto& b_value = b.at(column_name);

									if (!a_value.has_value() || !b_value.has_value())
										return false;

									if (a_value.type() != b_value.type())
										return false;

									if (a_value.type() == typeid(int))
										return get::integer(a_value) > get::integer(b_value);

									if (a_value.type() == typeid(float) || a_value.type() == typeid(double))
										return get::real(a_value) > get::real(b_value);

									if (a_value.type() == typeid(std::string) || a_value.type() == typeid(const char*))
										return get::text(a_value) > get::text(b_value);

									return false;
								};

								// sort
								std::sort(_specs.data().begin(), _specs.data().end(), compare_by_column_for_descending);
							} break;

							case sort_options::none:
							default:
								log(name + ": no sorting");
								break;
							}

							// reset the other sort options
							for (auto& it : _header_sort_options) {
								if (it.first != name)
									it.second = sort_options::none;
							}
						}
						catch (const std::exception& e) { log(e.what()); }
					}
				}
			}
		}

		bool widgets::table_view_impl::on_keydown(WPARAM wParam) {
			float adjustment = 0.f;

			switch (wParam) {
			case VK_UP:
				adjustment = _row_height;
				break;
			case VK_DOWN:
				adjustment = -_row_height;
				break;
			default:
				break;
			}

			if (adjustment != 0.f) {
				if (_specs.selected().empty()) {
					// simple scrolling
				}
				else {
					// move last selection one unit (unless it's at the beginning or the end)
					long new_selection = smallest(static_cast<long>(_specs.data().size() - 1),
						largest(0L, static_cast<long>(_last_selected) -
							static_cast<long>(adjustment / _row_height)));

					_specs.selected().clear();
					_specs.selected().push_back(new_selection);
					_last_selected = new_selection;

					// check out if new selection is within table area
					D2D1_RECT_F rect_selected = _rectA;

					rect_selected.top += (_row_height * new_selection);
					rect_selected.bottom = rect_selected.top + _row_height;

					float diff = 0.f;
					if (rect_selected.top < _rectB.top)
						diff = _rectB.top - rect_selected.top;	// above table area
					else
						if (rect_selected.bottom > _rectB.bottom)
							diff = _rectB.bottom - rect_selected.bottom;	// below table area

					if (diff != 0.f) {
						// move the displacement to ensure visibility of new selection
					}

					_book_on_selection = true;
				}
			}

			if (adjustment != 0.f)
				return true;
			else
				return false;
		}

		bool widgets::table_view_impl::hit(const bool& hit) {
			if (_is_static || hit == _hit) {
				if (hit || _pressed)
					return true;
				else
					return false;
			}

			_hit = hit;
			return true;
		}

		void widgets::table_view_impl::on_right_click() {
			if (_specs.events().context_menu) {
				std::vector<table_row> var;
				for (const auto& it : _specs.selected()) {
					try { var.push_back(_specs.data().at(it)); }
					catch (const std::exception&) {}
				}

				_specs.events().context_menu(var);
			}

			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		widgets::table_view_specs&
			widgets::table_view_impl::specs() { return _specs; }

		widgets::table_view_specs&
			widgets::table_view_impl::operator()() { return specs(); }

		void widgets::table_view_impl::on_selection() {
			if (_specs.events().selection) {
				std::vector<table_row> var;
				for (const auto& it : _specs.selected()) {
					try { var.push_back(_specs.data().at(it)); }
					catch (const std::exception&) {}
				}

				_specs.events().selection(var);
			}
		}
	}
}
