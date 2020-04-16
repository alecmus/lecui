/*
** tree_impl.cpp - tree_impl implementation
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

#include "tree_impl.h"
#include "../label/label_impl.h"

namespace liblec {
	namespace lecui {
		widgets_impl::tree::tree(const std::string& page_alias,
			const std::string& alias,
			ID2D1Factory* p_direct2d_factory,
			IDWriteFactory* p_directwrite_factory) :
			p_brush_(nullptr),
			p_brush_border_(nullptr),
			p_brush_fill_(nullptr),
			p_brush_hot_(nullptr),
			p_brush_disabled_(nullptr),
			p_brush_selected_(nullptr),
			p_text_format_(nullptr),
			p_direct2d_factory_(p_direct2d_factory),
			p_directwrite_factory_(p_directwrite_factory),
			p_text_layout_(nullptr),
			margin_(0.f)	// the tree will be moved into a special tree pane. The pane will have a margin!
		{
			page_alias_ = page_alias;
			alias_ = alias;
		}

		widgets_impl::tree::~tree() { discard_resources(); }

		widgets_impl::widget_type
			widgets_impl::tree::type() {
			return lecui::widgets_impl::widget_type::tree;
		}

		HRESULT widgets_impl::tree::create_resources(
			ID2D1HwndRenderTarget* p_render_target) {
			log("creating resources:   " + page_alias_ + ":" + alias_);
			specs_old_ = specs_;
			is_static_ = false;

			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_fill),
					&p_brush_fill_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_border),
					&p_brush_border_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_hot),
					&p_brush_hot_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_disabled),
					&p_brush_disabled_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_selected),
					&p_brush_selected_);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(specs_.color_text),
					&p_brush_);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory_->CreateTextFormat(
					convert_string(specs_.font).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(specs_.font_size),
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

		void widgets_impl::tree::discard_resources() {
			log("discarding resources: " + page_alias_ + ":" + alias_);
			resources_created_ = false;
			safe_release(&p_brush_);
			safe_release(&p_brush_border_);
			safe_release(&p_brush_fill_);
			safe_release(&p_brush_hot_);
			safe_release(&p_brush_disabled_);
			safe_release(&p_brush_selected_);
			safe_release(&p_text_format_);
		}

		D2D1_RECT_F&
			widgets_impl::tree::render(ID2D1HwndRenderTarget* p_render_target,
				const D2D1_SIZE_F& change_in_size, const D2D1_POINT_2F& offset, const bool& render) {
			if (specs_old_ != specs_) {
				log("specs changed: " + alias_);
				specs_old_ = specs_;
				discard_resources();
			}

			if (!resources_created_)
				create_resources(p_render_target);

			rect_ = position(specs_.rect, specs_.on_resize, change_in_size.width, change_in_size.height);
			rect_.left -= offset.x;
			rect_.right -= offset.x;
			rect_.top -= offset.y;
			rect_.bottom -= offset.y;

			if (!render || !visible_)
				return rect_;

			class helper {
			public:
				static void draw_level(ID2D1Factory* p_direct2d_factory,
					ID2D1HwndRenderTarget* p_render_target, IDWriteFactory* p_directwrite_factory,
					IDWriteTextFormat* p_text_format_, ID2D1SolidColorBrush* p_brush,
					ID2D1SolidColorBrush* p_brush_selected, ID2D1SolidColorBrush* p_brush_hot_,
					const std::string& font, const float& font_size, std::map<std::string,
					widgets::tree::tree_specs::node>& level, const D2D1_RECT_F& rect,
					float& right_, float& bottom_, float& optimized_right_, float& optimized_bottom_, bool hit_, D2D1_POINT_2F point_,
					float dpi_scale_) {
					float bottom = rect.top;
					float right = rect.right;

					auto rect_node = rect;
					rect_node.left += 10.f;
					rect_node.right += 10.f;

					for (auto& node : level) {
						const bool has_children = !node.second.children.empty();

						rect_node.top = bottom;
						rect_node.bottom = rect_node.top + 20.f;
						rect_node = widgets_impl::measure_text(p_directwrite_factory, node.first,
							font, font_size, false, true, true, false, rect_node);
						rect_node.right += 1.f;	// failsafe to prevent occasional ellipses when scrolling horizontally

						// capture node rectangle
						node.second.rc = convert_rect(rect_node);

						// capture optimized dimensions
						optimized_right_ = largest(optimized_right_, rect_node.right);
						optimized_bottom_ = largest(optimized_bottom_, rect_node.bottom);

						right = largest(right, rect_node.right);
						bottom = largest(bottom, rect_node.bottom);

						// make adjustments to global trackers
						right_ = largest(right_, right);
						bottom_ = largest(bottom_, bottom);

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
						if (hit_) {
							auto rc = rect_node;
							scale_RECT(rc, dpi_scale_);

							// handle hit status
							bool hot = false;
							if (
								point_.x >= rc.left && point_.x <= rc.right &&
								point_.y >= rc.top && point_.y <= rc.bottom)
								hot = true;
							else
								hot = false;

							if (hot)
								p_render_target->FillRectangle(rect_node, p_brush_hot_);
						}

						if (node.second.selected) {
							p_render_target->FillRectangle(rect_node, p_brush_selected);
						}

						// create a text layout
						IDWriteTextLayout* p_text_layout_ = nullptr;
						HRESULT hr = p_directwrite_factory->CreateTextLayout(convert_string(node.first).c_str(),
							(UINT32)node.first.length(), p_text_format_, rect_node.right - rect_node.left,
							rect_node.bottom - rect_node.top, &p_text_layout_);

						if (SUCCEEDED(hr)) {
							// draw the text layout
							p_render_target->DrawTextLayout(D2D1_POINT_2F{ rect_node.left, rect_node.top },
								p_text_layout_, p_brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
						}

						// release the text layout
						safe_release(&p_text_layout_);

						bottom = rect_node.bottom;

						if (node.second.expand && has_children) {
							auto rect_child = rect_node;
							rect_child.top = rect_node.bottom;
							rect_child.bottom = rect_child.top + 20.f;
							rect_child.left += 10.f;
							rect_child.right += 10.f;

							draw_level(p_direct2d_factory, p_render_target, p_directwrite_factory,
								p_text_format_, p_brush, p_brush_selected, p_brush_hot_, font,
								font_size, node.second.children, rect_child, right_, bottom_,
								optimized_right_, optimized_bottom_, hit_, point_, dpi_scale_);	// recursion

							right = largest(right, right_);
							bottom = largest(bottom, bottom_);
						}
					}

				}
			};

			// draw the tree
			auto right_ = rect_.right;
			auto bottom_ = rect_.top + 20;
			auto optimized_right_ = 0.f;
			auto optimized_bottom_ = 0.f;
			helper::draw_level(p_direct2d_factory_, p_render_target, p_directwrite_factory_,
				p_text_format_, p_brush_, p_brush_selected_, p_brush_hot_, specs_.font,
				specs_.font_size, specs_.root, rect_, right_, bottom_, optimized_right_,
				optimized_bottom_, hit_, point_, dpi_scale_);

			const auto width = optimized_right_ - rect_.left;
			const auto height = optimized_bottom_ - rect_.top;

			// update widget rect
			specs_.rect.width(width);
			specs_.rect.height(height);

			return rect_;
		}

		void widgets_impl::tree::on_click() {
			class helper {
			public:
				static void check(std::map<std::string, widgets::tree::tree_specs::node>& level, D2D1_POINT_2F point_, float dpi_scale_) {
					for (auto& node : level) {
						// check if marker has been clicked
						auto rect_marker = convert_rect(node.second.rc_expand);
						auto rect_node = convert_rect(node.second.rc);
						scale_RECT(rect_marker, dpi_scale_);
						scale_RECT(rect_node, dpi_scale_);

						if (point_.x >= rect_marker.left && point_.x <= rect_marker.right &&
							point_.y >= rect_marker.top && point_.y <= rect_marker.bottom)
							node.second.expand = !node.second.expand;

						if (point_.x >= rect_node.left && point_.x <= rect_node.right &&
							point_.y >= rect_node.top && point_.y <= rect_node.bottom)
							node.second.selected = true;
						else
							node.second.selected = false;

						check(node.second.children, point_, dpi_scale_);	// recursion
					}
				}
			};

			// mark selected
			helper::check(specs_.root, point_, dpi_scale_);

			// handle on_selection
			if (specs_.events().selection)
				on_selection();

			// handle on_click
			if (specs_.events().click)
				specs_.events().click();
		}

		bool widgets_impl::tree::hit(const bool& hit) {
			if (is_static_ || hit == hit_) {
				if (hit || pressed_)
					return true;
				else
					return false;
			}

			hit_ = hit;
			return true;
		}

		widgets::tree::tree_specs&
			widgets_impl::tree::specs() { return specs_; }

		widgets::tree::tree_specs&
			widgets_impl::tree::operator()() { return specs(); }

		void widgets_impl::tree::on_selection() {
			class helper {
			public:
				static void check(std::map<std::string, widgets::tree::tree_specs::node>& level,
					std::function<void(widgets::tree::tree_specs::node& n)> selection) {
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

			helper::check(specs_.root, specs_.events().selection);
		}
	}
}
