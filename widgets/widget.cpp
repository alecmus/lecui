//
// widget.cpp - widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "widget.h"
#include "../form_impl/form_impl.h"
#include "../utilities/timer.h"

namespace liblec {
	namespace lecui {
		namespace widgets {
			/// This operator is for determining whether to recreate widget resources. It doesn't
			/// represent equality in the sense of widgets having the same characteristics!
			bool widget::operator==(const widget& param) {
				return
					/// Colors
					(_color_text == param._color_text) &&
					(_color_fill == param._color_fill) &&
					(_color_hot == param._color_hot) &&
					(_color_selected == param._color_selected) &&
					(_color_disabled == param._color_disabled) &&

					/// Font
					(_font == param._font) &&
					(_font_size == param._font_size) &&

					/// Cursor
					(_cursor == param._cursor);
			}

			bool widget::operator!=(const widget& param) {
				return !operator==(param);
			}
		}

		class widget_manager::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		widget_manager::widget_manager(form& fm) :
			_d(*new impl(fm)) {}
		widget_manager::~widget_manager() {
			delete& _d;
		}

		void widget_manager::enable(const std::string& path, bool enable) {
			_d._fm._d.enable(path, enable);
		}

		void widget_manager::show(const std::string& path, bool show) {
			_d._fm._d.show(path, show);
		}
		void widget_manager::close(const std::string& path) {
			// use timer in case a widget is closed from its own handler.
			// this way the actual closing will be done (hopefully) outside the handler coz of async.
			// the caller still has to exercise caution by avoiding such logical errors.
			_d._fm._d._scheduled_for_closure.push_back(path);
			timer_manager(_d._fm).add("close_widget_timer", 0,
				[&]() {
					timer_manager(_d._fm).stop("close_widget_timer");
					for (const auto& it : _d._fm._d._scheduled_for_closure)
						_d._fm._d.close(it);

					_d._fm._d._scheduled_for_closure.clear();
				});
		}

		void widget_manager::select(const std::string& path) {
			_d._fm._d.select(path);
		}
	}
}
