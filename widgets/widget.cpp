//
// widget.cpp - widget implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
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

			/// This operator is for determining whether to recreate badge resources. It doesn't
			/// represent equality in the sense of badges having the same characteristics!
			bool badge_specs::operator==(const badge_specs& param) {
				return
					/// Colors
					(_color == param._color) &&
					(_color_border == param._color_border) &&
					(_color_text == param._color_text) &&

					/// Font
					(_font == param._font) &&
					(_font_size == param._font_size);
			}

			bool badge_specs::operator!=(const badge_specs& param) {
				return !operator==(param);
			}

			std::string& badge_specs::text() { return _text; }

			badge_specs& badge_specs::text(const std::string& text) {
				_text = text;
				return *this;
			}

			std::string& badge_specs::font() { return _font; }

			badge_specs& badge_specs::font(const std::string& font) {
				_font = font;
				return *this;
			}

			float& badge_specs::font_size() { return _font_size; }

			badge_specs& badge_specs::font_size(const float& font_size) {
				_font_size = font_size;
				return *this;
			}

			float& badge_specs::border() { return _border; }

			badge_specs& badge_specs::border(const float& border) {
				_border = border;
				return *this;
			}

			lecui::color& badge_specs::color() { return _color; }

			badge_specs& badge_specs::color(const lecui::color& color) {
				_color = color;
				return *this;
			}

			lecui::color& badge_specs::color_border() { return _color_border; }

			badge_specs& badge_specs::color_border(const lecui::color& color_border) {
				_color_border = color_border;
				return *this;
			}

			lecui::color& badge_specs::color_text() { return _color_text; }

			badge_specs& badge_specs::color_text(const lecui::color& color_text) {
				_color_text = color_text;
				return *this;
			}

			badge_specs::badge_position& badge_specs::position() { return _position; }

			badge_specs& badge_specs::position(const badge_position& position) {
				_position = position;
				return *this;
			}

			bool badge_widget::operator==(const badge_widget& param) {
				return
					/// Badge
					(_badge == param._badge);
			}

			bool badge_widget::operator!=(const badge_widget& param) {
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

		bool widget_manager::enable(const std::string& path, std::string& error) {
			return _d._fm._d.enable(path, true, error);
		}

		bool widget_manager::disable(const std::string& path, std::string& error) {
			return _d._fm._d.enable(path, false, error);
		}

		bool widget_manager::show(const std::string& path, std::string& error) {
			return _d._fm._d.show(path, true, error);
		}

		bool widget_manager::hide(const std::string& path, std::string& error) {
			return _d._fm._d.show(path, false, error);
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

		bool widget_manager::refresh(const std::string& path, std::string& error) {
			return _d._fm._d.refresh(path, error);
		}
	}
}
