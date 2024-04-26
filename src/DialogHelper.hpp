#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>
#include <vector>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"

namespace YYCC::DialogHelper {

	struct FileDialogFilterEntry {
		std::string FileType;
		std::string FileExtension;
	};
	using FileDialogFilter = std::vector<FileDialogFilterEntry>;

	bool OpenFileDialog(HWND parent, const char* title, const FileDialogFilter& filter, std::string& ret);
	bool OpenMultipleFileDialog(HWND parent, const char* title, const FileDialogFilter& filter, std::vector<std::string>& ret);
	bool SaveFileDialog(HWND parent, const char* title, const FileDialogFilter& filter, std::string& ret);

	bool OpenFolderDialog(HWND parent, std::string& ret);

}

#endif
