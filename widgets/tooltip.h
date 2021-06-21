//
// tooltip.h - tooltip interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#pragma once

#include "../containers/page.h"
#include "../controls.h"
#include "../timer.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class tooltip_form : public form {
				const std::string tooltip_text_;
				const float margin_ = 5.f;
				const std::string font_{ "Segoe UI" };
				const float font_size_ = 9.f;
				const lecui::size min_size_ = { 20.f, 20.f };
				lecui::rect rect_;
				lecui::size max_size_ = { 4200.f, 10000.f };
				const std::string al_page_home_ = "home_page";
				lecui::controls ctrls_{ *this };
				lecui::dimensions dim_{ *this };
				lecui::page_management page_man_{ *this };
				lecui::timer_management timer_man_{ *this };
				const unsigned long lifetime_;
				lecui::form& parent_;

				bool on_initialize(std::string& error) override;
				void on_start() override;
				bool on_layout(std::string& error) override;

			public:
				tooltip_form(form& parent, const std::string& text, unsigned long lifetime);
				~tooltip_form();
			};
		}
	}
}
