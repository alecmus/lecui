//
// tree_view.cpp - tree widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../tree_view.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::tree_view_specs::operator==(const tree_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::tree_view_specs::operator!=(const tree_view_specs& param) {
			return !operator==(param);
		}

		widgets::tree_view_specs& widgets::tree_view_specs::operator=(const tree_view_specs& right) {
			// generic specs
			text_ = right.text_;
			tooltip_ = right.tooltip_;
			rect_ = right.rect_;
			on_resize_ = right.on_resize_;
			cursor_ = right.cursor_;
			font_ = right.font_;
			font_size_ = right.font_size_;
			color_text_ = right.color_text_;
			color_fill_ = right.color_fill_;
			color_hot_ = right.color_hot_;
			color_selected_ = right.color_selected_;
			color_disabled_ = right.color_disabled_;

			// events
			tree_events_ = right.tree_events_;

			// widget specific specs
			border_ = right.border_;
			color_border_ = right.color_border_;
			corner_radius_x_ = right.corner_radius_x_;
			corner_radius_y_ = right.corner_radius_y_;
			root_ = right.root_;

			// NOT copied (alias_, p_special_pane_specs_)

			return *this;
		}

		widgets::tree_view_specs::tree_view_specs(const tree_view_specs& right) {
			*this = right;
		}

		std::string& widgets::tree_view_specs::text() { return text_; }

		widgets::tree_view_specs& widgets::tree_view_specs::text(const std::string& text) {
			text_ = text;
			return *this;
		}

		std::string& widgets::tree_view_specs::tooltip() { return tooltip_; }

		widgets::tree_view_specs& widgets::tree_view_specs::tooltip(const std::string& tooltip) {
			tooltip_ = tooltip;
			return *this;
		}

		lecui::rect& widgets::tree_view_specs::rect() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect();
			else
				return rect_;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::rect(const lecui::rect& rect) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->rect(rect);
			else
				rect_ = rect;

			return *this;
		}

		widgets::specs::resize_params& widgets::tree_view_specs::on_resize() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize();
			else
				return on_resize_;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::on_resize(const resize_params& on_resize) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->on_resize(on_resize);
			else
				on_resize_ = on_resize;

			return *this;
		}

		widgets::specs::cursor_type& widgets::tree_view_specs::cursor() { return cursor_; }

		widgets::tree_view_specs& widgets::tree_view_specs::cursor(const cursor_type cursor) {
			cursor_ = cursor;
			return *this;
		}

		std::string& widgets::tree_view_specs::font() { return font_; }

		widgets::tree_view_specs& widgets::tree_view_specs::font(const std::string& font) {
			font_ = font;
			return *this;
		}

		float& widgets::tree_view_specs::font_size() { return font_size_; }

		widgets::tree_view_specs& widgets::tree_view_specs::font_size(const float& font_size) {
			font_size_ = font_size;
			return *this;
		}

		color& widgets::tree_view_specs::color_text() { return color_text_; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_text(const color& color_text) {
			color_text_ = color_text;
			return *this;
		}

		color& widgets::tree_view_specs::color_fill() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->color_fill();
			else
				return color_fill_;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::color_fill(const color& color_fill) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->color_fill(color_fill);
			else
				color_fill_ = color_fill;

			return *this;
		}

		color& widgets::tree_view_specs::color_hot() { return color_hot_; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_hot(const color& color_hot) {
			color_hot_ = color_hot;
			return *this;
		}

		color& widgets::tree_view_specs::color_selected() { return color_selected_; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_selected(const color& color_selected) {
			color_selected_ = color_selected;
			return *this;
		}

		color& widgets::tree_view_specs::color_disabled() { return color_disabled_; }

		widgets::tree_view_specs& widgets::tree_view_specs::color_disabled(const color& color_disabled) {
			color_disabled_ = color_disabled;
			return *this;
		}

		float& widgets::tree_view_specs::border() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->border();
			else
				return border_;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::border(const float& border) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->border(border);
			else
				border_ = border;

			return *this;
		}

		lecui::color& widgets::tree_view_specs::color_border() {
			if (p_special_pane_specs_)	// redirect to special pane
				return reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->color_border();
			else
				return color_border_;
		}

		widgets::tree_view_specs& widgets::tree_view_specs::color_border(const color& color_border) {
			if (p_special_pane_specs_)	// redirect to special pane
				reinterpret_cast<lecui::containers::pane_specs*>(p_special_pane_specs_)->color_border(color_border);
			else
				color_border_ = color_border;

			return *this;
		}

		float& widgets::tree_view_specs::corner_radius_x() { return corner_radius_x_; }

		widgets::tree_view_specs& widgets::tree_view_specs::corner_radius_x(const float& corner_radius_x) {
			corner_radius_x_ = corner_radius_x;
			return *this;
		}

		float& widgets::tree_view_specs::corner_radius_y() { return corner_radius_y_; }

		widgets::tree_view_specs& widgets::tree_view_specs::corner_radius_y(const float& corner_radius_y) {
			corner_radius_y_ = corner_radius_y;
			return *this;
		}

		std::map<std::string, widgets::tree_view_specs::node>& widgets::tree_view_specs::root() { return root_; }

		widgets::tree_view_specs& widgets::tree_view_specs::root(const std::map<std::string, widgets::tree_view_specs::node>& root) {
			root_ = root;
			return *this;
		}

		class widgets::tree_view_builder::impl {
		public:
			// note: this placement is only temporary. the next time WM_PAINT is called
			// this tree widget will be moved into a special pane
			// this is important so we don't have to manually handle scroll bar issues in tree_view_impl.
			// all we need to do for scroll bars is dynamically change specs_.rect as the tree changes
			// and the pane will do all the scrolling for us
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_tree(alias)) {
				specs_
					.color_text(defaults::color(page_.d_page_.fm_.d_.theme_, item::label))
					.color_fill(defaults::color(page_.d_page_.fm_.d_.theme_, item::tree_view))
					.color_border(defaults::color(page_.d_page_.fm_.d_.theme_, item::tree_view_border))
					.color_hot(defaults::color(page_.d_page_.fm_.d_.theme_, item::tree_view_hover))
					.color_selected(defaults::color(page_.d_page_.fm_.d_.theme_, item::tree_view_selected));
			}
			containers::page& page_;
			tree_view_specs& specs_;
		};

		widgets::tree_view_builder::tree_view_builder(containers::page& page) :
			tree_view_builder(page, "") {}

		widgets::tree_view_builder::tree_view_builder(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::tree_view_builder::~tree_view_builder() { delete& d_; }

		widgets::tree_view_specs&
			widgets::tree_view_builder::specs() {
			return d_.specs_;
		}

		widgets::tree_view_specs&
			widgets::tree_view_builder::operator()() {
			return specs();
		}

		widgets::tree_view_specs&
			widgets::tree_view_builder::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tree(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_tree(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
