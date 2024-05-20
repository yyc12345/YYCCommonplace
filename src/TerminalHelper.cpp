#include "TerminalHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"
#include <iostream>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "WinImportSuffix.hpp"

namespace YYCC::TerminalHelper {

	bool EnsureTerminalColor(FILE* fs) {
		if (!_isatty(_fileno(fs))) return false;

		HANDLE h_output;
		DWORD dw_mode;

		h_output = (HANDLE)_get_osfhandle(_fileno(fs));
		if (!GetConsoleMode(h_output, &dw_mode)) return false;
		if (!SetConsoleMode(h_output, dw_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return false;

		return true;
	}

	bool EnsureTerminalUTF8(FILE* fs) {
		if (!SetConsoleCP(CP_UTF8)) return false;
		if (!SetConsoleOutputCP(CP_UTF8)) return false;

		/*_setmode(_fileno(stdout), _O_U8TEXT);*/
		int _ = _setmode(_fileno(fs), _O_U16TEXT);
		return true;
	}

	bool FGets(std::string& u8_buf, FILE* stream) {
		std::wstring wcmd;
		if (std::getline(std::wcin, wcmd).fail()) return false;
		YYCC::EncodingHelper::WcharToChar(wcmd.c_str(), u8_buf, CP_UTF8);
		return true;
	}

	void FPuts(const char* u8_buf, FILE* stream) {
		std::fputws(YYCC::EncodingHelper::UTF8ToWchar(u8_buf).c_str(), stream);
	}

	void FPrintf(FILE* stream, const char* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		FPuts(YYCC::StringHelper::VPrintf(u8_fmt, argptr).c_str(), stream);
		va_end(argptr);
	}
	
}

#endif
