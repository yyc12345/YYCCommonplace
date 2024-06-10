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

	//template<typename _Ty, std::enable_if_t<std::is_same_v<_Ty, char> || std::is_same_v<_Ty, wchar_t>, int> = 0>
	//static bool FetchEOL(std::basic_string<_Ty>& internal_buffer, std::basic_string<_Ty>& result_buffer) {
	//	// try finding EOL in internal buffer
	//	size_t pos;
	//	if constexpr (std::is_same_v<_Ty, char>) internal_buffer.find_first_of('\n');
	//	else internal_buffer.find_first_of(L'\n');

	//	// check finding result
	//	if (pos == std::wstring::npos) {
	//		// the whole string do not include EOL, fully appended to return value
	//		result_buffer += internal_buffer;
	//		internal_buffer.clear();
	//		// return false mean need more data
	//		return false;
	//	} else {
	//		// split result
	//		// push into result and remain some in internal buffer.
	//		result_buffer.append(internal_buffer, 0u, pos);
	//		internal_buffer.erase(0u, pos + 1u); // +1 because EOL take one place.
	//		// return true mean success finding
	//		return true;
	//	}
	//}

	template<bool _bIsConsole>
	static std::string PlainRead(HANDLE hStdIn) {
		using _TChar = std::conditional_t<_bIsConsole, wchar_t, char>;

		// Prepare an internal buffer because the read data may not be fully used.
		// For example, we may read x\ny in a single calling but after processing \n, this function will return
		// so y will temporarily stored in this internal buffer for next using.
		// Thus this function is not thread safe.
		static std::basic_string<_TChar> internal_buffer;
		// create return value buffer
		std::basic_string<_TChar> return_buffer;

		// Prepare some variables
		DWORD dwReadNumberOfChars;
		_TChar szReadChars[64];
		size_t eol_pos;

		// try fetching EOL
		while (true) {
			// if internal buffer is empty, 
			// try fetching it.
			if (internal_buffer.empty()) {
				// console and non-console use different method to read.
				if constexpr (_bIsConsole) {
					// console handle, use ReadConsoleW.
					// read from console, the read data is wchar based
					if (!ReadConsoleW(hStdIn, szReadChars, sizeof(szReadChars) / sizeof(_TChar), &dwReadNumberOfChars, NULL))
						break;
				} else {
					// anything else, use ReadFile instead.
					// the read data is utf8 based
					if (!ReadFile(hStdIn, szReadChars, sizeof(szReadChars), &dwReadNumberOfChars, NULL))
						break;
				}

				// send to internal buffer
				if (dwReadNumberOfChars == 0) break;
				internal_buffer.append(szReadChars, dwReadNumberOfChars);
			}

			// try finding EOL in internal buffer
			if constexpr (std::is_same_v<_TChar, char>) eol_pos = internal_buffer.find_first_of('\n');
			else eol_pos = internal_buffer.find_first_of(L'\n');
			// check finding result
			if (eol_pos == std::wstring::npos) {
				// the whole string do not include EOL, fully appended to return value
				return_buffer += internal_buffer;
				internal_buffer.clear();
				// need more data, continue while
			} else {
				// split result
				// push into result and remain some in internal buffer.
				return_buffer.append(internal_buffer, 0u, eol_pos);
				internal_buffer.erase(0u, eol_pos + 1u); // +1 because EOL take one place.
				// break while mean success finding
				break;
			}
		}

		// post-process for return value
		std::string real_return_buffer;
		if constexpr (_bIsConsole) {
			// console mode need convert wchar to utf8
			YYCC::EncodingHelper::WcharToUTF8(return_buffer.c_str(), real_return_buffer);
		} else {
			// non-console just copt the result
			real_return_buffer = return_buffer;
		}
		// every mode need delete \r words
		YYCC::StringHelper::Replace(real_return_buffer, "\r", "");
		// return value
		return real_return_buffer;
	}

	std::string ReadLine() {
#if YYCC_OS == YYCC_OS_WINDOWS

		// get stdin mode
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		// use different method to get according to whether stdin is redirected
		DWORD dwConsoleMode;
		if (GetConsoleMode(hStdIn, &dwConsoleMode)) {
			return PlainRead<true>(hStdIn);
		} else {
			return PlainRead<false>(hStdIn);
		}

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

		// Prepare some Win32 variables
		// fetch stdout handle first
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwConsoleMode;
		DWORD dwWrittenNumberOfChars;

		// if stdout was redirected, this handle may point to a file handle or anything else,
		// WriteConsoleW can not write data into such scenario, so we need check whether this handle is console handle
		if (GetConsoleMode(hStdOut, &dwConsoleMode)) {
			// console handle, use WriteConsoleW.
			// convert utf8 string to wide char first
			std::wstring wstrl(YYCC::EncodingHelper::UTF8ToWchar(strl.c_str()));
			size_t wstrl_size = wstrl.size();
			// write string with size check
			if (wstrl_size <= std::numeric_limits<DWORD>::max()) {
				WriteConsoleW(hStdOut, wstrl.c_str(), static_cast<DWORD>(wstrl_size), &dwWrittenNumberOfChars, NULL);
			}
		} else {
			// anything else, use WriteFile instead.
			// WriteFile do not need extra convertion, because it is direct writing.
			// check whether string length is overflow
			size_t strl_size = strl.size() * sizeof(std::string::value_type);
			// write string with size check
			if (strl_size <= std::numeric_limits<DWORD>::max()) {
				WriteFile(hStdOut, strl.c_str(), static_cast<DWORD>(strl_size), &dwWrittenNumberOfChars, NULL);
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
