//
// tooltip.h - tooltip interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#include "../containers/page.h"
#include "../controls.h"
#include "../utilities/timer.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			class tooltip_form : public form {
				const std::string _tooltip_text;
				const float _margin = 5.f;
				const std::string _font{ "Segoe UI" };
				const float _font_size = 9.f;
				const lecui::size _min_size = { 20.f, 20.f };
				lecui::rect _rect;
				lecui::size _max_size = { 4200.f, 10000.f };
				const std::string _al_page_home = "home_page";
				lecui::controls _ctrls{ *this };
				lecui::dimensions _dim{ *this };
				lecui::page_manager _page_man{ *this };
				lecui::timer_manager _timer_man{ *this };
				const unsigned long _lifetime;
				lecui::form& _parent;

				bool on_initialize(std::string& error);
				bool on_layout(std::string& error);
				void on_start();

			public:
				tooltip_form(form& parent, const std::string& text, unsigned long lifetime);
				~tooltip_form();
			};
		}
	}
}
