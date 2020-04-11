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

		namespace containers {
			class tab_control;
			class tab;
			class pane;
		}

		namespace widgets {
			class timer;
			class label;
			class image;
			class button;
			class combo;
			class custom;
			class group;
			class list;
			class rectangle;
			class toggle;
			class progress_indicator;
			class progress_bar;
			class checkbox;
			class textbox;
			class tree;
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

			void move(const point& point);
			void move(const form_position& form_position);

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
			std::string open_file(const open_file_params& params);
			std::string save_file(const std::string& file,
				const save_file_params& params);

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

			friend page;
			friend dimensions;
			friend appearance;
			friend controls;
			friend containers::tab_control;
			friend containers::tab;
			friend containers::pane;
			friend widgets::timer;
			friend widgets::label;
			friend widgets::image;
			friend widgets::button;
			friend widgets::combo;
			friend widgets::custom;
			friend widgets::group;
			friend widgets::list;
			friend widgets::rectangle;
			friend widgets::toggle;
			friend widgets::progress_indicator;
			friend widgets::progress_bar;
			friend widgets::checkbox;
			friend widgets::textbox;
			friend widgets::tree;
		};
	}
}
