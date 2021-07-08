//
// html_editor.cpp - html editor widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../html_editor.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::html_editor_specs::operator==(const html_editor_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_caret_ == param.color_caret_) &&
				(color_control_border_ == param.color_control_border_) &&
				(color_control_fill_ == param.color_control_fill_);
		}

		bool widgets::html_editor_specs::operator!=(const html_editor_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::html_editor_specs::text() { return text_; }

		widgets::html_editor_specs& widgets::html_editor_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::html_editor_specs::tooltip() { return tooltip_; }

		widgets::html_editor_specs& widgets::html_editor_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::html_editor_specs::rect() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special rectangle
				return rect_widget_overall_;	// to-do: how to transfer changes to special panes
			else
				return rect_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::rect(const lecui::rect& rect) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				rect_widget_overall_ = rect;	// to-do: how to transfer changes to special panes
			else
				rect_ = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::html_editor_specs::on_resize() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				return on_resize_overall_;	// to-do: how to transfer changes to special panes
			else
				return on_resize_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::on_resize(const resize_params& on_resize) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				on_resize_overall_ = on_resize;	// to-do: how to transfer changes to special panes
			else
				on_resize_ = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::html_editor_specs::cursor() { return cursor_; }

		widgets::html_editor_specs& widgets::html_editor_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::html_editor_specs::font() { return font_; }

		widgets::html_editor_specs& widgets::html_editor_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::html_editor_specs::font_size() { return font_size_; }

		widgets::html_editor_specs& widgets::html_editor_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::html_editor_specs::color_text() { return color_text_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::html_editor_specs::color_fill() { return color_fill_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::html_editor_specs::color_hot() { return color_hot_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::html_editor_specs::color_selected() { return color_selected_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::html_editor_specs::color_disabled() { return color_disabled_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::html_editor_specs::border() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_html_pane_specs_)->border();
			else
				return border_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::border(const float& border) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_html_pane_specs_)->border(border);
			else
				border_ = border;

			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_border() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_html_pane_specs_)->color_border();
			else
				return color_border_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_border(const color& color_border) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_html_pane_specs_)->color_border(color_border);
			else
				color_border_ = color_border;

			return *this;
		}

		float& widgets::html_editor_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::html_editor_specs& widgets::html_editor_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::html_editor_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::html_editor_specs& widgets::html_editor_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_caret() { return color_caret_; }

		widgets::html_editor_specs& widgets::html_editor_specs::color_caret(const color& color_caret) {
			color_caret_ = color_caret;
			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_control_border() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_controls_pane_specs_)->color_border();
			else
				return color_control_border_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_control_border(const color& color_control_border) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_controls_pane_specs_)->color_border(color_control_border);
			else
				color_control_border_ = color_control_border;

			return *this;
		}

		lecui::color& widgets::html_editor_specs::color_control_fill() {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_controls_pane_specs_)->color_fill();
			else
				return color_control_fill_;
		}

		widgets::html_editor_specs& widgets::html_editor_specs::color_control_fill(const color& color_control_fill) {
			if (p_controls_pane_specs_ && p_html_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_controls_pane_specs_)->color_fill(color_control_fill);
			else
				color_control_fill_ = color_control_fill;

			return *this;
		}

		class widgets::html_editor_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_html_editor(alias)) {
				specs_
					.font("Calibri")
					.font_size(11.f)
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_border))
					.color_disabled(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_disabled))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_selected))
					.color_caret(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_editor_caret))
					.color_control_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_control))
					.color_control_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::html_control_border));
			}
			containers::page& page_;
			html_editor_specs& specs_;
		};

		widgets::html_editor_builder::html_editor_builder(containers::page& page) :
			html_editor_builder(page, "") {}

		widgets::html_editor_builder::html_editor_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::html_editor_builder::~html_editor_builder() { delete& d_; }

		widgets::html_editor_specs&
			widgets::html_editor_builder::specs() {
			return d_.specs_;
		}

		widgets::html_editor_specs&
			widgets::html_editor_builder::operator()() {
			return specs();
		}

		widgets::html_editor_specs&
			widgets::html_editor_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_html_editor(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
