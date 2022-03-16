//
// form.cpp - form implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "form.h"
#include "appearance.h"
#include "controls.h"
#include "utilities/timer.h"
#include "form_impl/form_impl.h"
#include "error/win_error.h"

// Windows headers
#include <Windows.h>

namespace liblec {
	namespace lecui {
		// this is the constructor that all the others below call
		form::form(const std::string& caption) :
			_d(*new impl(*this, caption)) {}

		form::form(const std::string& caption, form& parent) :
			form::form(caption) {

			// copy the parent's default theme setting
			appearance aprnc(*this);
			aprnc.theme(parent._d._theme);

			if (IsWindow(parent._d._hWnd)) {
				// close the parent's tooltips
				parent._d.close_tooltips();

				// capture parent
				_d._p_parent = &parent;
				_d._hWnd_parent = parent._d._hWnd;

				// this is a child window. add it to the parent's map of children.
				parent._d._m_children.insert(std::make_pair(this, this));
			}
		}

		form::~form() {
			if (_d._hWnd_parent && _d._p_parent) {
				// this is a child window. remove it from the parent's map of children
				try { _d._p_parent->_d._m_children.erase(this); }
				catch (const std::exception&) {}
			}

			delete& _d;
		}

		std::string form::menu_form_caption() {
			return std::string("lecui::form::menu");
		}

		std::string form::tooltip_form_caption() {
			return std::string("lecui::form::tooltip");
		}

		void form::move(const point& point) {
			_d._user_pos = true;
			_d._preset_pos = false;
			_d._point = point;
		}

		void form::move(const form_position& form_position) {
			_d._user_pos = true;
			_d._preset_pos = true;
			_d._form_position = form_position;
		}

		void form::force_instance() {
			_d._force_instance = true;
		}

		bool form::create(std::string& error) {
			if (_d._create_called) {
				error = "Library usage error: form::create";
				return false;
			}
			else {
				_d._create_called = true;

				if (!_d._initialized) {
					error = "Library initialization error: form::create";
					return false;
				}
			}

			if (_d._p_instance) {
				if (_d._p_instance->another_instance_running()) {
					log("another instance running");

					if (!_d._force_instance) {
						_d.open_existing_instance();
						return true;
					}
				}
			}

			// don't allow a parent to have more than one modal form
			if (IsWindow(_d._hWnd_parent)) {
				if (_d._p_parent) {
					if (_d._p_parent->_d._m_children.size() > 1) {
						log("Another modal form is open");
						error = "Another modal form is open";
						return false;
					}
				}
			}

			// get resource module handle
			if (!_d._resource_dll_filename.empty()) {
				_d._resource_module_handle =
					LoadLibrary(convert_string(_d._resource_dll_filename).c_str());

				if (!_d._resource_module_handle) {
					// LoadLibrary failed
					error = "Loading " + _d._resource_dll_filename + " failed: " + get_last_error();
					return false;
				}
			}
			else
				_d._resource_module_handle = GetModuleHandle(nullptr);

			// call the on_initialize virtual method
			if (!on_initialize(error))
				return false;

			// check if close was called
			if (_d._close_called)
				return true;

			// call the on_layout virtual method
			if (!on_layout(error))
				return false;

			// handle menu form and tooltip form
			if (_d._menu_form || _d._tooltip_form) {
				_d._allow_resizing = false;
				_d._allow_minimize = false;

				if (_d._tooltip_form)
					_d._activate = false;
			}

			// create form controls
			if (!(_d._menu_form || _d._tooltip_form)) {
				_d.create_close_button([&]() { on_close(); });

				if (_d._allow_resizing)
					_d.create_maximize_button();

				if (_d._allow_minimize)
					_d.create_minimize_button();

				_d.create_form_caption();
				_d.create_form_menu();
			}

			// register window class
			WNDCLASSEX wcex = { 0 };
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.lpfnWndProc = _d.window_procedure;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wcex.hInstance = GetModuleHandle(nullptr);

			// load main application icon
			if (_d._idi_icon)
				wcex.hIcon = LoadIcon(_d._resource_module_handle, MAKEINTRESOURCE(_d._idi_icon));

			// load small application icon
			if (_d._idi_icon_small)
				wcex.hIconSm = (HICON)LoadImage(_d._resource_module_handle,
					MAKEINTRESOURCE(_d._idi_icon_small),
					IMAGE_ICON,
					GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
					LR_DEFAULTCOLOR
					);

			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = nullptr;
			wcex.lpszClassName = L"liblec::lecui::form";
			RegisterClassEx(&wcex);

			if (_d._user_pos) {
				if (_d._preset_pos)
					_d.set_position(_d._form_position, _d._size.get_width(), _d._size.get_height());
				else
					_d.set_position(_d._point.get_x(), _d._point.get_y(), _d._size.get_width(), _d._size.get_height());
			}
			else {
				if (IsWindow(_d._hWnd_parent) && IsWindowEnabled(_d._hWnd_parent))
					_d.set_position(form_position::center_to_parent,
						_d._size.get_width(), _d._size.get_height());
				else
					_d.set_position(form_position::center_to_working_area,
						_d._size.get_width(), _d._size.get_height());
			}

			// Register this instance so other instances can find this form and open it
			if (!_d._guid.empty() && !IsWindow(_d._hWnd_parent))
				_d._reg_id = RegisterWindowMessageA(_d._guid.c_str());

			// perform initialization (_d._hWnd will be captured in WM_CREATE)
			if (!CreateWindowEx(_d._tooltip_form ? NULL : _d._top_most == true ? WS_EX_TOPMOST : NULL, wcex.lpszClassName,
				convert_string(_d._caption_plain).c_str(),
				_d._tooltip_form ? WS_POPUP : static_cast<DWORD>(form::impl::style::aero_borderless),
				static_cast<int>(.5f + _d._point.get_x() * _d._dpi_scale),
				static_cast<int>(.5f + _d._point.get_y() * _d._dpi_scale),
				static_cast<int>(.5f + _d._size.get_width() * _d._dpi_scale),
				static_cast<int>(.5f + _d._size.get_height() * _d._dpi_scale),
				_d._hWnd_parent, nullptr, wcex.hInstance, this)) {
				error = get_last_error();
				return false;
			}

			ShowWindow(_d._hWnd, _d._start_hidden ? SW_HIDE : (_d._activate ? SW_SHOW : SW_SHOWNA));
			UpdateWindow(_d._hWnd);

			// Update the user interface. This is important for widgets that are moved into
			// special panes. If this is not done the widgets will not be drawn fully. One
			// typical symptom of this is that before the mouse is moved at least once over an
			// active widget the drawing will be incomplete, e.g. the selected text in comboboxes
			// won't initially be displayed.
			update();

			if (_d._activate && !_d._start_hidden && !_d._tooltip_form)
				SetForegroundWindow(_d._hWnd);

			// Disable parent if this is neither a menu form nor a tooltip form.
			// If this is a menu form or tooltip form, do not disable parent because we will need to be
			// able to click the parent. The parent, is responsible for ignoring mouse movements
			// and such things when it detects that a child menu form is open
			if (!(_d._menu_form || _d._tooltip_form) && IsWindow(_d._hWnd_parent) && IsWindowEnabled(_d._hWnd_parent))
				EnableWindow(_d._hWnd_parent, FALSE);

			MSG msg = {};

			// main message loop
			BOOL result = NULL;
			while ((result = GetMessage(&msg, nullptr, 0, 0)) != 0) {	// GetMessage returns 0 when WM_QUIT is seen

				// quitting via a user defined quit message
				if (msg.message == UWM_QUIT)
					break;

				if (result == -1) {
					// error occurred
					error = get_last_error();
					log(error);
					break;
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			if (_d._parent_closing)
				PostThreadMessage(_d._current_thread_id, UWM_QUIT, 0, 0);

			return true;
		}

		void form::close() {
			_d._close_called = true;

			if (IsWindow(_d._hWnd)) {
				// stop all timers
				for (auto& it : _d._timers)
				{
					timer_manager timer(*this);
					if (timer.running(it.first))
						timer.stop(it.first);
				}

				if (!_d._m_children.empty()) {
					// children exist, notify them the parent is closing and close them
					for (auto& child : _d._m_children) {
						child.second->_d._parent_closing = true;
						child.second->close();
					}
				}

				// Enable parent if this is neither a menu form nor a tooltip form.
				// If this is a menu form or a tooltip form, do not enable parent because the menu form is not
				// responsible for disabling the parent (if it is disabled)
				if (!(_d._menu_form || _d._tooltip_form) && IsWindow(_d._hWnd_parent) && !IsWindowEnabled(_d._hWnd_parent))
					EnableWindow(_d._hWnd_parent, TRUE);

				DestroyWindow(_d._hWnd);
			}
		}

		bool form::on_initialize(std::string& error) { return true; }
		bool form::on_layout(std::string& error) { return true; }
		void form::on_start() {}
		void form::on_close() { close(); }
		void form::on_shutdown() {}

		bool form::prompt(const std::string& question) {
			class prompt_form : public form {
				const size _min_size = { 220.f, 120.f };
				const size _max_size = { 420.f, 400.f };
				const std::string _question;
				const float _margin = 10.f;
				const size _button_size = widgets::button().rect().size();
				bool& _user_agreed;

			public:
				prompt_form(const std::string& title, const std::string& question, form& parent, bool& user_agreed) :
					form(title, parent),
					_question(question),
					_user_agreed(user_agreed) {
					controls ctrls(*this);
					ctrls
						.allow_resize(false)
						.allow_minimize(false);

					appearance apprnc(*this);
					apprnc
						.caption_icon(parent._d._png_caption_icon);

					// impose maximums
					D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, _max_size.get_width(), _max_size.get_height());

					// measure the question
					widgets::label specs_lbl;
					rect = widgets::measure_label(_d._p_directwrite_factory, question,
						specs_lbl.font(), specs_lbl.font_size(), text_alignment::left, paragraph_alignment::top, rect);

					auto width = (rect.right - rect.left) + 2 * _margin;
					auto height = _d._caption_bar_height + _margin + (rect.bottom - rect.top) +
						_margin + _button_size.get_height() + _margin;

					// impose minimums
					width = largest(width, _min_size.get_width());
					height = largest(height, _min_size.get_height());

					dimensions dim(*this);
					dim.set_size({ width, height });
				}

				bool on_layout(std::string& error) override {
					page_manager page_man(*this);
					auto& home_page = page_man.add("home");

					auto& label = widgets::label::add(home_page);
					label
						.text(_question).multiline(true)
						.rect(rect()
							.left(_margin)
							.right(home_page.size().get_width())
							.top(_margin)
							.bottom(home_page.size().get_height() - _margin - _button_size.get_height() - _margin));

					// add yes and no buttons, in that order for tab navigation
					auto& button_yes = widgets::button::add(home_page, "button_yes");
					auto& button_no = widgets::button::add(home_page, "button_no");

					// position the no button on the bottom right
					button_no.text("No")
						.rect().place(rect()
							.left(_margin)
							.right(home_page.size().get_width() - _margin)
							.top(_margin)
							.bottom(home_page.size().get_height() - _margin),
							100.f, 100.f);
					button_no.events().action = [&]() {
						_user_agreed = false;
						close();
					};

					// snap the yes button to the no button
					button_yes
						.text("Yes")
						.rect().snap_to(button_no.rect(), lecui::rect::snap_type::left, _margin);
					button_yes.events().action = [&]() {
						_user_agreed = true;
						close();
					};

					page_man.show("home");
					widget_manager widget_man(*this);
					widget_man.select("home/button_yes");
					return true;
				}
			};

			// prompt the user
			std::string error;
			bool user_agreed = false;
			if (!prompt_form(_d._caption_formatted, question, *this, user_agreed).create(error))
				log(error);

			return user_agreed;
		}

		void form::message(const std::string& message) {
			if (!message.empty()) {
				class message_form : public form {
					const size _min_size = { 220.f, 120.f };
					const size _max_size = { 420.f, 400.f };
					const std::string _message;
					const float _margin = 10.f;
					const size _button_size = widgets::button().rect().size();

				public:
					message_form(const std::string& title, const std::string& message, form& parent) :
						form(title, parent),
						_message(message) {
						controls ctrls(*this);
						ctrls
							.allow_resize(false)
							.allow_minimize(false);

						appearance apprnc(*this);
						apprnc
							.caption_icon(parent._d._png_caption_icon);

						// impose maximums
						D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, _max_size.get_width(), _max_size.get_height());

						// measure the message
						widgets::label specs_lbl;
						rect = widgets::measure_label(_d._p_directwrite_factory, message,
							specs_lbl.font(), specs_lbl.font_size(), text_alignment::left, paragraph_alignment::top, rect);

						auto width = (rect.right - rect.left) + 2 * _margin;
						auto height = _d._caption_bar_height + _margin + (rect.bottom - rect.top) +
							_margin + _button_size.get_height() + _margin;

						// impose minimums
						width = largest(width, _min_size.get_width());
						height = largest(height, _min_size.get_height());

						dimensions dim(*this);
						dim.set_size({ width, height });
					}

					bool on_layout(std::string& error) override {
						page_manager page_man(*this);
						auto& home_page = page_man.add("home");

						auto& label = widgets::label::add(home_page);
						label
							.text(_message).multiline(true)
							.rect(rect()
								.left(_margin)
								.right(home_page.size().get_width())
								.top(_margin)
								.bottom(home_page.size().get_height() - _margin - _button_size.get_height() - _margin));

						// add the ok button on the bottom right
						auto& button = widgets::button::add(home_page, "button_ok");
						button
							.text("Ok")
							.rect().place(rect()
								.left(_margin)
								.right(home_page.size().get_width() - _margin)
								.top(_margin)
								.bottom(home_page.size().get_height() - _margin),
								100.f, 100.f);
						button.events().action = [&]() { close(); };

						page_man.show("home");
						widget_manager widget_man(*this);
						widget_man.select("home/button_ok");
						return true;
					}
				};

				// display the message
				std::string error;
				if (!message_form(_d._caption_formatted, message, *this).create(error))
					log(error);
			}
		}

		void form::update() { _d.update(); }

		// this is an expensive call. only use if update() doesn't get the job done.
		void form::reload() { _d.discard_device_resources(); _d.update(); }

		void form::on_caption(std::function<void()> on_caption, const std::string& tooltip) {
			_d._on_caption = on_caption;
			_d._caption_tooltip = tooltip;
		}

		void
			form::on_drop_files(std::function<void(const std::string& file)> on_drop_files) {
			_d._on_drop_files = on_drop_files;
			DragAcceptFiles(_d._hWnd, on_drop_files == nullptr ? FALSE : TRUE);
		}

		void form::on_receive_data(std::function<void(const std::string& data)> on_receive_data) {
			_d._on_receive_data = on_receive_data;
		}

		bool form::keep_alive() {
			MSG msg = {};

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == UWM_QUIT) {
					// user defined quit message detected ... don't process;
					// repost it since it was removed from the queue by PM_REMOVE
					// then return false to notify caller to stop the lengthy operation immediately
					PostThreadMessage(GetCurrentThreadId(), UWM_QUIT, 0, 0);
					return false;
				}
				else
					if (msg.message == WM_QUIT) {
						// not expected ... since we're using user defined quit message but possible,
						// e.g. if the system is logging off
						
						// WM_QUIT detected ... don't process;
						// repost it since it was removed from the queue by PM_REMOVE
						// then return false to notify caller to stop the lengthy operation immediately
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
			return _d.get_dpi_scale();
		}

		void form::allow_minimize() {
			if (!_d._allow_minimize)
				return;

			if (IsWindow(_d._hWnd))
				ShowWindow(_d._hWnd, SW_MINIMIZE);
		}

		void form::restore() {
			if (IsWindow(_d._hWnd)) {
				if (!IsWindowVisible(_d._hWnd))
					ShowWindow(_d._hWnd, SW_SHOW);
				else
					ShowWindow(_d._hWnd, SW_RESTORE);

				SetForegroundWindow(_d._hWnd);
			}
		}

		void form::maximize() {
			if (!_d._allow_resizing)
				return;

			if (IsWindow(_d._hWnd))
				ShowWindow(_d._hWnd, SW_MAXIMIZE);
		}

		void form::hide() {
			if (IsWindow(_d._hWnd))
				ShowWindow(_d._hWnd, SW_HIDE);
		}

		bool form::visible() {
			if (IsWindow(_d._hWnd))
				return IsWindowVisible(_d._hWnd) == TRUE;
			else
				return false;
		}

		void form::show() {
			if (IsWindow(_d._hWnd)) {
				ShowWindow(_d._hWnd, SW_SHOW);
				SetForegroundWindow(_d._hWnd);
			}
		}

		bool form::minimized() {
			if (IsWindow(_d._hWnd))
				return IsIconic(_d._hWnd) == TRUE;
			else
				return false;
		}

		void form::prevent_quit() {
			if (_d._p_close_button)
				// disable the close button
				// no need to do anything else to prevent the app from exiting ...
				// once the close button is disabled even Alt+F4 won't work
				_d._p_close_button->enable(false);

			update();
		}

		void form::allow_quit() {
			if (_d._p_close_button)
				// enable the close button
				_d._p_close_button->enable(true);

			update();
		}

		form::form_events& form::events() { return _d._form_events; }
	}
}
