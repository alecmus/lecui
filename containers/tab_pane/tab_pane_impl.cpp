//
// tab_pane_impl.cpp - tab_pane_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "tab_pane_impl.h"
#include "../../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets::tab_pane_impl::tab_pane_impl(containers::page& page,
			const std::string& alias,
			const float& content_margin,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_fill(nullptr),
			_p_brush_border(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_tabs(nullptr),
			_p_brush_tabs_border(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_margin(12.f),
			_padding(5.f),	// used when caption orientation is perpendicular to the tab area
			_content_margin(content_margin),
			_tab_gap(10.f),
			_tab_text_toggle(1.5f),
			_tab_height(25.f),
			_tab_height_set(false),
			_bar_height(2.f),
			_rect_tabs({ 0.f, 0.f, 0.f, 0.f }),
			_rect_client_area({ 0.f, 0.f, 0.f, 0.f }),
			_rect_tab_pane({ 0.f, 0.f, 0.f, 0.f }) {}

		widgets::tab_pane_impl::~tab_pane_impl() { discard_resources(); }

		widgets::widget_type
			widgets::tab_pane_impl::type() {
			return lecui::widgets::widget_type::tab_pane;
		}

		HRESULT widgets::tab_pane_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_text()),
					&_p_brush);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_fill()),
					&_p_brush_fill);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_border()),
					&_p_brush_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_hot()),
					&_p_brush_hot);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_disabled()),
					&_p_brush_disabled);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_tabs()),
					&_p_brush_tabs);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_tabs_border()),
					&_p_brush_tabs_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(_specs.color_selected()),
					&_p_brush_selected);
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
				_p_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				_p_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(_p_directwrite_factory, _p_text_format);
			}

			_resources_created = true;
			return hr;
		}

		void widgets::tab_pane_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_fill);
			safe_release(&_p_brush_border);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_tabs);
			safe_release(&_p_brush_tabs_border);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::tab_pane_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;
				discard_resources();
			}

			if (!_initial_capture_done) {
				_rect_previous = _specs.rect();
				_initial_capture_done = true;
			}

			if (_specs.rect() != _rect_previous && _p_page_impl && _p_tab_pane_rect) {
				log("rect changed: " + _alias);

				// pane rect changed
				const auto& width_change = _specs.rect().width() - _rect_previous.width();
				const auto& height_change = _specs.rect().height() - _rect_previous.height();

				_rect_previous = _specs.rect();

				// adjust pane rect accordingly

				// page dimensions
				_p_page_impl->width(_p_page_impl->width() + width_change);
				_p_page_impl->height(_p_page_impl->height() + height_change);

				// pane dimensions
				_p_tab_pane_rect->width(_p_tab_pane_rect->width() + width_change);
				_p_tab_pane_rect->height(_p_tab_pane_rect->height() + height_change);

				// scroll bars

				// horizontal scrollbar
				_p_page_impl->h_scrollbar().specs().rect().right(_p_page_impl->h_scrollbar().specs().rect().right() + width_change);

				_p_page_impl->h_scrollbar().specs().rect().top(_p_page_impl->h_scrollbar().specs().rect().top() + height_change);
				_p_page_impl->h_scrollbar().specs().rect().bottom(_p_page_impl->h_scrollbar().specs().rect().bottom() + height_change);

				// vertical scrollbar
				_p_page_impl->v_scrollbar().specs().rect().bottom(_p_page_impl->v_scrollbar().specs().rect().bottom() + height_change);

				_p_page_impl->v_scrollbar().specs().rect().left(_p_page_impl->v_scrollbar().specs().rect().left() + width_change);
				_p_page_impl->v_scrollbar().specs().rect().right(_p_page_impl->v_scrollbar().specs().rect().right() + width_change);
			}

			if (!_resources_created)
				create_resources(p_render_target);

			_rect_tab_pane = position(_specs.rect(), _specs.on_resize(), change_in_size.width, change_in_size.height);
			_rect_tab_pane.left -= offset.x;
			_rect_tab_pane.right -= offset.x;
			_rect_tab_pane.top -= offset.y;
			_rect_tab_pane.bottom -= offset.y;

			/// compute _rect_tabs
			_rect_tabs = _rect_tab_pane;

			switch (_specs.tab_side()) {
			case containers::tab_pane::side::left:
				_rect_tabs.right = _rect_tabs.left + _tab_height + _bar_height;
				break;

			case containers::tab_pane::side::right:
				_rect_tabs.left = _rect_tabs.right - (_tab_height + _bar_height);
				break;

			case containers::tab_pane::side::bottom:
				_rect_tabs.top = _rect_tabs.bottom - (_tab_height + _bar_height);
				break;

			case containers::tab_pane::side::top:
			default:
				_rect_tabs.bottom = _rect_tabs.top + _tab_height + _bar_height;
				break;
			}

			/// compute _rect_client_area
			_rect_client_area = _rect_tab_pane;

			switch (_specs.tab_side()) {
			case containers::tab_pane::side::left:
				_rect_client_area.left = _rect_tabs.right;
				break;

			case containers::tab_pane::side::right:
				_rect_client_area.right = _rect_tabs.left;
				break;

			case containers::tab_pane::side::bottom:
				_rect_client_area.bottom = _rect_tabs.top;
				break;

			case containers::tab_pane::side::top:
			default:
				_rect_client_area.top = _rect_tabs.bottom;
				break;
			}

			// only make the tab area respond to hit testing, even though for scroll bar at form level
			// we need to return the entire region through _rect_tab_pane
			_rect = _rect_tabs;

			if (!render || !_visible)
				return _rect_tab_pane;

			/// draw client area background and border
			D2D1_ROUNDED_RECT rounded_rect{ _rect_client_area,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_fill : _p_brush_disabled);
			p_render_target->DrawRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_border : _p_brush_disabled, _specs.border());

			auto measure_string = [](IDWriteFactory* _p_directwrite_factory,
				IDWriteTextFormat* _p_text_format, IDWriteTextLayout* _p_text_layout,
				const std::string& _text, float max_width, float max_height) {
					D2D1_SIZE_F minSize = { 0.f, 0.f };

					// create a text layout
					HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(_text).c_str(),
						(UINT32)_text.length(), _p_text_format, max_width, max_height, &_p_text_layout);

					// measure minimum width required
					if (SUCCEEDED(hr)) {
						DWRITE_TEXT_METRICS textMetrics;
						_p_text_layout->GetMetrics(&textMetrics);
						minSize.width = textMetrics.width;
						minSize.height = textMetrics.height;
					}

					// release the text layout
					safe_release(&_p_text_layout);

					return minSize;
			};

			/// draw tabs rectangle
			rounded_rect = { _rect_tabs,
				_specs.corner_radius_x(), _specs.corner_radius_y() };

			p_render_target->FillRoundedRectangle(&rounded_rect, _p_brush_tabs);
			p_render_target->DrawRoundedRectangle(&rounded_rect, _is_enabled ?
				_p_brush_tabs_border : _p_brush_disabled, _specs.tabs_border());

			/// draw the tab text
			D2D1_RECT_F _rect_current_tab = _rect_tabs;

			switch (_specs.tab_side()) {
			case containers::tab_pane::side::left:
				_rect_current_tab.right -= _bar_height;
				_rect_current_tab.bottom = _rect_current_tab.top;
				break;

			case containers::tab_pane::side::right:
				_rect_current_tab.left += _bar_height;
				_rect_current_tab.bottom = _rect_current_tab.top;
				break;

			case containers::tab_pane::side::bottom:
				_rect_current_tab.top += _bar_height;
				_rect_current_tab.right = _rect_current_tab.left;
				break;

			case containers::tab_pane::side::top:
			default:
				_rect_current_tab.bottom -= _bar_height;
				_rect_current_tab.right = _rect_current_tab.left;
				break;
			}

			for (const auto& tab_name : _tab_order) {

				D2D1_SIZE_F min_size = { 0.f, 0.f };

				switch (_specs.tab_side()) {
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					// calculate tab rect
					_rect_current_tab.top = _rect_current_tab.bottom;
					_rect_current_tab.bottom = _rect_client_area.bottom;

					// calculate minimum size (we will rotate the text, so we're factoring this into this computation)
					min_size = measure_string(_p_directwrite_factory, _p_text_format,
						_p_text_layout, tab_name, _rect_current_tab.bottom - _rect_current_tab.top,
						_rect_current_tab.right - _rect_current_tab.left);
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					// calculate tab rect
					_rect_current_tab.left = _rect_current_tab.right;
					_rect_current_tab.right = _rect_client_area.right;

					// calculate minimum size
					min_size = measure_string(_p_directwrite_factory, _p_text_format,
						_p_text_layout, tab_name, _rect_current_tab.right - _rect_current_tab.left,
						_rect_current_tab.bottom - _rect_current_tab.top);
					break;
				}

				D2D1_RECT_F _rect_text = { 0.f, 0.f, 0.f, 0.f };
				bool excess = false;

				switch (_specs.tab_side()) {
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					if (_specs.caption_orientation() == containers::tab_pane::orientation::vertical) {
						swap(min_size.width, min_size.height);
						_rect_current_tab.bottom = _rect_current_tab.top + min_size.height + 2.f * _tab_gap;
						_rect_text = _rect_current_tab;
					}
					else {
						_rect_current_tab.bottom = _rect_current_tab.top + 25.f + (2 * _tab_gap);
						_rect_text = _rect_current_tab;
						_rect_text.left += _padding;
						_rect_text.right = smallest(_rect_text.left + min_size.width, _rect_current_tab.right - 2 * _padding);
					}

					if (_rect_text.bottom > _rect_tab_pane.bottom)
						excess = true;

					_rect_text.top += _tab_gap;
					_rect_text.bottom -= _tab_gap;
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					if (_specs.caption_orientation() == containers::tab_pane::orientation::horizontal) {
						_rect_current_tab.right = _rect_current_tab.left + min_size.width + 2.f * _tab_gap;
						_rect_text = _rect_current_tab;
					}
					else {
						_rect_current_tab.right = _rect_current_tab.left + 25.f + (2 * _tab_gap);
						_rect_text = _rect_current_tab;
						_rect_text.bottom -= _padding;
						_rect_text.top = largest(_rect_text.bottom - min_size.width, _rect_current_tab.top + 2 * _padding);
					}

					if (_rect_text.right > _rect_tab_pane.right)
						excess = true;

					_rect_text.left += _tab_gap;
					_rect_text.right -= _tab_gap;
					break;
				}

				if (excess)
					break;

				// capture current tab
				_p_tab_rects[tab_name];	// add if it doesn't yet exist

				{
					D2D1_RECT_F rect = _rect_current_tab;

					// for aesthetics and to allow switching when moving over from one tab to the other

					switch (_specs.tab_side()) {
					case containers::tab_pane::side::left:
					case containers::tab_pane::side::right:
						rect.top += _tab_gap;
						rect.bottom -= _tab_gap;
						break;

					case containers::tab_pane::side::top:
					case containers::tab_pane::side::bottom:
					default:
						rect.left += _tab_gap;
						rect.right -= _tab_gap;
						break;
					}

					_p_tab_rects.at(tab_name) = rect;
				}

				if (!_is_static && _is_enabled && _hit) {
					D2D1_RECT_F rect = _rect_current_tab;

					switch (_specs.tab_side()) {
					case containers::tab_pane::side::left:
					case containers::tab_pane::side::right:
						rect.top += _tab_gap;
						rect.bottom -= _tab_gap;
						break;

					case containers::tab_pane::side::top:
					case containers::tab_pane::side::bottom:
					default:
						rect.left += _tab_gap;
						rect.right -= _tab_gap;
						break;
					}

					scale_RECT(rect, get_dpi_scale());

					if (_point.x >= rect.left && _point.x <= rect.right &&
						_point.y >= rect.top && _point.y <= rect.bottom) {
						if (!_is_static && _is_enabled && _hit) {
							if (_pressed)
								_specs.selected(tab_name);	// it's a tab, don't wait for a click!

							// move text a little (visual effect)
							switch (_specs.tab_side()) {
							case containers::tab_pane::side::left:
								_rect_text.right += _tab_text_toggle;
								_rect_text.left += _tab_text_toggle;
								break;

							case containers::tab_pane::side::right:
								_rect_text.left += _tab_text_toggle;
								_rect_text.right += _tab_text_toggle;
								break;

							case containers::tab_pane::side::bottom:
								_rect_text.top += _tab_text_toggle;
								_rect_text.bottom += _tab_text_toggle;
								break;

							case containers::tab_pane::side::top:
							default:
								_rect_text.bottom -= _tab_text_toggle;
								_rect_text.top -= _tab_text_toggle;
								break;
							}
						}
					}
				}

				switch (_specs.tab_side()) {
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right: {
					if (_specs.caption_orientation() == containers::tab_pane::orientation::vertical) {
						// rotate the text rectangle about its center

						const D2D1_POINT_2F center =
						{ _rect_text.left + (_rect_text.right - _rect_text.left) / 2.f,
							_rect_text.top + (_rect_text.bottom - _rect_text.top) / 2.f };

						D2D1_SIZE_F size =
						{ _rect_text.right - _rect_text.left ,
							_rect_text.bottom - _rect_text.top };
						swap(size.width, size.height);

						_rect_text.left = center.x - (size.width / 2.f);
						_rect_text.right = _rect_text.left + size.width;
						_rect_text.top = center.y - (size.height / 2.f);
						_rect_text.bottom = _rect_text.top + size.height;

						D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(90.f, center);

						D2D1_MATRIX_3X2_F old_transform;
						p_render_target->GetTransform(&old_transform);

						p_render_target->SetTransform(rotation);

						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), _p_text_format, _rect_text.right - _rect_text.left,
							_rect_text.bottom - _rect_text.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						p_render_target->SetTransform(old_transform);
					}
					else {
						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), _p_text_format, _rect_text.right - _rect_text.left,
							_rect_text.bottom - _rect_text.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}
					}
				}
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom: {
				default:
					if (_specs.caption_orientation() == containers::tab_pane::orientation::horizontal) {
						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), _p_text_format, _rect_text.right - _rect_text.left,
							_rect_text.bottom - _rect_text.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}
					}
					else {
						// rotate the text rectangle about its center

						const D2D1_POINT_2F center =
						{ _rect_text.left + (_rect_text.right - _rect_text.left) / 2.f,
							_rect_text.top + (_rect_text.bottom - _rect_text.top) / 2.f };

						D2D1_SIZE_F size =
						{ _rect_text.right - _rect_text.left ,
							_rect_text.bottom - _rect_text.top };
						swap(size.width, size.height);

						_rect_text.left = center.x - (size.width / 2.f);
						_rect_text.right = _rect_text.left + size.width;
						_rect_text.top = center.y - (size.height / 2.f);
						_rect_text.bottom = _rect_text.top + size.height;

						D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(-90.f, center);

						D2D1_MATRIX_3X2_F old_transform;
						p_render_target->GetTransform(&old_transform);

						p_render_target->SetTransform(rotation);

						// create a text layout
						HRESULT hr = _p_directwrite_factory->CreateTextLayout(convert_string(tab_name).c_str(),
							(UINT32)tab_name.length(), _p_text_format, _rect_text.right - _rect_text.left,
							_rect_text.bottom - _rect_text.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_text.left, _rect_text.top },
								_p_text_layout, _is_enabled ?
								_p_brush : _p_brush_disabled, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						p_render_target->SetTransform(old_transform);
					}
				}
					break;
				}

				// release the text layout
				safe_release(&_p_text_layout);
			}

			// draw the selected tab bar
			for (const auto& it : _p_tab_rects) {
				bool excess = false;
				switch (_specs.tab_side()) {
				case containers::tab_pane::side::left:
				case containers::tab_pane::side::right:
					if ((it.second.bottom + _tab_gap) > _rect_tab_pane.bottom)
						excess = true;
					break;

				case containers::tab_pane::side::top:
				case containers::tab_pane::side::bottom:
				default:
					if ((it.second.right + _tab_gap) > _rect_tab_pane.right)
						excess = true;
					break;
				}

				if (excess)
					break;

				if (it.first == _specs.selected()) {
					D2D1_RECT_F _rect_bar = it.second;

					switch (_specs.tab_side()) {
					case containers::tab_pane::side::left:
						_rect_bar.left = _rect_bar.right;
						_rect_bar.right = _rect_bar.right += _bar_height;
						break;

					case containers::tab_pane::side::right:
						_rect_bar.right = _rect_bar.left;
						_rect_bar.left = _rect_bar.left -= _bar_height;
						break;

					case containers::tab_pane::side::bottom:
						_rect_bar.bottom = _rect_bar.top;
						_rect_bar.top = _rect_bar.top -= _bar_height;
						break;

					case containers::tab_pane::side::top:
					default:
						_rect_bar.top = _rect_bar.bottom;
						_rect_bar.bottom = _rect_bar.bottom += _bar_height;
						break;
					}

					p_render_target->FillRectangle(&_rect_bar, _p_brush_selected);
					break;
				}
			}

			return _rect_tab_pane;
		}

		void widgets::tab_pane_impl::on_click() {
			for (auto& it : _p_tab_rects) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, get_dpi_scale());

				if (_point.x >= rect.left && _point.x <= rect.right &&
					_point.y >= rect.top && _point.y <= rect.bottom)
					_specs.selected(it.first);
			}
		}

		containers::tab_pane&
			widgets::tab_pane_impl::specs() { return _specs; }

		containers::tab_pane&
			widgets::tab_pane_impl::operator()() { return specs(); }

		const D2D1_RECT_F& widgets::tab_pane_impl::client_area() { return _rect_client_area; }

		const D2D1_RECT_F& widgets::tab_pane_impl::tab_pane_area() { return _rect_tab_pane; }

		const float& widgets::tab_pane_impl::content_margin() { return _content_margin; }

		float widgets::tab_pane_impl::caption_bar_height() { return (_tab_height + _bar_height); }

		float widgets::tab_pane_impl::tab_height() { return _tab_height; }

		float widgets::tab_pane_impl::bar_height() { return _bar_height; }

		float widgets::tab_pane_impl::padding() { return _padding; }

		float widgets::tab_pane_impl::margin() { return _margin; }

		void widgets::tab_pane_impl::set_tab_height(const float height) {
			_tab_height_set = true;
			_tab_height = height;
		}

		bool widgets::tab_pane_impl::tab_height_set() { return _tab_height_set; }

		void widgets::tab_pane_impl::close_tab(const std::string& tab_name) {
			try {
				// erase from tabs (all widgets within this tab are deleted immediately)
				_p_tabs.erase(tab_name);

				// erase from tab rects
				_p_tab_rects.erase(tab_name);

				// skip in tabs order
				std::vector<std::string> temp_tab_order;
				for (auto& tab : _tab_order) {
					if (tab != tab_name)
						temp_tab_order.push_back(tab);
				}
				_tab_order = temp_tab_order;
			}
			catch (const std::exception&) {}
		}

		bool widgets::tab_pane_impl::contains() {
			for (const auto& it : _p_tab_rects) {
				D2D1_RECT_F rect = it.second;
				scale_RECT(rect, get_dpi_scale());

				if (_point.x >= rect.left && _point.x <= rect.right &&
					_point.y >= rect.top && _point.y <= rect.bottom)
					return true;
			}

			return false;
		}
	}
}
