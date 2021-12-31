//
// filesystem.cpp - filesystem implementation
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#include "../filesystem.h"
#include "../../form_impl/form_impl.h"

// Windows headers
#include <Windows.h>
#include <ShObjIdl_core.h>		// for IFileDialog

namespace liblec {
	namespace lecui {
		class filesystem::impl {
		public:
			impl(form& fm) :
				_fm(fm) {}
			form& _fm;
		};

		filesystem::filesystem(form& fm) :
			_d(*(new impl(fm))) {}

		filesystem::~filesystem() { delete& _d; }

		std::string filesystem::select_folder(const std::string& title) {
			std::string folder;

			IFileDialog* p_file_dialog = nullptr;
			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&p_file_dialog)))) {

				DWORD options;
				if (SUCCEEDED(p_file_dialog->GetOptions(&options))) {
					p_file_dialog->SetOptions(options | FOS_PICKFOLDERS);

					if (!title.empty())
						p_file_dialog->SetTitle(convert_string(title).c_str());

					if (SUCCEEDED(p_file_dialog->Show(_d._fm._d._hWnd))) {

						IShellItem* p_shell_item = nullptr;

							if (SUCCEEDED(p_file_dialog->GetResult(&p_shell_item))) {
								LPWSTR result = nullptr;
								p_shell_item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &result);
								p_shell_item->Release();

								if (result)
									folder = convert_string(std::wstring(result));

								if (result != NULL)
									CoTaskMemFree(result);
							}
					}
				}

				p_file_dialog->Release();
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

				std::string _filter;
				_filter = description + " (" + extension + ")" + s_null;
				_filter += extension + s_null;

				filter += _filter;
				filetypes_indexed.push_back(types[i].extension);
			}

			// add all supported files
			if (include_all_supported_types) {
				std::string _filter;
				std::string description = "All supported files";
				std::string extension = all_file_extensions;
				_filter = description + " (" + extension + ")" + s_null;
				_filter += extension + s_null;

				filter += _filter;
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

		/// <summary>Helper class for handling COMDLG_FILTERSPEC filters.</summary>
		class filter_helper {
			COMDLG_FILTERSPEC* _filters = nullptr;
			DWORD _num_extensions = 0;
			filter_helper() = delete;

		public:
			/// <summary>Constructor.</summary>
			/// <param name="types">The list of file types.</param>
			/// <param name="include_all_supported_types">Whether to include an "All supported types" option.</param>
			filter_helper(std::vector<file_type> types, bool include_all_supported_types) {
				// make filter
				_filters = make_filter(types, _num_extensions, include_all_supported_types);
			}

			~filter_helper() {
				// clean-up
				free_filter(_filters);
			}

			/// <summary>Get the filter.</summary>
			/// <returns>The filter.</returns>
			COMDLG_FILTERSPEC* get_filter() {
				return _filters;
			}
			
			/// <summary>Get the size of the filter.</summary>
			/// <returns>The size of the filter.</returns>
			DWORD get_filter_size() {
				return _num_extensions;
			}

		private:
			COMDLG_FILTERSPEC* make_filter(std::vector<file_type> types, DWORD& num_extensions, bool include_all_supported_types) {
				num_extensions = 0;

				// use a map to group extensions by description
				std::map<std::string, file_type> _types;

				for (const auto& type : types) {
					if (_types.count(type.description))
						_types.at(type.description).extension += ";*." + type.extension;
					else
						_types.insert(std::make_pair(type.description, type));
				}

				if (!_types.empty()) {
					DWORD index = (include_all_supported_types && _types.size() > 1) ? 1 : 0;

					// dynamic memory allocation (for filter list)
					auto* filters = new COMDLG_FILTERSPEC[_types.size() + 1 + index];

					size_t all_spec_length = 0;

					for (const auto& [description, type] : _types) {
						std::wstring description_string = convert_string(type.description);
						std::wstring spec_string = convert_string("*." + type.extension);

						WCHAR* description_source = (WCHAR*)description_string.c_str();
						WCHAR* spec_source = (WCHAR*)spec_string.c_str();

						// dynamic memory allocation (name and spec strings)
						WCHAR* description_buffer = (WCHAR*)std::malloc((wcslen(description_source) + 1) * sizeof(WCHAR));
						WCHAR* spec_buffer = (WCHAR*)std::malloc((wcslen(spec_source) + 1) * sizeof(WCHAR));

						if (description_buffer && spec_buffer) {
							memcpy(description_buffer, description_source, (wcslen(description_source) + 1) * sizeof(WCHAR));
							memcpy(spec_buffer, spec_source, (wcslen(spec_source) + 1) * sizeof(WCHAR));

							filters[index].pszName = description_buffer;
							filters[index].pszSpec = spec_buffer;

							all_spec_length += wcslen(filters[index].pszSpec) + 1;
						}
						else {
							filters[index].pszName = nullptr;
							filters[index].pszSpec = nullptr;
							break;
						}

						index++;
					}

					if (include_all_supported_types && _types.size() > 1) {
						const WCHAR* all_supported_file_types_string = L"All supported files";

						// dynamic memory allocation (name and spec strings)
						WCHAR* description_buffer = (WCHAR*)std::malloc((wcslen(all_supported_file_types_string) + 1) * sizeof(WCHAR));
						WCHAR* spec_buffer = (WCHAR*)std::malloc(all_spec_length * sizeof(WCHAR));

						if (description_buffer && spec_buffer) {
							wcscpy_s(description_buffer, wcslen(all_supported_file_types_string) + 1, all_supported_file_types_string);
							spec_buffer[0] = 0;

							for (DWORD j = 1; j < index; j++) {
								wcscat_s(spec_buffer, all_spec_length, filters[j].pszSpec);
								if (j != index - 1)
									wcscat_s(spec_buffer, all_spec_length, L";");
							}
						}

						filters[0].pszName = description_buffer;
						filters[0].pszSpec = spec_buffer;
					}

					num_extensions = index;
					filters[index].pszName = nullptr;
					filters[index].pszSpec = nullptr;
					return filters;
				}
				else
					return nullptr;
			}

			void free_filter(COMDLG_FILTERSPEC* filters) {
				if (filters) {
					int32_t index = 0;
					while (filters[index].pszName) {
						// free memory allocated for name and spec strings
						std::free((void*)filters[index].pszName);
						std::free((void*)filters[index].pszSpec);
						index++;
					}

					// free memory allocated for filter list
					delete[] filters;
					filters = nullptr;
				}
			}
		};

		std::string filesystem::open_file(const open_file_params& params) {
			std::string file_path;

			IFileDialog* p_file_dialog = nullptr;

			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&p_file_dialog)))) {

				DWORD options;
				if (SUCCEEDED(p_file_dialog->GetOptions(&options))) {
					p_file_dialog->SetOptions(options | FOS_FORCEFILESYSTEM);

					filter_helper helper(params.file_types, params.include_all_supported_types);
					p_file_dialog->SetFileTypes(helper.get_filter_size(), helper.get_filter());

					if (!params.title.empty())
						p_file_dialog->SetTitle(convert_string(params.title).c_str());

					if (SUCCEEDED(p_file_dialog->Show(_d._fm._d._hWnd))) {

						IShellItem* p_shell_item = nullptr;

						if (SUCCEEDED(p_file_dialog->GetResult(&p_shell_item))) {
							LPWSTR result = nullptr;
							p_shell_item->GetDisplayName(SIGDN_FILESYSPATH, &result);
							p_shell_item->Release();

							if (result)
								file_path = convert_string(std::wstring(result));

							if (result != NULL)
								CoTaskMemFree(result);
						}
					}
				}

				p_file_dialog->Release();
			}

			return file_path;
		}

		std::string filesystem::save_file(const std::string& file, const save_file_params& params) {
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
			ofn.hwndOwner = _d._fm._d._hWnd;
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
	}
}
