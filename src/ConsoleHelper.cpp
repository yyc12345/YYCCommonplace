#include "ConsoleHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"
#include <iostream>

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "WinImportSuffix.hpp"

namespace YYCC::ConsoleHelper {

	static bool RawEnableColorfulConsole(FILE* fs) {
		if (!_isatty(_fileno(fs))) return false;

		HANDLE h_output;
		DWORD dw_mode;

		h_output = (HANDLE)_get_osfhandle(_fileno(fs));
		if (!GetConsoleMode(h_output, &dw_mode)) return false;
		if (!SetConsoleMode(h_output, dw_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT)) return false;

		return true;
	}

	bool EnableColorfulConsole(FILE* fs) {
		if (!RawEnableColorfulConsole(stdout)) return false;
		if (!RawEnableColorfulConsole(stderr)) return false;
		return true;
	}

	std::string ReadLine() {
#if YYCC_OS == YYCC_OS_WINDOWS

		return std::string();

#elif YYCC_OS == YYCC_OS_LINUX

		// in linux, directly use C++ function to fetch.
		std::string cmd;
		if (std::getline(std::cin, cmd).fail()) cmd.clear();
		return cmd;

#endif
	}

	void Write(const char* u8_strl) {}

	static void PlainWrite(const std::string& strl) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// fetch stdout handle first
		HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
		// if stdout was redirected, this handle may point to a file handle or anything else,
		// WriteConsoleW can not write data into such scenario, so we need check whether this handle is console handle
		DWORD console_mode;
		if (GetConsoleMode(hstdout, &console_mode)) {
			// console handle, use WriteConsoleW.
			// convert utf8 string to wide char first
			std::wstring wstrl(YYCC::EncodingHelper::UTF8ToWchar(strl.c_str()));
			size_t wstrl_size = wstrl.size();
			// write string with size check
			if (wstrl_size <= std::numeric_limits<DWORD>::max()) {
			WriteConsoleW(hstdout, wstrl.c_str(), static_cast<DWORD>(wstrl_size), NULL, NULL);
			}
		} else {
			// anything else, use WriteFile instead.
			// WriteFile do not need extra convertion, because it is direct writing.
			// check whether string length is overflow
			size_t strl_size = strl.size() * sizeof(std::string::value_type);
			// write string with size check
			if (strl_size <= std::numeric_limits<DWORD>::max()) {
				WriteFile(hstdout, strl.c_str(), static_cast<DWORD>(strl_size), NULL, NULL);
			}
		}

#elif YYCC_OS == YYCC_OS_LINUX

		// in linux, directly use C function to write.
		std::fputs(strl.c_str(), stdout);

#endif
	}

	void Write(const char* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		PlainWrite(YYCC::StringHelper::VPrintf(u8_fmt, argptr));
		va_end(argptr);
	}

	void WriteLine(const char* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		std::string cache(YYCC::StringHelper::VPrintf(u8_fmt, argptr));
		cache += "\n";
		PlainWrite(cache);
		va_end(argptr);
	}

}

#endif
