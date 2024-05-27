#include "DialogHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"

namespace YYCC::DialogHelper {

#pragma region COM Guard

	class ComGuard {
	public:
		ComGuard() : m_HasInit(false) {
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (SUCCEEDED(hr)) m_HasInit = true;
		}
		~ComGuard() {
			if (m_HasInit) {
				CoUninitialize();
			}
		}

	protected:
		bool m_HasInit;
	};

	static const ComGuard c_ComGuard;

#pragma endregion


#pragma region FileFilters

	bool FileFilters::Add(const char* filter_name, std::initializer_list<const char*> il) {
		// assign filter name
		if (filter_name == nullptr) return false;
		FilterName name(filter_name);

		// assign filter patterns
		FilterModes modes;
		for (const char* pattern : il) {
			if (pattern != nullptr)
				modes.emplace_back(std::string(pattern));
		}

		// check filter patterns
		if (modes.empty()) return false;

		// add into pairs and return
		m_Filters.emplace_back(std::make_pair(name, modes));
		return true;
	}

	bool FileFilters::Generate(WinFileFilters& win_result) const {
		// clear Windows oriented data
		win_result.Clear();

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
		// clear Windows oriented data
		win_result.Clear();

		// set owner
		win_result.m_WinOwner = m_Owner;

		// build file filters
		if (!m_FileTypes.Generate(win_result.m_WinFileTypes))
			return false;

		// check default file type index
		// check value overflow (comparing with >= because we need plus 1 for file type index later)
		if (m_DefaultFileTypeIndex >= std::numeric_limits<UINT>::max())
			return false;
		// check invalid index (overflow the length or registered file types if there is file type)
		if (m_FileTypes.Count() != 0u && m_DefaultFileTypeIndex >= m_FileTypes.Count())
			return false;
		// set index with additional plus according to Windows specification.
		win_result.m_WinDefaultFileTypeIndex = static_cast<UINT>(m_DefaultFileTypeIndex + 1);

		// build title and init file name
		if (m_HasTitle) {
			if (!YYCC::EncodingHelper::UTF8ToWchar(m_Title.c_str(), win_result.m_WinTitle))
				return false;
			win_result.m_HasTitle = true;
		}
		if (m_HasInitFileName) {
			if (!YYCC::EncodingHelper::UTF8ToWchar(m_InitFileName.c_str(), win_result.m_WinInitFileName))
				return false;
			win_result.m_HasInitFileName = true;
		}

		// fetch init directory
		if (m_HasInitDirectory) {
			// convert to wpath
			std::wstring w_init_directory;
			if (!YYCC::EncodingHelper::UTF8ToWchar(m_InitDirectory.c_str(), w_init_directory))
				return false;

			// fetch IShellItem*
			// Ref: https://stackoverflow.com/questions/76306324/how-to-set-default-folder-for-ifileopendialog-interface
			IShellItem* init_directory = NULL;
			HRESULT hr = SHCreateItemFromParsingName(w_init_directory.c_str(), NULL, IID_PPV_ARGS(&init_directory));
			if (FAILED(hr)) return false;

			// assign IShellItem*
			win_result.m_WinInitDirectory.reset(init_directory);
		}

		// everything is okey
		return true;
	}

#pragma endregion

#pragma region Windows Dialog Code

	enum class CommonFileDialogType {
		OpenFile,
		OpenMultipleFiles,
		SaveFile,
		OpenFolder
	};

	bool ExtractDisplayName(IShellItem* item, std::string& ret) {
		// fetch display name from IShellItem*
		LPWSTR _name;
		HRESULT hr = item->GetDisplayName(SIGDN_FILESYSPATH, &_name);
		if (FAILED(hr)) return false;
		SmartLPWSTR display_name(_name);

		// convert result
		if (!YYCC::EncodingHelper::WcharToUTF8(display_name.get(), ret))
			return false;

		// finished
		return true;
	}

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
		if (FAILED(hr)) return false;
		// create memory-safe dialog pointer
		SmartIFileDialog pfd(_pfd);

		// set options for dialog
		// before setting, always get the options first in order.
		// not to override existing options.
		DWORD dwFlags;
		hr = pfd->GetOptions(&dwFlags);
		if (FAILED(hr)) return false;
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
		if (FAILED(hr)) return false;

		// build Windows used file dialog parameters
		WinFileDialog win_params;
		if (!params.Generate(win_params))
			return false;

		// setup title and init file name
		if (win_params.HasTitle()) {
			hr = pfd->SetTitle(win_params.GetTitle());
			if (FAILED(hr)) return false;
		}
		if (win_params.HasInitFileName()) {
			hr = pfd->SetFileName(win_params.GetInitFileName());
			if (FAILED(hr)) return false;
		}

		// setup init directory
		if (win_params.HasInitDirectory()) {
			hr = pfd->SetFolder(win_params.GetInitDirectory());
		}

		// set file types and default file index when we picking file
		if constexpr (EDialogType != CommonFileDialogType::OpenFolder) {
			// set file types list
			const auto& file_filters = win_params.GetFileTypes();
			hr = pfd->SetFileTypes(file_filters.GetFilterCount(), file_filters.GetFilterSpecs());
			if (FAILED(hr)) return false;

			// set default file type index
			hr = pfd->SetFileTypeIndex(win_params.GetDefaultFileTypeIndex());
			if (FAILED(hr)) return false;
		}

		// show the dialog
		hr = pfd->Show(win_params.HasOwner() ? win_params.GetOwner() : nullptr);
		if (FAILED(hr)) return false;

		// obtain result when user click "OK" button.
		switch (EDialogType) {
			case CommonFileDialogType::OpenFile:
			case CommonFileDialogType::OpenFolder:
			case CommonFileDialogType::SaveFile:
			{
				// obtain one file entry
				IShellItem* _item;
				hr = pfd->GetResult(&_item);
				if (FAILED(hr)) return false;
				SmartIShellItem result_item(_item);

				// extract display name
				std::string result_name;
				if (!ExtractDisplayName(result_item.get(), result_name))
					return false;

				// append result
				ret.emplace_back(std::move(result_name));
			}
			break;
			case CommonFileDialogType::OpenMultipleFiles:
			{
				// try casting file dialog to file open dialog
				// Ref: https://learn.microsoft.com/en-us/windows/win32/learnwin32/asking-an-object-for-an-interface
				IFileOpenDialog* _pfod = nullptr;
				hr = pfd->QueryInterface(IID_PPV_ARGS(&_pfod));
				if (FAILED(hr)) return false;
				SmartIFileOpenDialog pfod(_pfod);

				// obtain multiple file entires
				IShellItemArray* _items;
				hr = pfod->GetResults(&_items);
				if (FAILED(hr)) return false;
				SmartIShellItemArray result_items(_items);

				// analyze file entries
				// get array count first
				DWORD result_items_count = 0u;
				hr = result_items->GetCount(&result_items_count);
				if (FAILED(hr)) return false;
				// iterate array
				for (DWORD i = 0u; i < result_items_count; ++i) {
					// fetch item by index
					IShellItem* _item;;
					hr = result_items->GetItemAt(i, &_item);
					if (FAILED(hr)) return false;
					SmartIShellItem result_item(_item);

					// extract display name
					std::string result_name;
					if (!ExtractDisplayName(result_item.get(), result_name))
						return false;

					// append result
					ret.emplace_back(std::move(result_name));
				}
			}
			break;
			default:
				return false;
		}

		// everything is okey
		return true;
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

}

#endif
