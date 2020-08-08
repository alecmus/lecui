/*
** instance.cpp - instance implementation
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

#include "instance.h"
#include "form_impl/form_impl.h"

namespace liblec {
	namespace lecui {
		class instance_management::impl {
		public:
			impl(form& fm, const std::string& guid) :
				fm_(fm) {
				if (!guid.empty()) {
					// capture form guid
					if (fm.d_.guid_.empty())
						fm.d_.guid_ = guid;

					// for limiting the number of instances of the app
					if (!fm.d_.p_instance_)
						fm.d_.p_instance_ = new limit_single_instance(guid);
				}
			}
			form& fm_;
		};

		instance_management::instance_management(form& fm, const std::string& guid) : d_(*(new impl(fm, guid))) {}
		instance_management::~instance_management() { delete& d_; }
	}
}
