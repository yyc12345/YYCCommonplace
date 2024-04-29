#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>
#include <vector>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shlobj_core.h>
#include <commdlg.h>
#include "WinImportSuffix.hpp"

namespace YYCC::DialogHelper {

	struct FileDialogParameter {
		FileDialogParameter() :
			m_Owner(nullptr),
			m_Filter(), m_SelectedFilter(0),
			m_Title(),
			m_DefaultExtension(), m_InitialDirectory(), m_InitialFileName() {}

		HWND m_Owner;
		std::vector<std::pair<u8string, u8string>> m_Filter;
		size_t m_SelectedFilter;
		u8string m_Title;
		u8string m_DefaultExtension;
		u8string m_InitialFileName;
		u8string m_InitialDirectory;
	};

	struct FolderDialogParameter {
		FolderDialogParameter() :
			m_Owner(nullptr),
			m_Title() {}

		HWND m_Owner;
		u8string m_Title;
	}

	bool OpenFileDialog(const FileDialogParameter& params, std::string& ret);
	bool OpenMultipleFileDialog(const FileDialogParameter& params, std::vector<std::string>& ret);
	bool SaveFileDialog(const FileDialogParameter& params, std::string& ret);

	bool OpenFolderDialog(const FolderDialogParameter& params, std::string& ret);

}

#endif
