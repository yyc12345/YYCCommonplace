#include "IOHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include <cstdio>
#include <iostream>
#include <string>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "WinImportSuffix.hpp"

namespace YYCC::IOHelper {

	bool futf8(FILE* fs) {
		// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setmode?view=msvc-170
		return _setmode(_fileno(fs), _O_U8TEXT) != -1;
	}

	FILE* fopen(const char* u8_filepath, const char* u8_mode) {
		// convert mode and file path to wchar
		std::wstring wmode, wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_mode, wmode))
			return nullptr;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_filepath, wpath))
			return nullptr;

		// call microsoft specified fopen which support wchar as argument.
		return _wfopen(wpath.c_str(), wmode.c_str());
	}

}

#endif
