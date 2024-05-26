#include "DialogHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"

namespace YYCC::DialogHelper {

#pragma region FileFilters

	bool FileFilters::Add(const char* filter_name, std::initializer_list<const char*> il) {
		// assign filter name
		if (filter_name == nullptr) return false;
		FilterName name(filter_name);

		// assign filter patterns
		FilterModes modes;
		for (const char* pattern : il) {
			if (pattern != nullptr) modes.emplace_back(std::string(pattern));
		}

		// check filter patterns
		if (modes.empty()) return false;

		// add into pairs and return
		m_Filters.emplace_back(std::make_pair(name, modes));
		return true;
	}

	bool FileFilters::Generate(WinFileFilters& win_result) const {
		// clear Windows oriented data
		win_result.m_WinDataStruct.reset();
		win_result.m_WinFilters.clear();

		// build new Windows oriented string vector first
		for (const auto& it : m_Filters) {
			// convert name to wchar
			WinFileFilters::WinFilterName name;
			if (!YYCC::EncodingHelper::UTF8ToWchar(it.first.c_str(), name))
				return false;

			// convert pattern and join them
			std::string joined_modes(YYCC::StringHelper::Join(it.second, u8";"));
			WinFileFilters::WinFilterModes modes;
			if (!YYCC::EncodingHelper::UTF8ToWchar(joined_modes.c_str(), modes))
				return false;

			// append new pair
			win_result.m_WinFilters.emplace_back(std::make_pair(name, modes));
		}

		// check filter size
		// if it overflow the maximum value, return false
		size_t count = win_result.m_WinFilters.size();
		if (count > std::numeric_limits<UINT>::max())
			return false;

		// create new win data struct
		// and assign string pointer from internal built win string vector.
		win_result.m_WinDataStruct.reset(new COMDLG_FILTERSPEC[count]);
		for (size_t i = 0u; i < count; ++i) {
			win_result.m_WinDataStruct[i].pszName = win_result.m_WinFilters[i].first.c_str();
			win_result.m_WinDataStruct[i].pszSpec = win_result.m_WinFilters[i].second.c_str();
		}

		// everything is okey
		return true;
	}

#pragma endregion

#pragma region File Dialog

	bool FileDialog::Generate(WinFileDialog& win_result) const {
		return false;
	}

#pragma endregion

#pragma region Windows Dialog Code

	enum class CommonFileDialogType {
		OpenFile,
		OpenMultipleFiles,
		SaveFile,
		OpenFolder
	};

	using SmartFileDialogPtr = std::unique_ptr<IFileDialog, std::function<void(IFileDialog*)>>;

	template<CommonFileDialogType EDialogType>
	bool CommonFileDialog(const FileDialog& params, std::vector<std::string>& ret) {
		// Reference: https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
		// prepare result variable
		HRESULT hr;

		// create file dialog instance
		// fetch dialog CLSID first
		CLSID dialog_clsid;
		switch (EDialogType) {
			case CommonFileDialogType::OpenFile:
			case CommonFileDialogType::OpenMultipleFiles:
			case CommonFileDialogType::OpenFolder:
				dialog_clsid = CLSID_FileOpenDialog;
				break;
			case CommonFileDialogType::SaveFile:
				dialog_clsid = CLSID_FileSaveDialog;
				break;
			default:
				return false;
		}
		// create raw dialog pointer
		IFileDialog* _pfd = nullptr;
		hr = CoCreateInstance(
			dialog_clsid,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&_pfd)
		);
		if (!SUCCEEDED(hr)) return false;
		// create memory-safe dialog pointer
		SmartFileDialogPtr pfd(
			_pfd,
			[](IFileDialog* instance) -> void {
				if (instance != nullptr)
					instance->Release();
			}
		);

		// set options for dialog
		// before setting, always get the options first in order.
		// not to override existing options.
		DWORD dwFlags;
		hr = pfd->GetOptions(&dwFlags);
		if (!SUCCEEDED(hr)) return false;
		// modify options
		switch (EDialogType) {
			// We want user only can pick file system files: FOS_FORCEFILESYSTEM.
			// Open dialog default: FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR
			// Save dialog default: FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR
			// Pick folder: FOS_PICKFOLDERS
			case CommonFileDialogType::OpenFile:
				dwFlags |= FOS_FORCEFILESYSTEM;
				dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
				break;
			case CommonFileDialogType::OpenMultipleFiles:
				dwFlags |= FOS_FORCEFILESYSTEM;
				dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
				dwFlags |= FOS_ALLOWMULTISELECT;
				break;
			case CommonFileDialogType::SaveFile:
				dwFlags |= FOS_FORCEFILESYSTEM;
				dwFlags |= FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR;
				break;
			case CommonFileDialogType::OpenFolder:
				dwFlags |= FOS_FORCEFILESYSTEM;
				dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
				dwFlags |= FOS_PICKFOLDERS;
				break;
			default:
				return false;
		}
		// set folder dialog options
		hr = pfd->SetOptions(dwFlags);
		if (!SUCCEEDED(hr)) return false;

		// set title
		std::wstring wtitle, winit_filename, winit_directory;
		if (params.GetTitle() != nullptr) {
			EncodingHelper::UTF8ToWchar(params.GetTitle(), wtitle);
		}

		// set file types and default file index when we picking file
		WinFileFilters win_file_filters;
		if constexpr (EDialogType != CommonFileDialogType::OpenFolder) {
			// generate data from user specified file filters
			const auto& file_filters = params.GetFileTypes();
			if (!file_filters.Generate(win_file_filters))
				return false;

			// set file types list
			hr = pfd->SetFileTypes(win_file_filters.GetFilterCount(), win_file_filters.GetFilterSpecs());
			if (!SUCCEEDED(hr)) return false;

			// set default file type index
			// Windows order this is 1-based index.
			// We plus 1 for it because we used is 0-based index.
			hr = pfd->SetFileTypeIndex(params.GetDefaultFileTypeIndex() + 1);
			if (!SUCCEEDED(hr)) return false;
		}

		// CoCreate the File Open Dialog object.
		IFileDialog* pfd = NULL;
		HRESULT hr = CoCreateInstance(
			CLSID_FileOpenDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pfd)
		);
		if (SUCCEEDED(hr)) {
			// Set the options on the dialog.
			DWORD dwFlags;

			// Before setting, always get the options first in order 
			// not to override existing options.
			hr = pfd->GetOptions(&dwFlags);
			if (SUCCEEDED(hr)) {
				// In this case, get shell items only for file system items.
				hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
				if (SUCCEEDED(hr)) {
					// Set the file types to display only. 
					// Notice that this is a 1-based array.
					hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
					if (SUCCEEDED(hr)) {
						// Set the selected file type index to Word Docs for this example.
						hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
						if (SUCCEEDED(hr)) {
							// Set the default extension to be ".doc" file.
							hr = pfd->SetDefaultExtension(L"doc;docx");
							if (SUCCEEDED(hr)) {
								// Show the dialog
								hr = pfd->Show(NULL);
								if (SUCCEEDED(hr)) {
									// Obtain the result once the user clicks 
									// the 'Open' button.
									// The result is an IShellItem object.
									IShellItem* psiResult;
									hr = pfd->GetResult(&psiResult);
									if (SUCCEEDED(hr)) {
										// We are just going to print out the 
										// name of the file for sample sake.
										PWSTR pszFilePath = NULL;
										hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
											&pszFilePath);
										if (SUCCEEDED(hr)) {
											TaskDialog(NULL,
												NULL,
												L"CommonFileDialogApp",
												pszFilePath,
												NULL,
												TDCBF_OK_BUTTON,
												TD_INFORMATION_ICON,
												NULL);
											CoTaskMemFree(pszFilePath);
										}
										psiResult->Release();
									}
								}
							}
						}
					}
				}
			}
			pfd->Release();
		}
		return SUCCEEDED(hr);
	}

#pragma endregion

#pragma region Wrapper Functions

	bool OpenFileDialog(const FileDialog& params, std::string& ret) {
		std::vector<std::string> cache;
		bool isok = CommonFileDialog<CommonFileDialogType::OpenFile>(params, cache);
		if (isok) ret = cache.front();
		return isok;
	}
	bool OpenMultipleFileDialog(const FileDialog& params, std::vector<std::string>& ret) {
		return CommonFileDialog<CommonFileDialogType::OpenMultipleFiles>(params, ret);
	}
	bool SaveFileDialog(const FileDialog& params, std::string& ret) {
		std::vector<std::string> cache;
		bool isok = CommonFileDialog<CommonFileDialogType::SaveFile>(params, cache);
		if (isok) ret = cache.front();
		return isok;
	}

	bool OpenFolderDialog(const FileDialog& params, std::string& ret) {
		std::vector<std::string> cache;
		bool isok = CommonFileDialog<CommonFileDialogType::OpenFolder>(params, cache);
		if (isok) ret = cache.front();
		return isok;
	}

#pragma endregion


	//template<bool TIsOpen, bool TMultiSelection>
	//bool GeneralFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret) {
	//	// make sure multi-selection only available in open mode
	//	static_assert(TIsOpen && (!TIsOpen && TMultiSelection == false));

	//	// build filter
	//	std::wstring w_filter;
	//	for (const auto& filter_pair : params.m_Filter) {
	//		w_filter += EncodingHelper::UTF8ToWchar(filter_pair.first.c_str());
	//		w_filter += L'\0';
	//		w_filter += EncodingHelper::UTF8ToWchar(filter_pair.second.c_str());
	//		w_filter += L'\0';
	//	}
	//	// build title
	//	std::wstring w_title(EncodingHelper::UTF8ToWchar(params.m_Title.c_str()));
	//	// build default extension
	//	std::wstring w_default_ext(EncodingHelper::UTF8ToWchar(params.m_DefaultExtension.c_str()));
	//	// build initial directory
	//	std::wstring w_init_dir(EncodingHelper::UTF8ToWchar(params.m_InitialDirectory.c_str()));
	//	// prepare file name receiver and preset it as initial file name
	//	std::wstring path_receiver(EncodingHelper::UTF8ToWchar(params.m_InitialFileName.c_str()));
	//	path_receiver.resize(std::max(MAX_PATH, path_receiver.size()), L'\0');

	//	// prepare the common part of file dialog struct
	//	OPENFILENAMEW dialog_param;
	//	ZeroMemory(&dialog_param, sizeof(OPENFILENAMEW));
	//	dialog_param.lStructSize = sizeof(OPENFILENAMEW);
	//	// dialog owner
	//	dialog_param.hwndOwner = params.m_Owner;
	//	// if no filter, we pass NULL
	//	dialog_param.lpstrFilter = w_filter.empty() ? NULL : w_filter.c_str();
	//	// no record to user selected filter
	//	dialog_param.lpstrCustomFilter = NULL;
	//	dialog_param.nMaxCustFilter = 0;
	//	dialog_param.nFilterIndex = 0;
	//	// path receiver, also is init filename
	//	dialog_param.lpstrFile = path_receiver.data();
	//	dialog_param.nMaxFile = static_caast<DWORD>(path_receiver.size());
	//	// no selected file infos
	//	dialog_param.lpstrFileTitle = NULL;
	//	dialog_param.nMaxFileTitle = 0;
	//	// initial directory
	//	dialog_param.lpstrInitialDir = w_init_dir.empty() ? NULL : w_init_dir.c_str();
	//	// dialog title
	//	dialog_param.lpstrTitle = w_title.empty() ? NULL : w_title.c_str();
	//	// setup basic flags
	//	dialog_param.Flags = OFN_EXPLORER;
	//	// default extension
	//	dialog_param.lpstrDefExt = w_default_ext.empty() ? NULL : w_default_ext.c_str();

	//	BOOL status;
	//	if constexpr (TIsOpen) {
	//		// multi-selection need add special multi-selection flags
	//		if constexpr (TMultiSelection) {
	//			dialog_param.Flags |= OFN_ALLOWMULTISELECT;
	//		}

	//		// call browser
	//		status = GetOpenFileNameW(&dialog_param);

	//		// only process result when success
	//		if (status) {
	//			if constexpr (TMultiSelection) {
	//				// get directory part, copy from start to dialog param specified offset
	//				std::wstring w_directory_part(path_receiver.c_str(), dialog_param.nFileOffset);
	//				// get file names part one by one
	//				size_t filename_cursor = dialog_param.nFileOffset;
	//				while (path_receiver[filename_cursor] != L'\0') {
	//					// init wstring from given offset
	//					std::wstring w_filename_part(path_receiver.c_str() + filename_cursor);
	//					// get eaten chars from result and increase to cursor
	//					filename_cursor += w_filename_part.size() + 1u;
	//					// combine 2 parts and insert into list
	//					ret.emplace_back(std::string(EncodingHelper::WcharToUTF8(w_directory_part + w_filename_part)));
	//				}
	//			} else {
	//				ret.emplace_back(std::string(EncodingHelper::WcharToUTF8(path_receiver.c_str())));
	//			}
	//		}

	//	} else {
	//		// call browser
	//		status = GetSaveFileNameW(&dialog_param);

	//		// only process result when success
	//		if (status) {
	//			ret.emplace_back(std::string(EncodingHelper::WcharToUTF8(path_receiver.c_str())));
	//		}

	//	}

	//	// if failed, clear result
	//	// and return result
	//	if (!status) {
	//		ret.clear();
	//	}
	//	return status == TRUE;
	//}

	//bool OpenFileDialog(const FileDialogParameter& params, std::string& ret) {
	//	std::vector<std::string> cache;
	//	bool isok = GeneralFileDialog<true, false>(params, cache);
	//	if (isok) ret = cache.front();
	//	return isok;
	//}
	//bool OpenMultipleFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret) {
	//	return GeneralFileDialog<true, true>(params, ret);
	//}
	//bool SaveFileDialog(const FileDialogParameter& params, std::string& ret) {
	//	std::vector<std::string> cache;
	//	bool isok = GeneralFileDialog<false, false>(params, cache);
	//	if (isok) ret = cache.front();
	//	return isok;
	//}

	//bool OpenFolderDialog(const FolderDialogParameter& params, std::string& ret) {
	//	// create wchar string cache for windows W-tail function
	//	std::wstring w_title(EncodingHelper::UTF8ToWchar(params.m_Title.c_str()));
	//	// create buffer for receiving selected folder
	//	// initialize with MAX_PATH length and content is filled with zero.
	//	std::wstring path_receiver(MAX_PATH, L'\0');

	//	// prepare folder foalog struct
	//	BROWSEINFOW dialog_param = { 0 };
	//	dialog_param.hwndOwner = params.m_Owner;
	//	dialog_param.pidlRoot = nullptr;
	//	dialog_param.pszDisplayName = path_receiver.data();
	//	dialog_param.lpszTitle = w_title.c_str();
	//	dialog_param.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	//	dialog_param.lpfn = nullptr;

	//	// call browser
	//	PIDLIST_ABSOLUTE place = SHBrowseForFolderW(&dialog_param);
	//	// if browser failed, return.
	//	if (place == nullptr) {
	//		ret.clear();
	//		return false;
	//	}

	//	// get path from browser result
	//	BOOL status;
	//	if (status = SHGetPathFromIDListW(place, path_receiver.data())) {
	//		EncodingHelper::WcharToUTF8(path_receiver.c_str(), ret);
	//	} else {
	//		ret.clear();
	//	}

	//	// clear browser result and return
	//	CoTaskMemFree(place);
	//	return status == TRUE;
	//}

}

#endif
