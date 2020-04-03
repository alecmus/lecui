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

class liblec::lecui::widgets::timer::timer_impl {
public:
	timer_impl(form& fm) :
		fm_(fm) {}
	form& fm_;
};

liblec::lecui::widgets::timer::timer(form& fm) :
	d_(*new timer_impl(fm)) {}

liblec::lecui::widgets::timer::~timer() { delete& d_; }

void liblec::lecui::widgets::timer::add(const std::string& name,
	const unsigned long& milliseconds, std::function<void()> on_timer) {
	if (running(name)) return;

	int unique_id = (d_.fm_.d_.timers_.find(name) == d_.fm_.d_.timers_.end()) ?
		d_.fm_.d_.make_unique_id() : d_.fm_.d_.timers_.at(name).unique_id;

	liblec::lecui::form::form_impl::timer timer_;
	timer_.milliseconds = milliseconds;
	timer_.on_timer = on_timer;
	timer_.unique_id = unique_id;

	// insert to timer map
	d_.fm_.d_.timers_[name] = timer_;

	if (IsWindow(d_.fm_.d_.hWnd_))
		d_.fm_.d_.start_timer(name);
	else
		d_.fm_.d_.timers_.at(name).running = false;	// timer will be started on window creation
}

bool liblec::lecui::widgets::timer::running(const std::string& name) {
	return (d_.fm_.d_.timers_.find(name) == d_.fm_.d_.timers_.end()) ?
		false : d_.fm_.d_.timers_.at(name).running;
}

void liblec::lecui::widgets::timer::stop(const std::string& name) {
	if (d_.fm_.d_.timers_.find(name) != d_.fm_.d_.timers_.end()) {
		log("stopping timer: " + name);
		KillTimer(d_.fm_.d_.hWnd_, (UINT_PTR)d_.fm_.d_.timers_.at(name).unique_id);
		d_.fm_.d_.timers_.at(name).running = false;
	}
}
