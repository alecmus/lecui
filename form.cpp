/*
** form.cpp - form implementation
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

#include "form.h"
#include "controls.h"
#include "widgets/timer.h"
#include "form_impl.h"
#include "error/win_error.h"

// Windows headers
#include <Windows.h>
#include <ShlObj.h>		// for SHBrowseForFolderA

#if defined(min)
#undef min	// to circumvent conflict with dimensions::min
#endif

namespace liblec {
	namespace lecui {
		// this is the constructor that all the others below call
		form::form(const std::string& caption) :
			d_(*new impl(caption)) {
			log("entering form constructor");
		}

		form::form() :
			form::form("form") {}

		form::form(const std::string& caption, form& parent) :
			form::form(caption) {
			// capture parent
			d_.p_parent_ = &parent;
			d_.hWnd_parent_ = parent.d_.hWnd_;

			// this is a child window. add it to the parent's map of children.
			parent.d_.m_children_.insert(std::make_pair(this, this));
		}

		form::~form() {
			if (d_.hWnd_parent_ && d_.p_parent_) {
				// this is a child window. remove it from the parent's map of children
				try { d_.p_parent_->d_.m_children_.erase(this); }
				catch (const std::exception&) {}
			}

			delete& d_;
			log("exiting form destructor");
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
			log("form::show");

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

			// call the layout virtual function
			if (!layout(error))
				return false;

			// create form controls

			d_.create_close_button([&]() { on_close(); });

			if (d_.allow_resizing_)
				d_.create_maximize_button();

			if (d_.allow_minimize_)
				d_.create_minimize_button();

			d_.create_form_caption([&]() { on_caption(); });

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

			if (d_.activate_)
				SetForegroundWindow(d_.hWnd_);

			// disable parent
			if (IsWindow(d_.hWnd_parent_) && IsWindowEnabled(d_.hWnd_parent_))
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
					widgets::timer timer(*this);
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

				// enable parent
				if (IsWindow(d_.hWnd_parent_) && !IsWindowEnabled(d_.hWnd_parent_))
					EnableWindow(d_.hWnd_parent_, TRUE);

				DestroyWindow(d_.hWnd_);
			}
		}

		bool form::layout(std::string& error) { return true; }
		void form::on_start() {}
		void form::on_caption() {}
		void form::on_close() { close(); }
		void form::on_shutdown() {}

		bool form::prompt(const std::string& question) {
			if (MessageBoxA(IsWindow(d_.hWnd_) ? d_.hWnd_ : GetForegroundWindow(),
				question.c_str(), d_.caption_plain_.c_str(), MB_YESNO) == IDYES)
				return true;
			else
				return false;
		}

		void form::message(const std::string& message) {
			if (!message.empty()) {
				class message_form : public form {
					const size min_size_ = { 220.f, 120.f };
					const size max_size_ = { 420.f, 400.f };
					const std::string message_;
					const float margin_ = 10.f;
					const size button_size_ = { 80.f, 25.f };

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
						widgets::label_specs specs_lbl;
						rect = widgets_impl::measure_label(d_.p_directwrite_factory_, message,
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

					bool layout(std::string& error) override {
						page_management page_man(*this);
						auto& home_page = page_man.add("home");

						auto& specs_lbl = widgets::label(home_page).add("message");
						specs_lbl.text = message_;
						specs_lbl.multiline = true;
						specs_lbl.rect = { margin_, home_page.size().width, margin_,
							home_page.size().height - margin_ - button_size_.height - margin_ };

						auto& specs_btn = widgets::button(home_page).add("button");
						specs_btn.text = "Ok";
						specs_btn.rect = { home_page.size().width - margin_ - button_size_.width,
							home_page.size().width - margin_,
							home_page.size().height - margin_ - button_size_.height,
							home_page.size().height - margin_ };
						specs_btn.on_click = [&]() { close(); };

						page_man.show("home");
						return true;
					}
				};

				// display the message
				std::string error;
				if (!message_form(d_.caption_formatted_, message, *this).show(error))
					log(error);
			}
		}

		std::string form::select_folder(const std::string& title) {
			char szDir[MAX_PATH];
			BROWSEINFOA bInfo = { 0 };
			bInfo.hwndOwner = d_.hWnd_;
			bInfo.pszDisplayName = szDir;
			bInfo.lpszTitle = title.c_str();
			bInfo.iImage = -1;

			std::string folder;
			LPITEMIDLIST lpItem = SHBrowseForFolderA(&bInfo);
			if (lpItem != NULL) {
				SHGetPathFromIDListA(lpItem, szDir);
				folder = szDir;
			}

			return folder;
		}

		static auto make_filter_string(const std::vector<file_type>& file_types,
			const bool& include_all_supported_types,
			std::vector<std::string>& filetypes_indexed) {
			/// If we indicate a NULL termminator with a new line character,
			/// The filter string format is as follows :
			/// Bitmap Files(*.bmp)
			/// *.BMP
			/// JPEG(*.jpg; *.jpeg)
			/// *.JPG; *.JPEG
			/// PNG(*.png)
			/// *.PNG
			/// All supported images
			/// *.JPG; *.JPEG; *.PNG; *.BMP
			std::string filter;
			filetypes_indexed.clear();

			// create NULL terminator
			std::string s_null({ '\0' });

			std::vector<file_type> types;

			for (size_t i = 0; i < file_types.size(); i++) {
				// check if type description hasn't already been captured; if it has,
				// append it's extension to the other
				bool b_new = true;

				for (size_t x = 0; x < types.size(); x++) {
					if (types[x].description == file_types[i].description) {
						types[x].extension += ";*." + file_types[i].extension;

						b_new = false;
						break;
					}
				}

				if (b_new) {
					file_type type;
					type.description = file_types[i].description;
					type.extension = "*." + file_types[i].extension;
					types.push_back(type);
				}
			}

			std::string all_file_extensions;

			for (size_t i = 0; i < types.size(); i++) {
				std::string description = types[i].description;
				std::string extension = types[i].extension;

				if (all_file_extensions.empty())
					all_file_extensions = extension;
				else
					all_file_extensions += ";" + extension;

				std::string filter_;
				filter_ = description + " (" + extension + ")" + s_null;
				filter_ += extension + s_null;

				filter += filter_;
				filetypes_indexed.push_back(types[i].extension);
			}

			// add all supported files
			if (include_all_supported_types) {
				std::string filter_;
				std::string description = "All supported files";
				std::string extension = all_file_extensions;
				filter_ = description + " (" + extension + ")" + s_null;
				filter_ += extension + s_null;

				filter += filter_;
				filetypes_indexed.push_back("");
			}

			return filter;
		}

		// remove anything that includes and comes after the first occurence of c in the string fullpath
		static void rem_tail(std::string& str, char c) {
			// remove extension if present
			const auto char_idx = str.find(c);
			if (std::string::npos != char_idx)
				str.erase(char_idx);

			return;
		}

		std::string form::open_file(const open_file_params& params) {
			std::vector<std::string> filetypes_indexed;
			const auto filter_string =
				make_filter_string(params.file_types, params.include_all_supported_types,
					filetypes_indexed);

			char _FilePath[MAX_PATH];
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = d_.hWnd_;
			ofn.lpstrFile = _FilePath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter_string.c_str();
			ofn.nFilterIndex = 1;
			ofn.hInstance = GetModuleHandle(NULL);
			ofn.lpstrTitle = params.title.c_str();
			ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
				OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
			return (GetOpenFileNameA(&ofn) == TRUE) ? std::string(_FilePath) : std::string();
		}

		std::string form::save_file(const std::string& file,
			const save_file_params& params) {
			std::vector<std::string> filetypes_indexed;
			const auto filter_string =
				make_filter_string(params.file_types, params.include_all_supported_types,
					filetypes_indexed);

			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			char _FilePath[MAX_PATH];
			if (!file.empty())
				auto temp = lstrcpynA(_FilePath, file.c_str(), _countof(_FilePath));
			ofn.hwndOwner = d_.hWnd_;
			ofn.lpstrFile = _FilePath;
			if (file.empty())
				ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter_string.c_str();
			ofn.nFilterIndex = 1;
			ofn.hInstance = GetModuleHandle(NULL);
			ofn.lpstrTitle = params.title.c_str();
			ofn.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
				OFN_NOTESTFILECREATE | OFN_ENABLESIZING;

			std::string fullpath;
			if (GetSaveFileNameA(&ofn) == TRUE) {
				// capture full path
				fullpath = _FilePath;

				// remove multiple file extensions
				for (size_t i = 0; i < filetypes_indexed.size(); i++)
					rem_tail(filetypes_indexed[i], ';');

				// determine selected file type
				std::string sExtension("");

				///
				/// nFilterIndex
				/// Type: DWORD
				/// The index of the currently selected filter in the File Types control. The buffer
				/// pointed to by lpstrFilter contains pairs of strings that define the filters. The
				/// first pair of strings has an index value of 1, the second pair 2, and so on. An
				/// index of zero indicates the custom filter specified by	lpstrCustomFilter. You
				/// can specify an index on input to indicate the initial filter description and
				/// filter pattern for the dialog box. When the user selects a file, nFilterIndex
				/// returns the index of the currently displayed filter. If nFilterIndex is zero and
				/// lpstrCustomFilter is NULL, the system uses the first filter in the lpstrFilter
				/// buffer. If all three members are zero or NULL, the system does not use any
				/// filters and does not show any files in the file list control of the dialog box.
				/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646839(v=vs.85).aspx
				///
				if (filetypes_indexed.size() > 0 && filetypes_indexed.size() >= ofn.nFilterIndex - 1)
					sExtension = filetypes_indexed[ofn.nFilterIndex - 1];

				// add the selected extension to the full path (if it exists)
				if (!sExtension.empty()) {
					// format extension properly
					const size_t period_idx = sExtension.rfind('.');

					if (std::basic_string<TCHAR>::npos != period_idx)
						sExtension.erase(0, period_idx);

					// add the extension if it's not there
					if (fullpath.find(sExtension) == std::basic_string<TCHAR>::npos)
						fullpath += sExtension;
					else { /* user probably selected an existing file or typed in with an extension */ }
				}
			}

			return fullpath;
		}

		void form::enable(const std::string& path, bool enable) { d_.enable(path, enable); }
		void form::show(const std::string& path, bool show) { d_.show(path, show); }

		void form::close(const std::string& path) {
			// use timer in case a widget is closed from its own handler.
			// this way the actual closing will be done (hopefully) outside the handler coz of async.
			// the caller still has to exercise caution by avoiding such logical errors.
			d_.scheduled_for_closure_.push_back(path);
			lecui::widgets::timer(*this).add("close_widget_timer", 0,
				[&]() {
					lecui::widgets::timer(*this).stop("close_widget_timer");
					for (const auto& it : d_.scheduled_for_closure_)
						d_.close(it);

					d_.scheduled_for_closure_.clear();
				});
		}

		void form::update() { d_.update(); }

		// this is an expensive call. only use if update() doesn't get the job done.
		void form::reload() { d_.discard_device_resources(); d_.update(); }

		void
			form::dropped_files(std::function<void(const std::string& file)> on_drop_files) {
			d_.on_drop_files_ = on_drop_files;
			DragAcceptFiles(d_.hWnd_, on_drop_files == nullptr ? FALSE : TRUE);
		}
	}
}
