//
// widget_impl.cpp - widget_impl implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "widget_impl.h"
#include "../form_impl/form_impl.h"
#include "../containers/page/page_impl.h"

namespace liblec {
	namespace lecui {
		widgets::widget_impl::widget_impl(containers::page& page, const std::string& alias) :
			_page(page),
			_alias(alias),
			_is_static(true),
			_hit(false),
			_pressed(false),
			_rect({ .0f, .0f, .0f, .0f }),
			_visible(true),
			_is_enabled(true),
			_point({ 0.f, 0.f }),
			_selected(false),
			_point_on_press({ 0.f,0.f }),
			_point_on_release({ 0.f,0.f }),
			_resources_created(false),
			_h_cursor(nullptr),
			_tooltip_active(false) {}

		widgets::widget_impl::~widget_impl() {}

		const std::string& widgets::widget_impl::alias() { return _alias; }

		bool widgets::widget_impl::contains(const D2D1_POINT_2F& point) {
			// capture the point
			_point = point;

			if (point.x == 0.f && point.y == 0.f)
				return false;

			D2D1_RECT_F rect = _rect;
			scale_RECT(rect, _page._d_page.get_dpi_scale());

			if (point.x >= rect.left && point.x <= rect.right &&
				point.y >= rect.top && point.y <= rect.bottom)
				return contains();
			else
				return false;
		}

		bool widgets::widget_impl::hit(const bool& hit) {
			if (_is_static || hit == _hit)
				return false;	// Don't refresh ui. Either widget is static or hit status hasn't changed.

			_hit = hit;

			if (_hit) {
				if (generic_specs().events().mouse_enter)
					generic_specs().events().mouse_enter();
			}
			else {
				if (generic_specs().events().mouse_leave)
					generic_specs().events().mouse_leave();
			}

			return true;
		}

		void widgets::widget_impl::press(const bool& pressed) {
			bool change = _pressed != pressed;

			_pressed = pressed;

			if (change) {
				if (_pressed)
					_point_on_press = _point;
				else
					_point_on_release = _point;
			}
		}

		bool widgets::widget_impl::pressed() { return _pressed; }
		bool widgets::widget_impl::is_static() { return _is_static; }
		void widgets::widget_impl::show(const bool& show) { _visible = show; }
		bool widgets::widget_impl::visible() { return _visible; }
		void widgets::widget_impl::enable(const bool& enable) {
			_is_enabled = enable;
		}
		bool widgets::widget_impl::enabled() { return _is_enabled; }
		const D2D1_RECT_F& widgets::widget_impl::get_rect() { return _rect; }
		bool widgets::widget_impl::selected() { return _selected; }
		bool widgets::widget_impl::hit() { return _hit; }
		HCURSOR widgets::widget_impl::cursor() { return _h_cursor; }
		float widgets::widget_impl::get_dpi_scale() { return _page._d_page.get_dpi_scale(); }
		form& widgets::widget_impl::get_form() { return _page._d_page.get_form(); }

		void widgets::widget_impl::show_tooltip() {
			// make a local copy of the tooltip text
			_tooltip_text = generic_specs().tooltip();

			// trim leading and trailing whitespace
			trim(_tooltip_text);

			// empty tooltips not allowed
			if (_tooltip_text.empty())
				return;

			// failsafe
			if (get_form()._d._p_tooltip_form.get())
				return;

			log("show_tooltip: " + _tooltip_text);

			_tooltip_active = true;

			// create tooltip object
			get_form()._d._p_tooltip_form = std::unique_ptr<tooltip_form>(new tooltip_form(get_form(), _tooltip_text, 5000));

			// display the tooltip
			std::string error;
			if (!get_form()._d._p_tooltip_form->create(error)) {}

			// delete tooltip object
			get_form()._d._p_tooltip_form.reset(nullptr);

			_tooltip_active = false;
		}

		void widgets::widget_impl::hide_tooltip() {
			// close the tooltip
			if (_tooltip_active) {
				log("hide_tooltip: " + _tooltip_text);

				// deletion of the tooltip object will be done after the blocking show method returns
				if (get_form()._d._p_tooltip_form.get())
					get_form()._d._p_tooltip_form->close();
			}
		}

		void widgets::widget_impl::create_badge_resources(badge_specs& badge,
			ID2D1HwndRenderTarget* p_render_target,
			IDWriteFactory* p_directwrite_factory,
			badge_resources& resources) {
			HRESULT hr = S_OK;

			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(badge.color()),
					&resources.p_brush_badge);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(badge.color_border()),
					&resources.p_brush_badge_border);
			if (SUCCEEDED(hr))
				hr = p_render_target->CreateSolidColorBrush(convert_color(badge.color_text()),
					&resources.p_brush_badge_text);
			if (SUCCEEDED(hr)) {
				// Create a DirectWrite text format object.
				hr = p_directwrite_factory->CreateTextFormat(
					convert_string(badge.font()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_NORMAL,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					convert_fontsize_to_dip(badge.font_size()),
					L"", //locale
					&resources.p_text_format_badge
				);
			}
			if (SUCCEEDED(hr)) {
				resources.p_text_format_badge->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				resources.p_text_format_badge->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				make_single_line(p_directwrite_factory, resources.p_text_format_badge);
			}
		}

		void widgets::widget_impl::draw_badge(badge_specs& badge,
			D2D1_RECT_F rect,
			ID2D1HwndRenderTarget* p_render_target,
			IDWriteFactory* p_directwrite_factory,
			badge_resources& resources) {
			if (!badge.text().empty()) {
				// draw badge

				// measure size of badge text
				auto _rect_badge = measure_text(p_directwrite_factory, badge.text(), badge.font(),
					badge.font_size(), text_alignment::center, true, true, false, rect);

				// add padding
				const float padding = (_rect_badge.bottom - _rect_badge.top) / 4.f;
				_rect_badge.left -= padding;
				_rect_badge.right += padding;

				// force width to be at least as great as the height
				auto width = _rect_badge.right - _rect_badge.left;
				auto height = _rect_badge.bottom - _rect_badge.top;

				if (width < height)
					_rect_badge.right = _rect_badge.left + height;

				// don't allow width and height to be greater than the widget
				if (width > rect.right - rect.left)
					_rect_badge.right = _rect_badge.left + (rect.right - rect.left);

				if (height > rect.bottom - rect.top)
					_rect_badge.bottom = _rect_badge.top + (rect.bottom - rect.top);

				// position badge rectangle
				switch (badge.position()) {
				case widgets::badge_specs::badge_position::top_left:
					pos_rect(rect, _rect_badge, 0.f, 0.f);
					break;
				case widgets::badge_specs::badge_position::top_right:
					pos_rect(rect, _rect_badge, 100.f, 0.f);
					break;
				case widgets::badge_specs::badge_position::bottom_left:
					pos_rect(rect, _rect_badge, 0.f, 100.f);
					break;
				default:
				case widgets::badge_specs::badge_position::bottom_right:
					pos_rect(rect, _rect_badge, 100.f, 100.f);
					break;
				}

				// make sides rounded enough to turn a square into a circle
				D2D1_ROUNDED_RECT _rect_badge_rounded{ _rect_badge,
					height / 2.f, height / 2.f };

				// draw the badge background
				p_render_target->FillRoundedRectangle(_rect_badge_rounded, resources.p_brush_badge);

				// create a text layout
				IDWriteTextLayout* p_text_layout_badge = nullptr;

				HRESULT hr = p_directwrite_factory->CreateTextLayout(convert_string(badge.text()).c_str(),
					(UINT32)badge.text().length(), resources.p_text_format_badge, _rect_badge.right - _rect_badge.left,
					_rect_badge.bottom - _rect_badge.top, &p_text_layout_badge);

				if (SUCCEEDED(hr)) {
					// draw the text layout
					p_render_target->DrawTextLayout(D2D1_POINT_2F{ _rect_badge.left, _rect_badge.top },
						p_text_layout_badge, resources.p_brush_badge_text, D2D1_DRAW_TEXT_OPTIONS_CLIP);
				}

				// release the text layout
				safe_release(&p_text_layout_badge);

				// draw the badge border
				p_render_target->DrawRoundedRectangle(_rect_badge_rounded, resources.p_brush_badge_border, badge.border());
			}
		}

		void widgets::widget_impl::discard_badge_resources(badge_resources& resources) {
			safe_release(&resources.p_brush_badge);
			safe_release(&resources.p_brush_badge_border);
			safe_release(&resources.p_brush_badge_text);
			safe_release(&resources.p_text_format_badge);
		}

		void widgets::widget_impl::on_click() {
			if (generic_specs().events().click)
				generic_specs().events().click();

			if (type() != widgets::widget_type::html_editor &&
				type() != widgets::widget_type::text_field &&
				type() != widgets::widget_type::combobox) {
				if (generic_specs().events().action)
					generic_specs().events().action();
			}
		}

		void widgets::widget_impl::on_right_click() {
			if (generic_specs().events().right_click)
				generic_specs().events().right_click();
		}

		void widgets::widget_impl::on_action() {
			if (generic_specs().events().action)
				generic_specs().events().action();
		}

		bool widgets::widget_impl::on_mousewheel(float units) { return false; }
		bool widgets::widget_impl::on_keydown(WPARAM wParam) { return false; }
		void widgets::widget_impl::on_selection_change(const bool& selected) {}
		bool widgets::widget_impl::contains() { return true; }

		void widgets::widget_impl::select(const bool& selected) {
			bool change = _selected != selected;
			_selected = selected;

			if (change)
				on_selection_change(_selected);
		}

		D2D1_RECT_F
			widgets::widget_impl::position(const rect& rect,
				const resize_params& on_resize,
				const float& change_in_width, const float& change_in_height) {
			auto x_change = change_in_width * on_resize.get_x_rate() / 100.f;
			auto y_change = change_in_height * on_resize.get_y_rate() / 100.f;
			auto width_change =
				change_in_width * on_resize.get_width_rate() / 100.f;
			auto height_change =
				change_in_height * on_resize.get_height_rate() / 100.f;

			auto left = rect.get_left() + x_change;
			auto top = rect.get_top() + y_change;
			auto right = rect.get_right() + x_change + width_change;
			auto bottom = rect.get_bottom() + y_change + height_change;

			// apply limits on width and height

			// width
			if ((right - left) < on_resize.get_min_width())
				right = left + on_resize.get_min_width();

			if (on_resize.get_max_width() > on_resize.get_min_width())
				if ((right - left) > on_resize.get_max_width())
					right = left + on_resize.get_max_width();

			// height
			if ((bottom - top) < on_resize.get_min_height())
				bottom = top + on_resize.get_min_height();

			if (on_resize.get_max_height() > on_resize.get_min_height())
				if ((bottom - top) > on_resize.get_max_height())
					bottom = top + on_resize.get_max_height();

			const auto width = right - left;
			const auto height = bottom - top;

			// apply limits on position

			// x

			if (left < on_resize.get_min_x()) {
				left = on_resize.get_min_x();
				right = left + width;
			}

			if (on_resize.get_max_x() > on_resize.get_min_x()) {
				if (left > on_resize.get_max_x()) {
					left = on_resize.get_max_x();
					right = left + width;
				}
			}

			// y

			if (top < on_resize.get_min_y()) {
				top = on_resize.get_min_y();
				bottom = top + height;
			}

			if (on_resize.get_max_y() > on_resize.get_min_y()) {
				if (top > on_resize.get_max_y()) {
					top = on_resize.get_max_y();
					bottom = top + height;
				}
			}

			return D2D1::RectF(left, top, right, bottom);
		}
	}
}
