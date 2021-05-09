//
// timer.cpp - timer implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "timer.h"
#include "form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class timer_management::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		timer_management::timer_management(form& fm) :
			d_(*new impl(fm)) {}

		timer_management::~timer_management() { delete& d_; }

		void timer_management::add(const std::string& alias,
			const unsigned long& milliseconds, std::function<void()> on_timer) {
			if (running(alias)) return;

			int unique_id = (d_.fm_.d_.timers_.find(alias) == d_.fm_.d_.timers_.end()) ?
				d_.fm_.d_.make_unique_id() : d_.fm_.d_.timers_.at(alias).unique_id;

			form::impl::timer timer_;
			timer_.milliseconds = milliseconds;
			timer_.on_timer = on_timer;
			timer_.unique_id = unique_id;

			// insert to timer map
			d_.fm_.d_.timers_[alias] = timer_;

			if (IsWindow(d_.fm_.d_.hWnd_))
				d_.fm_.d_.start_timer(alias);
			else
				d_.fm_.d_.timers_.at(alias).running = false;	// timer will be started on form creation
		}

		bool timer_management::running(const std::string& alias) {
			return (d_.fm_.d_.timers_.find(alias) == d_.fm_.d_.timers_.end()) ?
				false : d_.fm_.d_.timers_.at(alias).running;
		}

		void timer_management::stop(const std::string& alias) {
			if (d_.fm_.d_.timers_.find(alias) != d_.fm_.d_.timers_.end()) {
				KillTimer(d_.fm_.d_.hWnd_, (UINT_PTR)d_.fm_.d_.timers_.at(alias).unique_id);
				d_.fm_.d_.timers_.at(alias).running = false;
			}
		}
	}
}
