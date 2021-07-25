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
			resize_params::resize_params() :
				resize_params(0.f, 0.f, 0.f, 0.f) {}

			resize_params::resize_params(const float perc_x,
				const float perc_y, const float perc_width, const float perc_height) :
				_perc_x(perc_x), _perc_y(perc_y), _perc_width(perc_width), _perc_height(perc_height) {}

			float& resize_params::perc_x() {
				return _perc_x;
			}

			float resize_params::get_perc_x() const {
				return _perc_x;
			}

			resize_params& resize_params::perc_x(const float& perc_x) {
				_perc_x = perc_x;
				return *this;
			}

			float& resize_params::perc_y() {
				return _perc_y;
			}

			float resize_params::get_perc_y() const {
				return _perc_y;
			}

			resize_params& resize_params::perc_y(const float& perc_y) {
				_perc_y = perc_y;
				return *this;
			}

			float& resize_params::perc_width() {
				return _perc_width;
			}

			float resize_params::get_perc_width() const {
				return _perc_width;
			}

			resize_params& resize_params::perc_width(const float& perc_width) {
				_perc_width = perc_width;
				return *this;
			}

			float& resize_params::perc_height() {
				return _perc_height;
			}

			float resize_params::get_perc_height() const {
				return _perc_height;
			}

			resize_params& resize_params::perc_height(const float& perc_height) {
				_perc_height = perc_height;
				return *this;
			}

			float& resize_params::min_x() {
				return _min_x;
			}

			const float resize_params::get_min_x() const {
				return _min_x;
			}

			resize_params& resize_params::min_x(const float& min_x) {
				_min_x = min_x;
				return *this;
			}

			float& resize_params::min_y() {
				return _min_y;
			}

			float resize_params::get_min_y() const {
				return _min_y;
			}

			resize_params& resize_params::min_y(const float& min_y) {
				_min_y = min_y;
				return *this;
			}

			float& resize_params::max_x() {
				return _max_x;
			}

			float resize_params::get_max_x() const {
				return _max_x;
			}

			resize_params& resize_params::max_x(const float& max_x) {
				_max_x = max_x;
				return *this;
			}

			float& resize_params::max_y() {
				return _max_y;
			}

			float resize_params::get_max_y() const {
				return _max_y;
			}

			resize_params& resize_params::max_y(const float& max_y) {
				_max_y = max_y;
				return *this;
			}

			float& resize_params::min_width() {
				return _min_width;
			}

			float resize_params::get_min_width() const {
				return _min_width;
			}

			resize_params& resize_params::min_width(const float& min_width) {
				_min_width = min_width;
				return *this;
			}

			float& resize_params::min_height() {
				return _min_height;
			}

			float resize_params::get_min_height() const {
				return _min_height;
			}

			resize_params& resize_params::min_height(const float& min_height) {
				_min_height = min_height;
				return *this;
			}

			float& resize_params::max_width() {
				return _max_width;
			}

			float resize_params::get_max_width() const {
				return _max_width;
			}

			resize_params& resize_params::max_width(const float& max_width) {
				_max_width = max_width;
				return *this;
			}

			float& resize_params::max_height() {
				return _max_height;
			}

			float resize_params::get_max_height() const {
				return _max_height;
			}

			resize_params& resize_params::max_height(const float& max_height) {
				_max_height = max_height;
				return *this;
			}

			bool resize_params::operator==(const resize_params& param) {
				return
					(_perc_x == param._perc_x) &&
					(_perc_y == param._perc_y) &&
					(_perc_width == param._perc_width) &&
					(_perc_height == param._perc_height) &&
					(_min_x == param._min_x) &&
					(_min_y == param._min_y) &&
					(_max_x == param._max_x) &&
					(_max_y == param._max_y) &&
					(_min_width == param._min_width) &&
					(_min_height == param._min_height) &&
					(_max_width == param._max_width) &&
					(_max_height == param._max_height);
			}

			bool resize_params::operator!=(const resize_params& param) {
				return !operator==(param);
			}

			/// This operator is for determining whether to recreate widget resources. It doesn't
			/// represent equality in the sense of widgets having the same characteristics!
			bool specs::operator==(const specs& param) {
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

			bool specs::operator!=(const specs& param) {
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
