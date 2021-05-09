//
// group.cpp - group container implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../group.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		bool containers::group::group_specs::operator==(const group_specs& param) {
			return
				// generic specs
				specs::operator==(param) &&

				// widget specific specs
				(color_border == param.color_border);
		}

		bool containers::group::group_specs::operator!=(const group_specs& param) {
			return !operator==(param);
		}

		class containers::group::impl {
		public:
			impl(containers::page& page, const std::string& alias) :
				page_(page),
				specs_(page_.d_page_.add_group(alias)) {
				specs_.color_fill = defaults::color(page_.d_page_.fm_.d_.theme_, item::group);
				specs_.color_border = defaults::color(page_.d_page_.fm_.d_.theme_, item::group_border);
			}
			containers::page& page_;
			group_specs& specs_;
		};

		containers::group::group(containers::page& page) :
			group(page, "") {}

		containers::group::group(containers::page& page, const std::string& alias) :
			d_(*(new impl(page, alias))) {}

		containers::group::~group() { delete& d_; }

		containers::group::group_specs&
			containers::group::specs() {
			return d_.specs_;
		}

		containers::group::group_specs&
			containers::group::operator()() {
			return specs();
		}

		containers::group::group_specs&
			containers::group::specs(form& fm, const std::string& path) {
			const auto idx = path.find("/");

			if (idx != std::string::npos) {
				const auto page_alias = path.substr(0, idx);
				const auto path_remaining = path.substr(idx + 1);
				try {
					// check form pages
					auto& page = fm.d_.p_pages_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
				try {
					// check status panes
					auto& page = fm.d_.p_status_panes_.at(page_alias);
					auto results = fm.d_.find_widget(page, path_remaining);
					return results.page.d_page_.get_group(results.widget.alias()).specs();
				}
				catch (const std::exception&) {}
			}

			throw std::invalid_argument("Invalid path");
		}
	}
}
