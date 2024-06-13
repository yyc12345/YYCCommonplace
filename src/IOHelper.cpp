#include "IOHelper.hpp"

#include "EncodingHelper.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <stdexcept>

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include "WinImportSuffix.hpp"
#endif

namespace YYCC::IOHelper {

	FILE* UTF8FOpen(const char* u8_filepath, const char* u8_mode) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// convert mode and file path to wchar
		std::wstring wmode, wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_mode, wmode))
			return nullptr;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_filepath, wpath))
			return nullptr;

		// call microsoft specified fopen which support wchar as argument.
		return _wfopen(wpath.c_str(), wmode.c_str());

#else
		return std::fopen(u8_filepath, u8_mode);
#endif
	}

	std::filesystem::path UTF8Path(const char* u8_path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// convert path to wchar
		std::wstring wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_path, wpath))
			throw std::invalid_argument("Fail to convert given UTF8 string.");

		// call microsoft specified fopen which support wchar as argument.
		return std::filesystem::path(wpath);

#else
		return std::filesystem::path(u8_path);
#endif
	}


}

