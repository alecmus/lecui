/*
** form.h - form interface
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

#pragma once

#if defined(LECUI_EXPORTS)
	#include "lecui.h"
#else
	#include <liblec/lecui.h>
#endif

#include <functional>

namespace liblec {
	namespace lecui {
		class page;
		class dimensions;
		class appearance;
		class controls;

		namespace widgets {
			class timer;
			class label;
			class image;
		}

		enum class form_position {
			// offset members use 10 pixels

			center_to_working_area,
			center_to_parent,
			top_left,
			top_left_offset,
			bottom_left,
			bottom_left_offset,
			top_right,
			top_right_offset,
			bottom_right,
			bottom_right_offset,
		};

		class lecui_api form {
		public:
			form(const std::string& caption);
			form(const std::string& caption, form& parent);
			~form();

			void move(const liblec::lecui::point& point);
			void move(const liblec::lecui::form_position& form_position);

			bool show(std::string& error);
			void close();

			virtual bool layout(std::string& error);
			virtual void on_start();
			virtual void on_caption();
			virtual void on_close();
			virtual void on_shutdown();

			bool prompt(const std::string& question);
			void message(const std::string& message);

			std::string select_folder(const std::string& title);
			std::string open_file(const liblec::lecui::open_file_params& params);
			std::string save_file(const std::string& file,
				const liblec::lecui::save_file_params& params);

			void enable(const std::string& name, bool enable);
			void show(const std::string& name, bool show);
			void close(const std::string& name);

			void update();
			void reload();

			void dropped_files(std::function<void(const std::string& file)>on_drop_files);

		private:
			class form_impl;
			form_impl& d_;

			form();

			form(const form&);
			form& operator=(const form&);

			friend liblec::lecui::page;
			friend liblec::lecui::dimensions;
			friend liblec::lecui::appearance;
			friend liblec::lecui::controls;
			friend liblec::lecui::widgets::timer;
			friend liblec::lecui::widgets::label;
			friend liblec::lecui::widgets::image;
		};
	}
}
