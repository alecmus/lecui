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
		bool widgets::tree_view::tree_view_specs::operator==(const tree_view_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border_ == param.color_border_);
		}

		bool widgets::tree_view::tree_view_specs::operator!=(const tree_view_specs& param) {
			return !operator==(param);
		}


		class widgets::tree_view::impl {
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

		widgets::tree_view::tree_view(containers::page& page) :
			tree_view(page, "") {}

		widgets::tree_view::tree_view(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		widgets::tree_view::~tree_view() { delete& d_; }

		widgets::tree_view::tree_view_specs&
			widgets::tree_view::specs() {
			return d_.specs_;
		}

		widgets::tree_view::tree_view_specs&
			widgets::tree_view::operator()() {
			return specs();
		}

		widgets::tree_view::tree_view_specs&
			widgets::tree_view::specs(form& fm, const std::string& path) {
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
