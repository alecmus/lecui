/*
** filesystem.cpp - filesystem implementation
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

#include "../filesystem.h"
#include "../../form_impl/form_impl.h"

// Windows headers
#include <Windows.h>
#include <ShlObj.h>		// for SHBrowseForFolderA

namespace liblec {
	namespace lecui {
		class filesystem::impl {
		public:
			impl(form& fm) :
				fm_(fm) {}
			form& fm_;
		};

		filesystem::filesystem(form& fm) :
			d_(*(new impl(fm))) {}

		filesystem::~filesystem() { delete& d_; }

		std::string filesystem::select_folder(const std::string& title) {
			char szDir[MAX_PATH];
			BROWSEINFOA bInfo = { 0 };
			bInfo.hwndOwner = d_.fm_.d_.hWnd_;
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

		std::string filesystem::open_file(const open_file_params& params) {
			std::vector<std::string> filetypes_indexed;
			const auto filter_string =
				make_filter_string(params.file_types, params.include_all_supported_types,
					filetypes_indexed);

			char _FilePath[MAX_PATH];
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = d_.fm_.d_.hWnd_;
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
			ofn.hwndOwner = d_.fm_.d_.hWnd_;
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