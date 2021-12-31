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

		/// <summary>Helper class for handling COMDLG_FILTERSPEC filters.</summary>
		class filter_helper {
			COMDLG_FILTERSPEC* _filters = nullptr;
			DWORD _num_extensions = 0;
			DWORD _default_file_type_index = 0;
			filter_helper() = delete;
			std::vector<file_type> _og_type_list;

		public:
			/// <summary>Constructor.</summary>
			/// <param name="types">The list of file types.</param>
			/// <param name="include_all_supported_types">Whether to include an "All Supported Types" option.</param>
			filter_helper(std::vector<file_type> types, const std::string& default_type, bool include_all_supported_types) {
				// make filter
				_filters = make_filter(types, default_type, _num_extensions, _default_file_type_index, include_all_supported_types);
			}

			/// <summary>Destructor.</summary>
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

			/// <summary>Determine and extension that matches a given index.</summary>
			/// <param name="index">The index as returned by GetFileTypeIndex().</param>
			/// <returns>The extension, if a match is found.</returns>
			std::string get_extension_from_index(DWORD index) {
				index -= 1;	// GetFileTypeIndex is 1 based not 0 based

				int32_t i = 0;
				std::string _description;

				while (_filters[i].pszName) {
					auto description = convert_string(_filters[i].pszName);

					if (i == index)
						_description = description;
					
					i++;
				}

				std::string _extension;

				for (const auto& type : _og_type_list) {
					if (type.description == _description) {
						// use the first occurrence under the description
						_extension = type.extension;
						break;
					}
				}

				return _extension;
			}

			/// <summary>Get the index of the default file type.</summary>
			/// <returns>The index of the default file type, suitable for consumption by SetFileTypeIndex().</returns>
			DWORD get_default_file_type_index() {
				return _default_file_type_index;
			}

		private:
			COMDLG_FILTERSPEC* make_filter(std::vector<file_type> types, const std::string& default_type, DWORD& num_extensions, DWORD& defaultFileTypeIndex, bool include_all_supported_types) {
				num_extensions = 0;
				_og_type_list = types;

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
						std::wstring spec_string = type.extension.empty() ? L"*.*" : convert_string("*." + type.extension);

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

							if (default_type == type.description)
								defaultFileTypeIndex = index + 1;
						}
						else {
							filters[index].pszName = nullptr;
							filters[index].pszSpec = nullptr;
							break;
						}

						index++;
					}

					if (include_all_supported_types && _types.size() > 1) {
						const WCHAR* all_supported_file_types_string = L"All Supported Files";

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

		std::vector<std::string> filesystem::open_file(const open_file_params& params) {
			std::vector<std::string> file_list;
			IFileDialog* p_file_dialog = nullptr;

			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&p_file_dialog)))) {

				DWORD options;
				if (SUCCEEDED(p_file_dialog->GetOptions(&options))) {
					p_file_dialog->SetOptions(options | FOS_FORCEFILESYSTEM);

					if (params.allow_multi_select)
						p_file_dialog->SetOptions(options | FOS_ALLOWMULTISELECT);

					filter_helper helper(params.file_types, params.default_type, params.include_all_supported_types);
					p_file_dialog->SetFileTypes(helper.get_filter_size(), helper.get_filter());

					if (!params.title.empty())
						p_file_dialog->SetTitle(convert_string(params.title).c_str());

					auto default_file_type_index = helper.get_default_file_type_index();

					if (default_file_type_index)
						p_file_dialog->SetFileTypeIndex(default_file_type_index);

					if (SUCCEEDED(p_file_dialog->Show(_d._fm._d._hWnd))) {

						if (params.allow_multi_select) {
							IFileOpenDialog* p_file_open_dialog;
							if (SUCCEEDED(p_file_dialog->QueryInterface(IID_IFileOpenDialog, (void**)&p_file_open_dialog))) {

								IShellItemArray* p_shell_items = nullptr;
								if (SUCCEEDED(p_file_open_dialog->GetResults(&p_shell_items))) {

									DWORD count = 0;
									
									if (SUCCEEDED(p_shell_items->GetCount(&count))) {

										for (DWORD i = 0; i < count; i++) {
											IShellItem* p_shell_item = nullptr;

											if (SUCCEEDED(p_shell_items->GetItemAt(i, &p_shell_item))) {
												LPWSTR result = nullptr;
												p_shell_item->GetDisplayName(SIGDN_FILESYSPATH, &result);

												if (result)
													file_list.push_back(convert_string(std::wstring(result)));

												if (result != NULL)
													CoTaskMemFree(result);

												p_shell_item->Release();
											}
										}
										p_shell_items->Release();
									}
								}

								p_file_open_dialog->Release();
							}
						}
						else {
							IShellItem* p_shell_item = nullptr;

							if (SUCCEEDED(p_file_dialog->GetResult(&p_shell_item))) {
								LPWSTR result = nullptr;
								p_shell_item->GetDisplayName(SIGDN_FILESYSPATH, &result);

								if (result)
									file_list.push_back(convert_string(std::wstring(result)));

								if (result != NULL)
									CoTaskMemFree(result);

								p_shell_item->Release();
							}
						}
					}
				}

				p_file_dialog->Release();
			}

			return file_list;
		}

		std::string filesystem::save_file(const std::string& file, const save_file_params& params) {
			std::string file_path;
			IFileDialog* p_file_dialog = nullptr;

			if (SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&p_file_dialog)))) {

				DWORD options;
				if (SUCCEEDED(p_file_dialog->GetOptions(&options))) {
					p_file_dialog->SetOptions(options | FOS_FORCEFILESYSTEM);

					file_type all_types;
					all_types.description = "All Files";
					all_types.extension = "";

					auto file_types = params.file_types;

					if (params.include_all_files)
						file_types.push_back(all_types);

					filter_helper helper(file_types, params.default_type, false);
					p_file_dialog->SetFileTypes(helper.get_filter_size(), helper.get_filter());

					if (!params.title.empty())
						p_file_dialog->SetTitle(convert_string(params.title).c_str());

					auto default_file_type_index = helper.get_default_file_type_index();

					if (default_file_type_index)
						p_file_dialog->SetFileTypeIndex(default_file_type_index);

					if (SUCCEEDED(p_file_dialog->Show(_d._fm._d._hWnd))) {

						IShellItem* p_shell_item = nullptr;

						if (SUCCEEDED(p_file_dialog->GetResult(&p_shell_item))) {
							LPWSTR result = nullptr;
							p_shell_item->GetDisplayName(SIGDN_FILESYSPATH, &result);

							if (result)
								file_path = convert_string(std::wstring(result));

							if (result != NULL)
								CoTaskMemFree(result);

							UINT fileTypeIndex = 0;
							if (SUCCEEDED(p_file_dialog->GetFileTypeIndex(&fileTypeIndex))) {
								std::string extension = helper.get_extension_from_index(fileTypeIndex);

								if (!extension.empty()) {
									bool extension_already_added = false;

									// check if extension has already been added by the user
									auto idx = file_path.rfind('.');

									if (idx != std::string::npos) {
										std::string user_extension = file_path;
										user_extension.erase(0, idx + 1);

										// make user extension lowercase for comparison
										for (auto& character : user_extension)
											character = tolower(character);

										std::string expected_extension = extension;

										// make expected extension lowercase for comparison
										for (auto& character : expected_extension)
											character = tolower(character);

										if (user_extension == expected_extension)
											extension_already_added = true;
									}

									if (!extension_already_added)
										file_path += "." + extension;
								}
							}

							p_shell_item->Release();
						}
					}
				}

				p_file_dialog->Release();
			}

			return file_path;
		}
	}
}
