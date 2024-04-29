#include "DialogHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"

namespace YYCC::DialogHelper {

	template<bool TIsOpen, bool TMultiSelection>
	bool GeneralFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret) {
		// make sure multi-selection only available in open mode
		static_assert(TIsOpen && (!TIsOpen && TMultiSelection == false));

		// build filter
		std::wstring w_filter;
		for (const auto& filter_pair : params.m_Filter) {
			w_filter += EncodingHelper::UTF8ToWchar(filter_pair.first.c_str());
			w_filter += L'\0';
			w_filter += EncodingHelper::UTF8ToWchar(filter_pair.second.c_str());
			w_filter += L'\0';
		}
		// build title
		std::wstring w_title(EncodingHelper::UTF8ToWchar(params.m_Title.c_str()));
		// build default extension
		std::wstring w_default_ext(EncodingHelper::UTF8ToWchar(params.m_DefaultExtension.c_str()));
		// build initial directory
		std::wstring w_init_dir(EncodingHelper::UTF8ToWchar(params.m_InitialDirectory.c_str()));
		// prepare file name receiver and preset it as initial file name
		std::wstring path_receiver(EncodingHelper::UTF8ToWchar(params.m_InitialFileName.c_str()));
		path_receiver.resize(std::max(MAX_PATH, path_receiver.size()), L'\0');

		// prepare the common part of file dialog struct
		OPENFILENAMEW dialog_param;
		ZeroMemory(&dialog_param, sizeof(OPENFILENAMEW));
		dialog_param.lStructSize = sizeof(OPENFILENAMEW);
		// dialog owner
		dialog_param.hwndOwner = params.m_Owner;
		// if no filter, we pass NULL
		dialog_param.lpstrFilter = w_filter.empty() ? NULL : w_filter.c_str();
		// no record to user selected filter
		dialog_param.lpstrCustomFilter = NULL;
		dialog_param.nMaxCustFilter = 0;
		dialog_param.nFilterIndex = 0;
		// path receiver, also is init filename
		dialog_param.lpstrFile = path_receiver.data();
		dialog_param.nMaxFile = static_caast<DWORD>(path_receiver.size());
		// no selected file infos
		dialog_param.lpstrFileTitle = NULL;
		dialog_param.nMaxFileTitle = 0;
		// initial directory
		dialog_param.lpstrInitialDir = w_init_dir.empty() ? NULL : w_init_dir.c_str();
		// dialog title
		dialog_param.lpstrTitle = w_title.empty() ? NULL : w_title.c_str();
		// setup basic flags
		dialog_param.Flags = OFN_EXPLORER;
		// default extension
		dialog_param.lpstrDefExt = w_default_ext.empty() ? NULL : w_default_ext.c_str();

		BOOL status;
		if constexpr (TIsOpen) {
			// multi-selection need add special multi-selection flags
			if constexpr (TMultiSelection) {
				dialog_param.Flags |= OFN_ALLOWMULTISELECT;
			}

			// call browser
			status = GetOpenFileNameW(&dialog_param);

			// only process result when success
			if (status) {
				if constexpr (TMultiSelection) {
					// get directory part, copy from start to dialog param specified offset
					std::wstring w_directory_part(path_receiver.c_str(), dialog_param.nFileOffset);
					// get file names part one by one
					size_t filename_cursor = dialog_param.nFileOffset;
					while (path_receiver[filename_cursor] != L'\0') {
						// init wstring from given offset
						std::wstring w_filename_part(path_receiver.c_str() + filename_cursor);
						// get eaten chars from result and increase to cursor
						filename_cursor += w_filename_part.size() + 1u;
						// combine 2 parts and insert into list
						ret.emplace_back(u8string(EncodingHelper::WcharToUTF8(w_directory_part + w_filename_part)));
					}
				} else {
					ret.emplace_back(u8string(EncodingHelper::WcharToUTF8(path_receiver.c_str())));
				}
			}

		} else {
			// call browser
			status = GetSaveFileNameW(&dialog_param);

			// only process result when success
			if (status) {
				ret.emplace_back(u8string(EncodingHelper::WcharToUTF8(path_receiver.c_str())));
			}

		}

		// if failed, clear result
		// and return result
		if (!status) {
			ret.clear();
		}
		return status == TRUE;
	}

	bool OpenFileDialog(const FileDialogParameter& params, std::string& ret) {
		std::vector<std::string> cache;
		bool isok = GeneralFileDialog<true, false>(params, cache);
		if (isok) ret = cache.front();
		return isok;
	}
	bool OpenMultipleFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret) {
		return GeneralFileDialog<true, true>(params, ret);
	}
	bool SaveFileDialog(const FileDialogParameter& params, std::string& ret) {
		std::vector<std::string> cache;
		bool isok = GeneralFileDialog<false, false>(params, cache);
		if (isok) ret = cache.front();
		return isok;
	}

	bool OpenFolderDialog(const FolderDialogParameter& params, std::string& ret) {
		// create wchar string cache for windows W-tail function
		std::wstring w_title(EncodingHelper::UTF8ToWchar(params.m_Title.c_str()));
		// create buffer for receiving selected folder
		// initialize with MAX_PATH length and content is filled with zero.
		std::wstring path_receiver(MAX_PATH, L'\0');

		// prepare folder foalog struct
		BROWSEINFOW dialog_param = { 0 };
		dialog_param.hwndOwner = params.m_Owner;
		dialog_param.pidlRoot = nullptr;
		dialog_param.pszDisplayName = path_receiver.data();
		dialog_param.lpszTitle = w_title.c_str();
		dialog_param.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		dialog_param.lpfn = nullptr;

		// call browser
		PIDLIST_ABSOLUTE place = SHBrowseForFolderW(&dialog_param);
		// if browser failed, return.
		if (place == nullptr) {
			ret.clear();
			return false;
		}

		// get path from browser result
		BOOL status;
		if (status = SHGetPathFromIDListW(place, path_receiver.data())) {
			EncodingHelper::WcharToUTF8(path_receiver.c_str(), ret);
		} else {
			ret.clear();
		}

		// clear browser result and return
		CoTaskMemFree(place);
		return status == TRUE;
	}

}

#endif
