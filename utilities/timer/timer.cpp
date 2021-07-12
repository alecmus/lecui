//
// timer.cpp - timer implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "../timer.h"
#include "../../form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class timer_manager::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		timer_manager::timer_manager(form& fm) :
			_d(*new impl(fm)) {}

		timer_manager::~timer_manager() { delete& _d; }

		void timer_manager::add(const std::string& alias,
			const unsigned long& milliseconds, std::function<void()> on_timer) {
			if (running(alias)) return;

			int unique_id = (_d._fm._d._timers.find(alias) == _d._fm._d._timers.end()) ?
				_d._fm._d.make_unique_id() : _d._fm._d._timers.at(alias).unique_id;

			form::impl::timer _timer;
			_timer.milliseconds = milliseconds;
			_timer.on_timer = on_timer;
			_timer.unique_id = unique_id;

			// insert to timer map
			_d._fm._d._timers[alias] = _timer;

			if (IsWindow(_d._fm._d._hWnd))
				_d._fm._d.start_timer(alias);
			else
				_d._fm._d._timers.at(alias).running = false;	// timer will be started on form creation
		}

		bool timer_manager::running(const std::string& alias) {
			return (_d._fm._d._timers.find(alias) == _d._fm._d._timers.end()) ?
				false : _d._fm._d._timers.at(alias).running;
		}

		void timer_manager::stop(const std::string& alias) {
			if (_d._fm._d._timers.find(alias) != _d._fm._d._timers.end()) {
				KillTimer(_d._fm._d._hWnd, (UINT_PTR)_d._fm._d._timers.at(alias).unique_id);
				_d._fm._d._timers.at(alias).running = false;
			}
		}
	}
}
