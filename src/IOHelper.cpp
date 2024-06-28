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

	FILE* UTF8FOpen(const yycc_char8_t* u8_filepath, const yycc_char8_t* u8_mode) {
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

}

