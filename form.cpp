//
// form.cpp - form implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include "form.h"
#include "appearance.h"
#include "controls.h"
#include "timer.h"
#include "form_impl/form_impl.h"
#include "error/win_error.h"

// Windows headers
#include <Windows.h>

namespace liblec {
	namespace lecui {
		// this is the constructor that all the others below call
		form::form(const std::string& caption) :
			d_(*new impl(*this, caption)) {}

		form::form(const std::string& caption, form& parent) :
			form::form(caption) {

			// copy the parent's default theme setting
			appearance aprnc(*this);
			aprnc.theme(parent.d_.theme_);

			if (IsWindow(parent.d_.hWnd_)) {
				// capture parent
				d_.p_parent_ = &parent;
				d_.hWnd_parent_ = parent.d_.hWnd_;

				// this is a child window. add it to the parent's map of children.
				parent.d_.m_children_.insert(std::make_pair(this, this));
			}
		}

		form::~form() {
			if (d_.hWnd_parent_ && d_.p_parent_) {
				// this is a child window. remove it from the parent's map of children
				try { d_.p_parent_->d_.m_children_.erase(this); }
				catch (const std::exception&) {}
			}

			delete& d_;
		}

		std::string form::menu_form_caption() {
			return std::string("lecui::form::menu");
		}

		void form::move(const point& point) {
			d_.user_pos_ = true;
			d_.preset_pos_ = false;
			d_.point_ = point;
		}

		void form::move(const form_position& form_position) {
			d_.user_pos_ = true;
			d_.preset_pos_ = true;
			d_.form_position_ = form_position;
		}

		bool form::show(std::string& error) {
			if (d_.show_called_) {
				error = "Library usage error: form::show";
				return false;
			}
			else {
				d_.show_called_ = true;

				if (!d_.initialized_) {
					error = "Library initialization error: form::show";
					return false;
				}
			}

			if (d_.p_instance_) {
				if (d_.p_instance_->another_instance_running()) {
					log("another instance running");
					d_.open_existing_instance();
					return true;
				}
			}

			// get resource module handle
			if (!d_.resource_dll_filename_.empty()) {
				d_.resource_module_handle_ =
					LoadLibrary(convert_string(d_.resource_dll_filename_).c_str());

				if (!d_.resource_module_handle_) {
					// LoadLibrary failed
					error = "Loading " + d_.resource_dll_filename_ + " failed: " + get_last_error();
					return false;
				}
			}
			else
				d_.resource_module_handle_ = GetModuleHandle(nullptr);

			// call the on_layout virtual function
			if (!on_layout(error))
				return false;

			// handle menu form
			if (d_.menu_form_) {
				d_.allow_resizing_ = false;
				d_.allow_minimize_ = false;
				d_.activate_ = false;
			}

			// create form controls
			if (!d_.menu_form_) {
				d_.create_close_button([&]() { on_close(); });

				if (d_.allow_resizing_)
					d_.create_maximize_button();

				if (d_.allow_minimize_)
					d_.create_minimize_button();

				d_.create_form_caption();
				d_.create_form_menu();
			}

			// register window class
			WNDCLASSEX wcex = { 0 };
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.lpfnWndProc = d_.window_procedure;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wcex.hInstance = GetModuleHandle(nullptr);

			// load main application icon
			if (d_.idi_icon_)
				wcex.hIcon = LoadIcon(d_.resource_module_handle_, MAKEINTRESOURCE(d_.idi_icon_));

			// load small application icon
			if (d_.idi_icon_small_)
				wcex.hIconSm = (HICON)LoadImage(d_.resource_module_handle_,
					MAKEINTRESOURCE(d_.idi_icon_small_),
					IMAGE_ICON,
					GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
					LR_DEFAULTCOLOR
					);

			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = nullptr;
			wcex.lpszClassName = L"liblec::lecui::form";
			RegisterClassEx(&wcex);

			if (d_.user_pos_) {
				if (d_.preset_pos_)
					d_.set_position(d_.form_position_, d_.size_.width, d_.size_.height);
				else
					d_.set_position(d_.point_.x, d_.point_.y, d_.size_.width, d_.size_.height);
			}
			else {
				if (IsWindow(d_.hWnd_parent_) && IsWindowEnabled(d_.hWnd_parent_))
					d_.set_position(form_position::center_to_parent,
						d_.size_.width, d_.size_.height);
				else
					d_.set_position(form_position::center_to_working_area,
						d_.size_.width, d_.size_.height);
			}

			// Register this instance so other instances can find this form and open it
			if (!d_.guid_.empty() && !IsWindow(d_.hWnd_parent_))
				d_.reg_id_ = RegisterWindowMessageA(d_.guid_.c_str());

			// perform initialization (d_.hWnd_ will be captured in WM_CREATE)
			if (!CreateWindowEx(d_.top_most_ == true ? WS_EX_TOPMOST : NULL, wcex.lpszClassName,
				convert_string(d_.caption_plain_).c_str(),
				static_cast<DWORD>(form::impl::style::aero_borderless),
				static_cast<int>(.5f + d_.point_.x * d_.dpi_scale_),
				static_cast<int>(.5f + d_.point_.y * d_.dpi_scale_),
				static_cast<int>(.5f + d_.size_.width * d_.dpi_scale_),
				static_cast<int>(.5f + d_.size_.height * d_.dpi_scale_),
				d_.hWnd_parent_, nullptr, wcex.hInstance, this)) {
				error = get_last_error();
				return false;
			}

			ShowWindow(d_.hWnd_, SW_SHOW);
			UpdateWindow(d_.hWnd_);

			// Update the user interface. This is important for widgets that are moved into
			// special panes. If this is not done the widgets will not be drawn fully. One
			// typical symptom of this is that before the mouse is moved at least once over an
			// active widget the drawing will be incomplete, e.g. the selected text in comboboxes
			// won't initially be displayed.
			update();

			if (d_.activate_)
				SetForegroundWindow(d_.hWnd_);

			// Disable parent if this is not a menu form.
			// If this is a menu form, do not disable parent because we will need to be
			// able to click the parent. The parent, is responsible for ignoring mouse movements
			// and such things when it detects that a child menu form is open
			if (!d_.menu_form_ && IsWindow(d_.hWnd_parent_) && IsWindowEnabled(d_.hWnd_parent_))
				EnableWindow(d_.hWnd_parent_, FALSE);

			MSG msg = {};

			// main message loop
			while (GetMessage(&msg, nullptr, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (d_.parent_closing_)
				PostQuitMessage(0);

			return true;
		}

		void form::close() {
			if (IsWindow(d_.hWnd_)) {
				// stop all timers
				for (auto& it : d_.timers_)
				{
					timer_management timer(*this);
					if (timer.running(it.first))
						timer.stop(it.first);
				}

				if (!d_.m_children_.empty()) {
					// children exist, notify them the parent is closing and close them
					for (auto& child : d_.m_children_) {
						child.second->d_.parent_closing_ = true;
						child.second->close();
					}
				}

				// Enable parent if this is not a menu form.
				// If this is a menu form, do not enable parent because the menu form is not
				// responsible for disabling the parent (if it is disabled)
				if (!d_.menu_form_ && IsWindow(d_.hWnd_parent_) && !IsWindowEnabled(d_.hWnd_parent_))
					EnableWindow(d_.hWnd_parent_, TRUE);

				DestroyWindow(d_.hWnd_);
			}
		}

		bool form::on_layout(std::string& error) { return true; }
		void form::on_start() {}
		void form::on_close() { close(); }
		void form::on_shutdown() {}

		bool form::prompt(const std::string& question) {
			class prompt_form : public form {
				const size min_size_ = { 220.f, 120.f };
				const size max_size_ = { 420.f, 400.f };
				const std::string question_;
				const float margin_ = 10.f;
				const size button_size_ = widgets::button::button_specs().rect.size();
				bool& user_agreed_;

			public:
				prompt_form(const std::string& title, const std::string& question, form& parent, bool& user_agreed) :
					form(title, parent),
					question_(question),
					user_agreed_(user_agreed) {
					controls ctrls(*this);
					ctrls.resize(false);
					ctrls.minimize(false);

					// impose maximums
					D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, max_size_.width, max_size_.height);

					// measure the question
					widgets::label::label_specs specs_lbl;
					rect = widgets::measure_label(d_.p_directwrite_factory_, question,
						specs_lbl.font, specs_lbl.font_size, false, false, rect);

					auto width = (rect.right - rect.left) + 2 * margin_;
					auto height = d_.caption_bar_height_ + margin_ + (rect.bottom - rect.top) +
						margin_ + button_size_.height + margin_;

					// impose minimums
					width = largest(width, min_size_.width);
					height = largest(height, min_size_.height);

					dimensions dim(*this);
					dim.size({ width, height });
				}

				bool on_layout(std::string& error) override {
					page_management page_man(*this);
					auto& home_page = page_man.add("home");

					widgets::label label(home_page);
					label().text = question_;
					label().multiline = true;
					label().rect = { margin_, home_page.size().width, margin_,
						home_page.size().height - margin_ - button_size_.height - margin_ };

					// add yes and no buttons, in that order for tab navigation
					widgets::button  button_yes(home_page, "button_yes");
					widgets::button button_no(home_page, "button_no");

					// position the no button on the bottom right
					button_no().text = "No";
					button_no().rect.place({ margin_, home_page.size().width - margin_,
							margin_, home_page.size().height - margin_ }, 100.f, 100.f);
					button_no().events().action = [&]() {
						user_agreed_ = false;
						close();
					};

					// snap the yes button to the no button
					button_yes().text = "Yes";
					button_yes().rect.snap_to(button_no().rect, lecui::rect::snap_type::left, margin_);
					button_yes().events().action = [&]() {
						user_agreed_ = true;
						close();
					};

					page_man.show("home");
					widget_management widget_man(*this);
					widget_man.select("home/button_yes");
					return true;
				}
			};

			// prompt the user
			std::string error;
			bool user_agreed = false;
			if (!prompt_form(d_.caption_formatted_, question, *this, user_agreed).show(error))
				log(error);

			return user_agreed;
		}

		void form::message(const std::string& message) {
			if (!message.empty()) {
				class message_form : public form {
					const size min_size_ = { 220.f, 120.f };
					const size max_size_ = { 420.f, 400.f };
					const std::string message_;
					const float margin_ = 10.f;
					const size button_size_ = widgets::button::button_specs().rect.size();

				public:
					message_form(const std::string& title, const std::string& message, form& parent) :
						form(title, parent),
						message_(message) {
						controls ctrls(*this);
						ctrls.resize(false);
						ctrls.minimize(false);

						// impose maximums
						D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, max_size_.width, max_size_.height);

						// measure the message
						widgets::label::label_specs specs_lbl;
						rect = widgets::measure_label(d_.p_directwrite_factory_, message,
							specs_lbl.font, specs_lbl.font_size, false, false, rect);

						auto width = (rect.right - rect.left) + 2 * margin_;
						auto height = d_.caption_bar_height_ + margin_ + (rect.bottom - rect.top) +
							margin_ + button_size_.height + margin_;

						// impose minimums
						width = largest(width, min_size_.width);
						height = largest(height, min_size_.height);

						dimensions dim(*this);
						dim.size({ width, height });
					}

					bool on_layout(std::string& error) override {
						page_management page_man(*this);
						auto& home_page = page_man.add("home");

						widgets::label label(home_page);
						label().text = message_;
						label().multiline = true;
						label().rect = { margin_, home_page.size().width, margin_,
							home_page.size().height - margin_ - button_size_.height - margin_ };

						// add the ok button on the bottom right
						widgets::button button(home_page, "button_ok");
						button().text = "Ok";
						button().rect.place({margin_, home_page.size().width - margin_,
							margin_, home_page.size().height - margin_ }, 100.f, 100.f);
						button().events().action = [&]() { close(); };

						page_man.show("home");
						widget_management widget_man(*this);
						widget_man.select("home/button_ok");
						return true;
					}
				};

				// display the message
				std::string error;
				if (!message_form(d_.caption_formatted_, message, *this).show(error))
					log(error);
			}
		}

		void form::update() { d_.update(); }

		// this is an expensive call. only use if update() doesn't get the job done.
		void form::reload() { d_.discard_device_resources(); d_.update(); }

		void form::on_caption(std::function<void()> on_caption) {
			d_.on_caption_ = on_caption;
		}

		void
			form::on_drop_files(std::function<void(const std::string& file)> on_drop_files) {
			d_.on_drop_files_ = on_drop_files;
			DragAcceptFiles(d_.hWnd_, on_drop_files == nullptr ? FALSE : TRUE);
		}

		void form::on_receive_data(std::function<void(const std::string& data)> on_receive_data) {
			d_.on_receive_data_ = on_receive_data;
		}

		bool form::keep_alive() {
			MSG msg = {};

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					PostQuitMessage(0);
					return false;
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			return true;
		}

		float form::get_dpi_scale() {
			return d_.get_dpi_scale();
		}
	}
}
