#include "csconsole.hpp"
#include "../macro/os_detector.hpp"

#include "../string/op.hpp"
#include "../string/reinterpret.hpp"
#include "../encoding/windows.hpp"
#include <iostream>
#include <cstdarg>
#include <cstdio>

#if defined(YYCC_OS_WINDOWS)
#include "../windows/import_guard_head.hpp"
#include <Windows.h>
#include "../windows/import_guard_tail.hpp"
#endif

#define OP ::yycc::string::op
#define REINTERPRET ::yycc::string::reinterpret
#define ENC ::yycc::encoding::windows

namespace yycc::carton::csconsole {

#pragma region Windows Console Specific Functions
#if defined(YYCC_OS_WINDOWS)

    template<bool BIsConsole>
    static std::u8string win_console_read(HANDLE hStdIn) {
        using TChar = std::conditional_t<BIsConsole, wchar_t, char>;

        // Prepare an internal buffer because the read data may not be fully used.
        // For example, we may read x\ny in a single calling but after processing \n, this function will return
        // so y will temporarily stored in this internal buffer for next using.
        // Thus this function is not thread safe.
        static std::basic_string<TChar> internal_buffer;
        // create return value buffer
        std::basic_string<TChar> return_buffer;

        // Prepare some variables
        DWORD dwReadNumberOfChars;
        TChar szReadChars[64];
        size_t eol_pos;

        // try fetching EOL
        while (true) {
            // if internal buffer is empty,
            // try fetching it.
            if (internal_buffer.empty()) {
                // console and non-console use different method to read.
                if constexpr (BIsConsole) {
                    // console handle, use ReadConsoleW.
                    // read from console, the read data is wchar based
                    if (!ReadConsoleW(hStdIn, szReadChars, sizeof(szReadChars) / sizeof(TChar), &dwReadNumberOfChars, NULL)) break;
                } else {
                    // anything else, use ReadFile instead.
                    // the read data is utf8 based
                    if (!ReadFile(hStdIn, szReadChars, sizeof(szReadChars), &dwReadNumberOfChars, NULL)) break;
                }

                // send to internal buffer
                if (dwReadNumberOfChars == 0) break;
                internal_buffer.append(szReadChars, dwReadNumberOfChars);
            }

            // try finding EOL in internal buffer
            if constexpr (std::is_same_v<TChar, char>) eol_pos = internal_buffer.find_first_of('\n');
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
        std::u8string real_return_buffer;
        if constexpr (BIsConsole) {
            // console mode need convert wchar to utf8
            auto rv = ENC::to_utf8(return_buffer);
            if (rv.has_value()) real_return_buffer = std::move(rv.value());
        } else {
            // non-console just copt the result
            real_return_buffer = REINTERPRET::as_utf8(return_buffer);
        }
        // every mode need delete \r words
        OP::replace(real_return_buffer, u8"\r", u8"");
        // return value
        return real_return_buffer;
    }

    static void win_console_write(const std::u8string_view& strl, bool to_stderr) {
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
            std::wstring wstrl = ENC::to_wchar(strl).value();
            size_t wstrl_size = wstrl.size();
            // write string with size check
            if (wstrl_size <= std::numeric_limits<DWORD>::max()) {
                WriteConsoleW(hStdOut, wstrl.c_str(), static_cast<DWORD>(wstrl_size), &dwWrittenNumberOfChars, NULL);
            }
        } else {
            // anything else, use WriteFile instead.
            // WriteFile do not need extra convertion, because it is direct writing.
            // check whether string length is overflow
            size_t strl_size = strl.size() * sizeof(std::u8string_view::value_type);
            // write string with size check
            if (strl_size <= std::numeric_limits<DWORD>::max()) {
                WriteFile(hStdOut, strl.data(), static_cast<DWORD>(strl_size), &dwWrittenNumberOfChars, NULL);
            }
        }
    }

#endif
#pragma endregion

#pragma region Read Functions

    std::u8string read_line() {
#if defined(YYCC_OS_WINDOWS)

        // get stdin mode
        HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
        // use different method to get according to whether stdin is redirected
        DWORD dwConsoleMode;
        if (GetConsoleMode(hStdIn, &dwConsoleMode)) {
            return win_console_read<true>(hStdIn);
        } else {
            return win_console_read<false>(hStdIn);
        }
        
#else

        // in linux, directly use C++ function to fetch.
        std::string cmd;
        if (std::getline(std::cin, cmd).fail()) cmd.clear();
        return REINTERPRET::as_utf8(cmd);

#endif
    }

#pragma endregion

#pragma region Write Functions

    template<bool BNeedFmt, bool BIsErr, bool BHasEOL>
    static void raw_write(const char8_t* u8_fmt, va_list argptr) {
        // Buiild string need to be written first
        // If no format string or plain string for writing, return.
        if (u8_fmt == nullptr) return;
        // Build or simply copy string
        std::u8string strl;
        if constexpr (BNeedFmt) {
            // treat as format string
            va_list argcpy;
            va_copy(argcpy, argptr);
            strl = OP::vprintf(u8_fmt, argcpy);
            va_end(argcpy);
        } else {
            // treat as plain string
            strl = u8_fmt;
        }
        // Checkout whether add EOL
        if constexpr (BHasEOL) {
            strl += u8'\n';
        }
        
#if defined(YYCC_OS_WINDOWS)
        // call Windows specific writer
        win_console_write(strl, BIsErr);
#else
        // in linux, directly use C function to write.
        std::fputs(REINTERPRET::as_ordinary(strl.c_str()), BIsErr ? stderr : stdout);
#endif
    }

    void format(const char8_t* u8_fmt, ...) {
        va_list argptr;
        va_start(argptr, u8_fmt);
        raw_write<true, false, false>(u8_fmt, argptr);
        va_end(argptr);
    }

    void format_line(const char8_t* u8_fmt, ...) {
        va_list argptr;
        va_start(argptr, u8_fmt);
        raw_write<true, false, true>(u8_fmt, argptr);
        va_end(argptr);
    }

    void write(const char8_t* u8_strl) {
        va_list empty{};
        raw_write<false, false, false>(u8_strl, empty);
    }

    void write_line(const char8_t* u8_strl) {
        va_list empty{};
        raw_write<false, false, true>(u8_strl, empty);
    }

    void eformat(const char8_t* u8_fmt, ...) {
        va_list argptr;
        va_start(argptr, u8_fmt);
        raw_write<true, true, false>(u8_fmt, argptr);
        va_end(argptr);
    }

    void eformat_line(const char8_t* u8_fmt, ...) {
        va_list argptr;
        va_start(argptr, u8_fmt);
        raw_write<true, true, true>(u8_fmt, argptr);
        va_end(argptr);
    }

    void ewrite(const char8_t* u8_strl) {
        va_list empty{};
        raw_write<false, true, false>(u8_strl, empty);
    }

    void ewrite_line(const char8_t* u8_strl) {
        va_list empty{};
        raw_write<false, true, true>(u8_strl, empty);
    }

#pragma endregion

}
