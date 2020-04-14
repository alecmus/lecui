/*
** tree.cpp - tree widget implementation
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

#include "../tree.h"
#include "../../form_impl.h"

namespace liblec {
	namespace lecui {
		bool widgets::tree_specs::operator==(const tree_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border);
		}

		bool widgets::tree_specs::operator!=(const tree_specs& param) {
			return !operator==(param);
		}


		class widgets::tree::impl {
		public:
			impl(containers::page& page) :
				page_(page) {}
			containers::page& page_;
		};

		widgets::tree::tree(containers::page& page) :
			d_(*(new impl(page))) {}

		widgets::tree::~tree() { delete& d_; }

		widgets::tree_specs&
			widgets::tree::add(const std::string& alias) {
			// note: this placement is only temporary. the next time page::show() is called
			// this tree widget will be moved into a special pane
			// this is important so we don't have to manually handle scroll bar issues in tree_impl.
			// all we need to do for scroll bars is dynamically change specs_.rect as the tree changes
			// and the pane will do all the scrolling for us
			return d_.page_.d_page_.add_tree(alias);
		}

		widgets::tree_specs&
			widgets::tree::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				auto& page = fm.d_.p_pages_.at(page_alias);
				auto results = fm.d_.find_widget(page, path_remaining);
				return results.page.d_page_.get_tree(results.widget.alias()).specs();
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
