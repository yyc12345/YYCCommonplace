#include "ConsoleHelper.hpp"

#include "EncodingHelper.hpp"
#include "StringHelper.hpp"
#include <iostream>

// Include Windows used headers in Windows.
#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "WinImportSuffix.hpp"
#endif

namespace YYCC::ConsoleHelper {

#pragma region Windows Specific Functions
#if YYCC_OS == YYCC_OS_WINDOWS

	static bool RawEnableColorfulConsole(FILE* fs) {
		if (!_isatty(_fileno(fs))) return false;

		HANDLE h_output;
		DWORD dw_mode;

		h_output = (HANDLE)_get_osfhandle(_fileno(fs));
		if (!GetConsoleMode(h_output, &dw_mode)) return false;
		if (!SetConsoleMode(h_output, dw_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT)) return false;

		return true;
	}

	/*
	Reference:
	* https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
	* https://stackoverflow.com/questions/69830460/reading-utf-8-input

	There is 3 way to make Windows console enable UTF8 mode.

	First one is calling SetConsoleCP and SetConsoleOutputCP.
	The side effect of this is std::cin and std::cout is broken,
	however there is a patch for this issue.

	Second one is calling _set_mode with _O_U8TEXT or _O_U16TEXT to enable Unicode mode for Windows console.
	This also have side effect which is stronger than first one.
	All puts family functions (ASCII-based output functions) will throw assertion exception.
	You only can use putws family functions (wide-char-based output functions).
	However these functions can not be used without calling _set_mode in Windows design.

	There still is another method, using WriteConsoleW directly visiting console.
	This function family can output correct string without calling any extra functions!
	This method is what we adopted.
	*/

	template<bool _bIsConsole>
	static yycc_u8string WinConsoleRead(HANDLE hStdIn) {
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
		yycc_u8string real_return_buffer;
		if constexpr (_bIsConsole) {
			// console mode need convert wchar to utf8
			YYCC::EncodingHelper::WcharToUTF8(return_buffer, real_return_buffer);
		} else {
			// non-console just copt the result
			real_return_buffer = EncodingHelper::ToUTF8(return_buffer);
		}
		// every mode need delete \r words
		YYCC::StringHelper::Replace(real_return_buffer, YYCC_U8("\r"), YYCC_U8(""));
		// return value
		return real_return_buffer;
	}

	static void WinConsoleWrite(const yycc_u8string& strl, bool to_stderr) {
		// Prepare some Win32 variables
		// fetch stdout handle first
		HANDLE hStdOut = GetStdHandle(to_stderr ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
		DWORD dwConsoleMode;
		DWORD dwWrittenNumberOfChars;

		// if stdout was redirected, this handle may point to a file handle or anything else,
		// WriteConsoleW can not write data into such scenario, so we need check whether this handle is console handle
		if (GetConsoleMode(hStdOut, &dwConsoleMode)) {
			// console handle, use WriteConsoleW.
			// convert utf8 string to wide char first
			std::wstring wstrl(YYCC::EncodingHelper::UTF8ToWchar(strl));
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
	}

#endif
#pragma endregion

	bool EnableColorfulConsole() {
#if YYCC_OS == YYCC_OS_WINDOWS

		bool ret = true;
		ret &= RawEnableColorfulConsole(stdout);
		ret &= RawEnableColorfulConsole(stderr);
		return ret;

#else

		// just return true and do nothing
		return true;

#endif
	}

	yycc_u8string ReadLine() {
#if YYCC_OS == YYCC_OS_WINDOWS

		// get stdin mode
		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		// use different method to get according to whether stdin is redirected
		DWORD dwConsoleMode;
		if (GetConsoleMode(hStdIn, &dwConsoleMode)) {
			return WinConsoleRead<true>(hStdIn);
		} else {
			return WinConsoleRead<false>(hStdIn);
		}
		
#else

		// in linux, directly use C++ function to fetch.
		std::string cmd;
		if (std::getline(std::cin, cmd).fail()) cmd.clear();
		return EncodingHelper::ToUTF8(cmd);

#endif
	}

	template<bool bNeedFmt, bool bIsErr, bool bHasEOL>
	static void RawWrite(const yycc_char8_t* u8_fmt, va_list argptr) {
		// Buiild string need to be written first
		// If no format string or plain string for writing, return.
		if (u8_fmt == nullptr) return;
		// Build or simply copy string
		yycc_u8string strl;
		if constexpr (bNeedFmt) {
			// treat as format string
			va_list argcpy;
			va_copy(argcpy, argptr);
			strl = YYCC::StringHelper::VPrintf(u8_fmt, argcpy);
			va_end(argcpy);
		} else {
			// treat as plain string
			strl = u8_fmt;
		}
		// Checkout whether add EOL
		if constexpr (bHasEOL) {
			strl += YYCC_U8("\n");
		}
		
#if YYCC_OS == YYCC_OS_WINDOWS
		// call Windows specific writer
		WinConsoleWrite(strl, bIsErr);
#else
		// in linux, directly use C function to write.
		std::fputs(EncodingHelper::ToNative(strl.c_str()), bIsErr ? stderr : stdout);
#endif
	}

	void Format(const yycc_char8_t* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		RawWrite<true, false, false>(u8_fmt, argptr);
		va_end(argptr);
	}

	void FormatLine(const yycc_char8_t* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		RawWrite<true, false, true>(u8_fmt, argptr);
		va_end(argptr);
	}

	void Write(const yycc_char8_t* u8_strl) {
		va_list empty{};
		RawWrite<false, false, false>(u8_strl, empty);
	}
	
	void WriteLine(const yycc_char8_t* u8_strl) {
		va_list empty{};
		RawWrite<false, false, true>(u8_strl, empty);
	}
	
	void ErrFormat(const yycc_char8_t* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		RawWrite<true, true, false>(u8_fmt, argptr);
		va_end(argptr);
	}

	void ErrFormatLine(const yycc_char8_t* u8_fmt, ...) {
		va_list argptr;
		va_start(argptr, u8_fmt);
		RawWrite<true, true, true>(u8_fmt, argptr);
		va_end(argptr);
	}

	void ErrWrite(const yycc_char8_t* u8_strl) {
		va_list empty{};
		RawWrite<false, true, false>(u8_strl, empty);
	}
	
	void ErrWriteLine(const yycc_char8_t* u8_strl) {
		va_list empty{};
		RawWrite<false, true, true>(u8_strl, empty);
	}

}

