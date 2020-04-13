/*
** timer.cpp - timer implementation
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

#include "widgets/timer.h"
#include "form_impl.h"

namespace liblec {
	namespace lecui {
		class widgets::timer::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		widgets::timer::timer(form& fm) :
			d_(*new impl(fm)) {}

		widgets::timer::~timer() { delete& d_; }

		void widgets::timer::add(const std::string& alias,
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

		bool widgets::timer::running(const std::string& alias) {
			return (d_.fm_.d_.timers_.find(alias) == d_.fm_.d_.timers_.end()) ?
				false : d_.fm_.d_.timers_.at(alias).running;
		}

		void widgets::timer::stop(const std::string& alias) {
			if (d_.fm_.d_.timers_.find(alias) != d_.fm_.d_.timers_.end()) {
				log("stopping timer: " + alias);
				KillTimer(d_.fm_.d_.hWnd_, (UINT_PTR)d_.fm_.d_.timers_.at(alias).unique_id);
				d_.fm_.d_.timers_.at(alias).running = false;
			}
		}
	}
}
