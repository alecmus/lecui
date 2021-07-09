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

namespace liblec {
	namespace lecui {
		widgets::table_view_impl::table_view_impl(containers::page& page,
			const std::string& alias,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			p_brush_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_text_header_(nullptr),
			p_brush_text_header_hot_(nullptr),
			p_brush_text_selected_(nullptr),
			p_brush_fill_header_(nullptr),
			p_brush_fill_alternate_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_brush_border_(nullptr),
			p_brush_grid_(nullptr),
			p_brush_row_hot_(nullptr),
			p_brush_row_selected_(nullptr),
			p_text_format_(nullptr),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			rectA_({ 0, 0, 0, 0 }),
			rectB_({ 0, 0, 0, 0 }),
			row_height_(20.f),
			margin_(row_height_ / 4.f),
			rect_header_({ 0.f, 0.f, 0.f, 0.f }),
			last_selected_(0UL),
			book_on_selection_(false) {}

		widgets::table_view_impl::~table_view_impl() { discard_resources(); }

		widgets::widget_type
			widgets::table_view_impl::type() {
			return lecui::widgets::widget_type::table_view;
		}

		HRESULT widgets::table_view_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			specs_old_ = specs_;
			is_static_ = false;
			h_cursor_ = get_cursor(specs_.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill()),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text_header()),
					&p_brush_text_header_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text_header_hot()),
					&p_brush_text_header_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text_selected()),
					&p_brush_text_selected_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill_header()),
					&p_brush_fill_header_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill_alternate()),
					&p_brush_fill_alternate_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot()),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled()),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected()),
					&p_brush_selected_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text()),
					&p_brush_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border()),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_grid()),
					&p_brush_grid_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_row_hot()),
					&p_brush_row_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_row_selected()),
					&p_brush_row_selected_);

			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory_->CreateTextFormat(
					convert_string(specs_.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(specs_.font_size()),
					L"", //locale
					&p_text_format_
					);
			}
			if (SUCCEEDED(hr)) {
				p_text_format_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				p_text_format_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(p_directwrite_factory_, p_text_format_);
			}

			resources_created_ = true;
			return hr;
		}

		void widgets::table_view_impl::discard_resources() {
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_text_header_);
			safe_release(&p_brush_text_header_hot_);
			safe_release(&p_brush_text_selected_);
			safe_release(&p_brush_fill_header_);
			safe_release(&p_brush_fill_alternate_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_grid_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets::table_view_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

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

			// use specs_.rect_ not specs_.rect() and specs_.on_resize_ not specs_.on_resize() due to redirection to special pane
			rect_ = position(specs_.rect_, specs_.on_resize_, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			const auto rect_page = page_.d_page_.get_rect();

			// step3: draw widget background
			{
				D2D1_ROUNDED_RECT rounded_rect{ rect_,
					specs_.corner_radius_x(), specs_.corner_radius_y() };

				if (render && visible_)
					p_render_target->FillRoundedRectangle(&rounded_rect, !is_enabled_ ?
						p_brush_disabled_ : p_brush_fill_);
			}

			rectA_ = { rect_.left, rect_.top + row_height_, rect_.right, rect_.bottom };

			// step4: define rectB_ (the table area)
			rectB_ = { rect_.left, rect_.top + row_height_, rect_.right, rect_.bottom };

			// for table rows to fill up horizontally (aesthetics)
			rectA_.right = largest(rectA_.right, rect_page.right);
			rectB_.right = largest(rectB_.right, rect_page.right);

			// for table rows to fill up vertically (aesthetics)
			rectA_.bottom = largest(rectA_.bottom, rect_page.bottom);
			rectB_.bottom = largest(rectB_.bottom, rect_page.bottom);

			// step5: draw header background
			{
				rect_header_ = rect_;
				rect_header_.top = rect_page.top;
				rect_header_.bottom = rect_header_.top + row_height_;
				rect_header_.right = rectA_.right;

				D2D1_ROUNDED_RECT rounded_rect{ rect_header_,
					specs_.corner_radius_x(), specs_.corner_radius_y() };

				if (render && visible_)
					p_render_target->FillRoundedRectangle(&rounded_rect, p_brush_fill_header_);
			}

			float table_width = 0.f;
			for (const auto& it : specs_.columns()) table_width += static_cast<float>(it.width);
			float table_height = row_height_ * specs_.data().size();

			// adjust rect_ (what will be written back) to match the table's used area
			rect_.bottom = rectA_.top + table_height;
			rect_.right = rectA_.left + table_width;

			// step9: define rectA_ (the area containing all the table contents)

			// step10: draw header
			{
				auto rect_header_cell = rect_header_;
				rect_header_cell.left = rectA_.left;
				rect_header_cell.right = rect_header_cell.left;

				for (const auto& it : specs_.columns()) {
					rect_header_cell.left = rect_header_cell.right;
					rect_header_cell.right = rect_header_cell.left + static_cast<float>(it.width);

					bool hot = false;

					if (specs().user_sort()) {
						// check if mouse is within this cell
						auto rect = rect_header_cell;

						header_hot_spots_[it.name] = rect;

						scale_RECT(rect, get_dpi_scale());

						// handle hit status
						if (hit_ &&
							point_.x >= rect.left && point_.x <= rect.right &&
							point_.y >= rect.top && point_.y <= rect.bottom)
							hot = true;
						else
							hot = false;

						if (hot) {
							D2D1_ROUNDED_RECT rounded_rect{ rect_header_cell,
								specs_.corner_radius_x(), specs_.corner_radius_y() };

							if (render && visible_)
								p_render_target->FillRoundedRectangle(&rounded_rect,
									p_brush_hot_);
						}
					}

					auto rect_text = rect_header_cell;
					rect_text.left += margin_;
					rect_text.right -= margin_;

					auto hr = p_directwrite_factory_->CreateTextLayout(
						convert_string(it.name).c_str(),
						(UINT32)it.name.length(),
						p_text_format_,
						rect_text.right - rect_text.left,
						rect_text.bottom - rect_text.top,
						&p_text_layout_);

					DWRITE_TEXT_RANGE text_range = { 0, static_cast<UINT32>(it.name.length()) };

					p_text_layout_->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, text_range);

					if (SUCCEEDED(hr) && render && visible_)
						p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_text.left, rect_text.top },
							p_text_layout_, hot ? p_brush_text_header_hot_ : p_brush_text_header_, D2D1_DRAW_TEXT_OPTIONS_CLIP);

					safe_release(&p_text_layout_);
				}
			}

			// step11: draw table
			{
				hot_spots_.clear();

				// step11a: clip to table area
				auto rect_clip = rectB_;
				rect_clip.top = rect_header_.bottom;
				auto_clip clip(render, p_render_target, rect_clip, 0.f);

				// step11b: figure out which rows are hidden from view and exclude them from the rendering
				// this gives a major performance boost when dealing with very large tables
				unsigned long hidden_above = (rectB_.top > rectA_.top) ?
					(static_cast<unsigned long>((rectB_.top - rectA_.top) / row_height_)) : 0;
				unsigned long hidden_below = (rectA_.bottom > rectB_.bottom) ?
					(static_cast<unsigned long>((rectA_.bottom - rectB_.bottom) / row_height_)) : 0;

				// step11c: do the drawing
				{
					auto rect_row = rectA_;
					rect_row.bottom = rect_row.top + row_height_ * (hidden_above);

					for (unsigned long row_number = hidden_above;
						row_number < (specs_.data().size() - hidden_below); row_number++) {
						rect_row.top = rect_row.bottom;
						rect_row.bottom = rect_row.top + row_height_;

						bool selected = std::find(specs_.selected().begin(), specs_.selected().end(),
							row_number) != specs_.selected().end();
						bool hot = false;

						{
							auto rect = rect_row;
							rect.left = rectB_.left;
							rect.right = rectB_.right;

							hot_spots_[row_number] = rect;

							scale_RECT(rect, get_dpi_scale());

							// handle hit status
							if (hit_ &&
								point_.x >= rect.left && point_.x <= rect.right &&
								point_.y >= rect.top && point_.y <= rect.bottom)
								hot = true;
							else
								hot = false;
						}

						if (selected) {
							auto rect = rect_row;
							rect.left = rectB_.left;
							rect.right = rectB_.right;

							D2D1_ROUNDED_RECT rounded_rect{ rect,
								specs_.corner_radius_x(), specs_.corner_radius_y() };

							if (render && visible_)
								p_render_target->FillRoundedRectangle(&rounded_rect,
									p_brush_row_selected_);
						}
						else
							if (hot) {
								auto rect = rect_row;
								rect.left = rectB_.left;
								rect.right = rectB_.right;

								D2D1_ROUNDED_RECT rounded_rect{ rect,
									specs_.corner_radius_x(), specs_.corner_radius_y() };

								if (render && visible_)
									p_render_target->FillRoundedRectangle(&rounded_rect,
										p_brush_row_hot_);
							}
							else
								if (row_number % 2 == 0) {
									// step11ci: draw alternate background
									auto rect = rect_row;
									rect.left = rectB_.left;
									rect.right = rectB_.right;

									D2D1_ROUNDED_RECT rounded_rect{ rect,
										specs_.corner_radius_x(), specs_.corner_radius_y() };

									if (render && visible_)
										p_render_target->FillRoundedRectangle(&rounded_rect,
											p_brush_fill_alternate_);
								}

						// step11cii: draw row
						auto rect_cell = rect_row;
						rect_cell.right = rect_cell.left;

						for (const auto& it : specs_.columns()) {
							rect_cell.left = rect_cell.right;
							rect_cell.right = rect_cell.left + static_cast<float>(it.width);

							auto rect_text = rect_cell;
							rect_text.left += margin_;
							rect_text.right -= margin_;

							try {
								auto value = specs_.data().at(row_number).at(it.name);

								std::string text;

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

								// create a text layout
								HRESULT hr = p_directwrite_factory_->CreateTextLayout(
									convert_string(text).c_str(),
									(UINT32)text.length(),
									p_text_format_,
									rect_text.right - rect_text.left,
									rect_text.bottom - rect_text.top,
									&p_text_layout_);

								if (SUCCEEDED(hr) && render && visible_) {
									// draw the text layout
									p_render_target->DrawTextLayout(
										D2D1_POINT_2F{ rect_text.left, rect_text.top },
										p_text_layout_,
										selected ? p_brush_text_selected_ : p_brush_,
										D2D1_DRAW_TEXT_OPTIONS_CLIP);
								}

								// release the text layout
								safe_release(&p_text_layout_);
							}
							catch (const std::exception&) {}
						}
					}
				}

				// step12: draw grid
				if (render && visible_) {
					const float line_width = .5f;

					// step12a: draw horizontal lines
					{
						auto rect_row = rectA_;
						rect_row.bottom = rect_row.top + row_height_ * (hidden_above);

						do {
							rect_row.top = rect_row.bottom;
							rect_row.bottom = rect_row.top + row_height_;

							if (rect_row.bottom > rectB_.bottom)
								break;

							p_render_target->DrawLine(D2D1::Point2F(rectB_.left, rect_row.bottom),
								D2D1::Point2F(rectB_.right, rect_row.bottom), p_brush_grid_, line_width);
						} while (true);
					}

					// step12b: draw vertical lines
					{
						auto x = rectA_.left;
						for (auto& it : specs_.columns()) {
							x += it.width;

							p_render_target->DrawLine(D2D1::Point2F(x, rectB_.top),
								D2D1::Point2F(x, rectB_.bottom), p_brush_grid_, line_width);
						}
					}
				}
			}

			return rect_;
		}

		void widgets::table_view_impl::on_click() {
			if (true) {
				if (book_on_selection_) {
					if (specs_.events().selection)
						on_selection();

					book_on_selection_ = false;
					return;
				}

				bool ctrl_pressed = (GetKeyState(VK_CONTROL) & 0x8000);
				bool shift_pressed = (GetKeyState(VK_SHIFT) & 0x8000);

				// check if any of the rows have been clicked
				bool selection_made = false;
				auto selected_previous = specs_.selected();
				specs_.selected().clear();

				for (auto& it : hot_spots_) {
					auto rect = it.second;
					scale_RECT(rect, get_dpi_scale());

					if (point_.x >= rect.left && point_.x <= rect.right &&
						point_.y >= rect.top && point_.y <= rect.bottom) {
						// change the selection
						selection_made = true;

						if (ctrl_pressed) {
							if (std::find(selected_previous.begin(), selected_previous.end(),
								it.first) == selected_previous.end()) {
								// add this row to the current selection, using the
								// order in which items appear in the table
								for (unsigned long row_number = 0; row_number < specs_.data().size();
									row_number++) {
									if (row_number == it.first)
										specs_.selected().push_back(row_number);
									else
										if (std::find(selected_previous.begin(), selected_previous.end(),
											row_number) != selected_previous.end())
											specs_.selected().push_back(row_number);
								}
							}
							else
							{
								// de-select this item
								for (const auto& m_it : selected_previous) {
									if (m_it == it.first)
										continue;
									specs_.selected().push_back(m_it);
								}
							}
						}
						else
							if (shift_pressed && !selected_previous.empty()) {
								// select all items beginning with the last selected item and this one
								if (it.first > last_selected_)
									for (unsigned long current_row = last_selected_;
										current_row <= it.first; current_row++)
										specs_.selected().push_back(current_row);
								else
									for (unsigned long current_row = it.first;
										current_row <= last_selected_; current_row++)
										specs_.selected().push_back(current_row);
							}
							else
								specs_.selected().push_back(it.first);

						/// Last selected item algorithm:
						/// 
						/// It's the item that's selected when when there is no selection or the item
						/// that's selected with shift is not pressed
						if (selected_previous.empty() || !shift_pressed)
							last_selected_ = it.first;
					}
				}

				if (selection_made) {
					if (specs_.events().selection)
						on_selection();

					if (specs_.events().click)
						specs_.events().click();

					if (specs_.events().action)
						specs_.events().action();
				}

				for (auto& [name, rectangle] : header_hot_spots_) {
					auto rect = rectangle;
					scale_RECT(rect, get_dpi_scale());

					if (point_.x >= rect.left && point_.x <= rect.right &&
						point_.y >= rect.top && point_.y <= rect.bottom) {
						// column has been clicked
						log(name + " clicked for sorting");
					}
				}
			}
		}

		bool widgets::table_view_impl::on_keydown(WPARAM wParam) {
			float adjustment = 0.f;

			switch (wParam) {
			case VK_UP:
				adjustment = row_height_;
				break;
			case VK_DOWN:
				adjustment = -row_height_;
				break;
			default:
				break;
			}

			if (adjustment != 0.f) {
				if (specs_.selected().empty()) {
					// simple scrolling
				}
				else {
					// move last selection one unit (unless it's at the beginning or the end)
					long new_selection = smallest(static_cast<long>(specs_.data().size() - 1),
						largest(0L, static_cast<long>(last_selected_) -
							static_cast<long>(adjustment / row_height_)));

					specs_.selected().clear();
					specs_.selected().push_back(new_selection);
					last_selected_ = new_selection;

					// check out if new selection is within table area
					D2D1_RECT_F rect_selected = rectA_;

					rect_selected.top += (row_height_ * new_selection);
					rect_selected.bottom = rect_selected.top + row_height_;

					float diff = 0.f;
					if (rect_selected.top < rectB_.top)
						diff = rectB_.top - rect_selected.top;	// above table area
					else
						if (rect_selected.bottom > rectB_.bottom)
							diff = rectB_.bottom - rect_selected.bottom;	// below table area

					if (diff != 0.f) {
						// move the displacement to ensure visibility of new selection
					}

					book_on_selection_ = true;
				}
			}

			if (adjustment != 0.f)
				return true;
			else
				return false;
		}

		bool widgets::table_view_impl::hit(const bool& hit) {
			if (is_static_ || hit == hit_) {
				if (hit || pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		void widgets::table_view_impl::on_right_click() {
			if (specs_.events().context_menu) {
				std::vector<table_row> var;
				for (const auto& it : specs_.selected()) {
					try { var.push_back(specs_.data().at(it)); }
					catch (const std::exception&) {}
				}

				specs_.events().context_menu(var);
			}

			if (specs_.events().right_click)
				specs_.events().right_click();
		}

		widgets::table_view_specs&
			widgets::table_view_impl::specs() { return specs_; }

		widgets::table_view_specs&
			widgets::table_view_impl::operator()() { return specs(); }

		void widgets::table_view_impl::on_selection() {
			if (specs_.events().selection) {
				std::vector<table_row> var;
				for (const auto& it : specs_.selected()) {
					try { var.push_back(specs_.data().at(it)); }
					catch (const std::exception&) {}
				}

				specs_.events().selection(var);
			}
		}
	}
}
