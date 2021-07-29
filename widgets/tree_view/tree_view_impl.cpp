//
// tree_view_impl.cpp - tree_view_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "tree_view_impl.h"
#include "../label/label_impl.h"
#include "../../form_impl/form_impl.h"
#include "../../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets::tree_view_impl::tree_view_impl(containers::page& page,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			widget_impl(page, alias),
			_p_brush(nullptr),
			_p_brush_hot(nullptr),
			_p_brush_disabled(nullptr),
			_p_brush_selected(nullptr),
			_p_text_format(nullptr),
			_p_direct2d_factory(p_direct2d_factory),
			_p_directwrite_factory(p_directwrite_factory),
			_p_text_layout(nullptr),
			_margin(0.f)	// the tree will be moved into a special tree pane. The pane will have a margin!
		{}

		widgets::tree_view_impl::~tree_view_impl() { discard_resources(); }

		widgets::widget_type
			widgets::tree_view_impl::type() {
			return lecui::widgets::widget_type::tree_view;
		}

		HRESULT widgets::tree_view_impl::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			_specs_old = _specs;
			_is_static = false;
			_h_cursor = get_cursor(_specs.cursor());

			HRESULT hr = S_OK;

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

		void widgets::tree_view_impl::discard_resources() {
			_resources_created = false;
			safe_release(&_p_brush);
			safe_release(&_p_brush_hot);
			safe_release(&_p_brush_disabled);
			safe_release(&_p_brush_selected);
			safe_release(&_p_text_format);
		}

		D2D1_RECT_F&
			widgets::tree_view_impl::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (_specs_old != _specs) {
				log("specs changed: " + _alias);
				_specs_old = _specs;

				try {
					if (_tree_pane_specs.has_value()) {
						// update the special pane widget
						_tree_pane_specs.value().get().color_fill() = _specs.color_fill();
						_tree_pane_specs.value().get().color_border() = _specs.color_border();

						// schedule a refresh
						_page._d_page.get_form()._d._schedule_refresh = true;
					}
				}
				catch (const std::exception& e) { log(e.what()); }

				discard_resources();
			}

			if (!_resources_created)
				create_resources(p_render_target);

			// use _specs._rect not _specs.rect() and _specs._on_resize not _specs.on_resize() due to redirection to special pane
			_rect = position(_specs._rect, _specs._on_resize, change_in_size.width, change_in_size.height);
			_rect.left -= offset.x;
			_rect.right -= offset.x;
			_rect.top -= offset.y;
			_rect.bottom -= offset.y;

			if (!render || !_visible)
				return _rect;

			class helper {
			public:
				static void draw_level(ID2D1Factory* p_direct2d_factory,
					ID2D1HwndRenderTarget* p_render_target, IDWriteFactory* p_directwrite_factory,
					IDWriteTextFormat* _p_text_format, ID2D1SolidColorBrush* p_brush,
					ID2D1SolidColorBrush* p_brush_selected, ID2D1SolidColorBrush* _p_brush_hot,
					const std::string& font, const float& font_size, std::map<std::string,
					widgets::tree_view::node>& level, const D2D1_RECT_F& rect,
					float& _right, float& _bottom, float& _optimized_right, float& _optimized_bottom, bool _hit, D2D1_POINT_2F _point,
					float _dpi_scale) {
					float bottom = rect.top;
					float right = rect.right;

					auto rect_node = rect;
					rect_node.left += 10.f;
					rect_node.right += 10.f;

					for (auto& node : level) {
						const bool has_children = !node.second.children.empty();

						rect_node.top = bottom;
						rect_node.bottom = rect_node.top + 20.f;
						rect_node = widgets::measure_text(p_directwrite_factory, node.first,
							font, font_size, false, true, true, false, rect_node);
						rect_node.right += 1.f;	// failsafe to prevent occasional ellipses when scrolling horizontally

						// capture node rectangle
						node.second.rc = convert_rect(rect_node);

						// capture optimized dimensions
						_optimized_right = largest(_optimized_right, rect_node.right);
						_optimized_bottom = largest(_optimized_bottom, rect_node.bottom);

						right = largest(right, rect_node.right);
						bottom = largest(bottom, rect_node.bottom);

						// make adjustments to global trackers
						_right = largest(_right, right);
						_bottom = largest(_bottom, bottom);

						D2D1_RECT_F rect_marker = { 0.f, 0.f, 0.f, 0.f };
						if (has_children) {
							// draw marker
							D2D1_RECT_F rect_marker_ref = rect_node;
							rect_marker_ref.left = rect.left;
							rect_marker_ref.right = rect_node.left;

							rect_marker = rect_marker_ref;
							rect_marker.bottom = rect_marker.top + 10.f;
							rect_marker.right = rect_marker.left + 10.f;

							pos_rect(rect_marker_ref, rect_marker, 50.f, 50.f);

							const float side = 10.f;

							HRESULT hr = S_OK;
							ID2D1PathGeometry* p_checkbox_geometry = nullptr;
							hr = p_direct2d_factory->CreatePathGeometry(&p_checkbox_geometry);

							if (SUCCEEDED(hr)) {
								ID2D1GeometrySink* p_sink = nullptr;
								hr = p_checkbox_geometry->Open(&p_sink);
								if (SUCCEEDED(hr)) {
									p_sink->SetFillMode(D2D1_FILL_MODE_WINDING);

									/// Marker shapes designed using a square of size 10x10
									if (node.second.expand) {
										///            (8,2)
										///          / |
										///        /   |
										///      /     |
										///    /       |
										///   ---------
										/// (2,8)      (8,8)
										/// 
										p_sink->BeginFigure(
											/// (2,8)
											D2D1::Point2F(rect_marker.left + (.2f * side), rect_marker.top + (.8f * side)),
											D2D1_FIGURE_BEGIN_FILLED
											);
										D2D1_POINT_2F points[] = {
											/// (8,2)
											D2D1::Point2F(rect_marker.left + (.8f * side), rect_marker.top + (.2f * side)),
											/// (8,8)
											D2D1::Point2F(rect_marker.left + (.8f * side), rect_marker.top + (.8f * side)),
										};
										p_sink->AddLines(points, ARRAYSIZE(points));
									}
									else {
										///  (3,1)
										///  | \
										///  |   \
										///  |     \ (7,5)
										///  |     /
										///  |   /
										///  | /
										///  (3,9)
										/// 
										p_sink->BeginFigure(
											/// (3,1)
											D2D1::Point2F(rect_marker.left + (.3f * side), rect_marker.top + (.1f * side)),
											D2D1_FIGURE_BEGIN_FILLED
											);
										D2D1_POINT_2F points[] = {
											/// (7,5)
											D2D1::Point2F(rect_marker.left + (.7f * side), rect_marker.top + (.5f * side)),
											/// (3,9)
											D2D1::Point2F(rect_marker.left + (.3f * side), rect_marker.top + (.9f * side)),
										};
										p_sink->AddLines(points, ARRAYSIZE(points));
									}

									p_sink->EndFigure(D2D1_FIGURE_END_CLOSED);
									hr = p_sink->Close();
									safe_release(&p_sink);
								}

								// draw the geometry
								p_render_target->FillGeometry(p_checkbox_geometry, p_brush);
								safe_release(&p_checkbox_geometry);
							}
						}

						// capture marker rectangle
						node.second.rc_expand = convert_rect(rect_marker);

						// check if mouse is in rect
						if (_hit) {
							auto rc = rect_node;
							scale_RECT(rc, _dpi_scale);

							// handle hit status
							bool hot = false;
							if (
								_point.x >= rc.left && _point.x <= rc.right &&
								_point.y >= rc.top && _point.y <= rc.bottom)
								hot = true;
							else
								hot = false;

							if (hot)
								p_render_target->FillRectangle(rect_node, _p_brush_hot);
						}

						if (node.second.selected) {
							p_render_target->FillRectangle(rect_node, p_brush_selected);
						}

						// create a text layout
						IDWriteTextLayout* _p_text_layout = nullptr;
						HRESULT hr = p_directwrite_factory->CreateTextLayout(convert_string(node.first).c_str(),
							(UINT32)node.first.length(), _p_text_format, rect_node.right - rect_node.left,
							rect_node.bottom - rect_node.top, &_p_text_layout);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_node.left, rect_node.top },
								_p_text_layout, p_brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&_p_text_layout);

						bottom = rect_node.bottom;

						if (node.second.expand && has_children) {
							auto rect_child = rect_node;
							rect_child.top = rect_node.bottom;
							rect_child.bottom = rect_child.top + 20.f;
							rect_child.left += 10.f;
							rect_child.right += 10.f;

							draw_level(p_direct2d_factory, p_render_target, p_directwrite_factory,
								_p_text_format, p_brush, p_brush_selected, _p_brush_hot, font,
								font_size, node.second.children, rect_child, _right, _bottom,
								_optimized_right, _optimized_bottom, _hit, _point, _dpi_scale);	// recursion

							right = largest(right, _right);
							bottom = largest(bottom, _bottom);
						}
					}

				}
			};

			// draw the tree
			auto _right = _rect.right;
			auto _bottom = _rect.top + 20;
			auto _optimized_right = 0.f;
			auto _optimized_bottom = 0.f;
			helper::draw_level(_p_direct2d_factory, p_render_target, _p_directwrite_factory,
				_p_text_format, _p_brush, _p_brush_selected, _p_brush_hot, _specs.font(),
				_specs.font_size(), _specs.root(), _rect, _right, _bottom, _optimized_right,
				_optimized_bottom, _hit, _point, get_dpi_scale());

			const auto width = _optimized_right - _rect.left;
			const auto height = _optimized_bottom - _rect.top;

			// update widget rect
			// use _specs._rect not _specs.rect() due to redirection to special pane
			_specs._rect.width(width);
			_specs._rect.height(height);

			return _rect;
		}

		void widgets::tree_view_impl::on_click() {
			class helper {
			public:
				static void check(std::map<std::string, widgets::tree_view::node>& level, D2D1_POINT_2F _point, float _dpi_scale) {
					for (auto& node : level) {
						// check if marker has been clicked
						auto rect_marker = convert_rect(node.second.rc_expand);
						auto rect_node = convert_rect(node.second.rc);
						scale_RECT(rect_marker, _dpi_scale);
						scale_RECT(rect_node, _dpi_scale);

						if (_point.x >= rect_marker.left && _point.x <= rect_marker.right &&
							_point.y >= rect_marker.top && _point.y <= rect_marker.bottom)
							node.second.expand = !node.second.expand;

						if (_point.x >= rect_node.left && _point.x <= rect_node.right &&
							_point.y >= rect_node.top && _point.y <= rect_node.bottom)
							node.second.selected = true;
						else
							node.second.selected = false;

						check(node.second.children, _point, _dpi_scale);	// recursion
					}
				}
			};

			// mark selected
			helper::check(_specs.root(), _point, get_dpi_scale());

			// handle on_selection
			if (_specs.events().selection)
				on_selection();

			// handle on_click
			if (_specs.events().click)
				_specs.events().click();

			if (_specs.events().action)
				_specs.events().action();
		}

		bool widgets::tree_view_impl::hit(const bool& hit) {
			if (_is_static || hit == _hit) {
				if (hit || _pressed)
					return true;
				else
					return false;
			}

			_hit = hit;
			return true;
		}

		void widgets::tree_view_impl::on_right_click() {
			if (_specs.events().right_click)
				_specs.events().right_click();
		}

		widgets::tree_view&
			widgets::tree_view_impl::specs() { return _specs; }

		widgets::tree_view&
			widgets::tree_view_impl::operator()() { return specs(); }

		void widgets::tree_view_impl::set_tree_pane_specs(containers::pane_specs& specs) {
			_tree_pane_specs = specs;
		}

		void widgets::tree_view_impl::on_selection() {
			class helper {
			public:
				static void check(std::map<std::string, widgets::tree_view::node>& level,
					std::function<void(widgets::tree_view::node& n)> selection) {
					for (auto& node : level) {
						if (node.second.selected) {
							if (selection)
								selection(node.second);
						}
						else
							check(node.second.children, selection);	// recursion
					}
				}
			};

			helper::check(_specs.root(), _specs.events().selection);
		}
	}
}
