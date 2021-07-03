//
// slider.cpp - slider widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../slider.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::slider_specs::operator==(const slider_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_) &&
				(color_tick_ == param.color_tick_) &&
				(color_knob_ == param.color_knob_) &&
				(color_knob_hot_ == param.color_knob_hot_) &&
				(color_knob_border_ == param.color_knob_border_);
		}

		bool widgets::slider_specs::operator!=(const slider_specs& param) {
			return !operator==(param);
		}

		std::string& widgets::slider_specs::text() { return text_; }

		widgets::slider_specs& widgets::slider_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::slider_specs::tooltip() { return tooltip_; }

		widgets::slider_specs& widgets::slider_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::slider_specs::rect() { return rect_; }

		widgets::slider_specs& widgets::slider_specs::rect(const lecui::rect& rect) {
			rect_ = rect;
			return *this;
		}

		widgets::specs::resize_params& widgets::slider_specs::on_resize() { return on_resize_; }

		widgets::slider_specs& widgets::slider_specs::on_resize(const resize_params& on_resize) {
			on_resize_ = on_resize;
			return *this;
		}

		widgets::specs::cursor_type& widgets::slider_specs::cursor() { return cursor_; }

		widgets::slider_specs& widgets::slider_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::slider_specs::font() { return font_; }

		widgets::slider_specs& widgets::slider_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::slider_specs::font_size() { return font_size_; }

		widgets::slider_specs& widgets::slider_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::slider_specs::color_text() { return color_text_; }

		widgets::slider_specs& widgets::slider_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::slider_specs::color_fill() { return color_fill_; }

		widgets::slider_specs& widgets::slider_specs::color_fill(const color& color_fill) {
			color_fill_ = color_fill;
			return *this;
		}

		color& widgets::slider_specs::color_hot() { return color_hot_; }

		widgets::slider_specs& widgets::slider_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::slider_specs::color_selected() { return color_selected_; }

		widgets::slider_specs& widgets::slider_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::slider_specs::color_disabled() { return color_disabled_; }

		widgets::slider_specs& widgets::slider_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::slider_specs::border() { return border_; }

		widgets::slider_specs& widgets::slider_specs::border(const float& border) {
			border_ = border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_border() { return color_border_; }

		widgets::slider_specs& widgets::slider_specs::color_border(const color& color_border) {
			color_border_ = color_border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob() { return color_knob_; }

		widgets::slider_specs& widgets::slider_specs::color_knob(const color& color_knob) {
			color_knob_ = color_knob;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob_hot() { return color_knob_hot_; }

		widgets::slider_specs& widgets::slider_specs::color_knob_hot(const color& color_knob_hot) {
			color_knob_hot_ = color_knob_hot;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_knob_border() { return color_knob_border_; }

		widgets::slider_specs& widgets::slider_specs::color_knob_border(const color& color_knob_border) {
			color_knob_border_ = color_knob_border;
			return *this;
		}

		lecui::color& widgets::slider_specs::color_tick() { return color_tick_; }

		widgets::slider_specs& widgets::slider_specs::color_tick(const color& color_tick) {
			color_tick_ = color_tick;
			return *this;
		}

		bool& widgets::slider_specs::snap_to_ticks() { return snap_to_ticks_; }

		widgets::slider_specs& widgets::slider_specs::snap_to_ticks(const bool& snap_to_ticks) {
			snap_to_ticks_ = snap_to_ticks;
			return *this;
		}

		bool& widgets::slider_specs::show_tick_marks() { return show_tick_marks_; }

		widgets::slider_specs& widgets::slider_specs::show_tick_marks(const bool& show_tick_marks) {
			show_tick_marks_ = show_tick_marks;
			return *this;
		}

		bool& widgets::slider_specs::show_tick_labels() { return show_tick_labels_; }

		widgets::slider_specs& widgets::slider_specs::show_tick_labels(const bool& show_tick_labels) {
			show_tick_labels_ = show_tick_labels;
			return *this;
		}

		widgets::slider_specs::slider_range& widgets::slider_specs::range() { return range_; }

		widgets::slider_specs& widgets::slider_specs::range(const slider_range& range) {
			range_ = range;
			return *this;
		}

		float& widgets::slider_specs::knob_radius() { return knob_radius_; }

		widgets::slider_specs& widgets::slider_specs::knob_radius(const float& knob_radius) {
			knob_radius_ = knob_radius;
			return *this;
		}

		float& widgets::slider_specs::slide_thickness() { return slide_thickness_; }

		widgets::slider_specs& widgets::slider_specs::slide_thickness(const float& slide_thickness) {
			slide_thickness_ = slide_thickness;
			return *this;
		}

		float& widgets::slider_specs::major_tick_unit() { return major_tick_unit_; }

		widgets::slider_specs& widgets::slider_specs::major_tick_unit(const float& major_tick_unit) {
			major_tick_unit_ = major_tick_unit;
			return *this;
		}

		long& widgets::slider_specs::minor_tick_count() { return minor_tick_count_; }

		widgets::slider_specs& widgets::slider_specs::minor_tick_count(const long& minor_tick_count) {
			minor_tick_count_ = minor_tick_count;
			return *this;
		}

		float& widgets::slider_specs::value() { return value_; }

		widgets::slider_specs& widgets::slider_specs::value(const float& value) {
			value_ = value;
			return *this;
		}

		class widgets::slider_builder::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_slider(alias)) {
				specs_
					.font_size(7.f)
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_border))
					.color_knob(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob))
					.color_knob_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_hot))
					.color_knob_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_knob_border))
					.color_tick(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_tick))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::slider_selected));
			}
			containers::page& page_;
			slider_specs& specs_;
		};

		widgets::slider_builder::slider_builder(containers::page& page) :
			slider_builder(page, "") {}

		widgets::slider_builder::slider_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::slider_builder::~slider_builder() { delete& d_; }

		widgets::slider_specs&
			widgets::slider_builder::specs() {
			return d_.specs_;
		}

		widgets::slider_specs&
			widgets::slider_builder::operator()() {
			return specs();
		}

		widgets::slider_specs&
			widgets::slider_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_slider(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
