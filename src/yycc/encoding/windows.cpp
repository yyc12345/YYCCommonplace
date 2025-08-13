#include "windows.hpp"

#if defined(YYCC_OS_WINDOWS)

#include "../string/reinterpret.hpp"
#include <limits>
#include <stdexcept>
#include <cuchar>

#include "../windows/import_guard_head.hpp"
#include <Windows.h>
#include "../windows/import_guard_tail.hpp"

#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret

namespace yycc::encoding::windows {

#pragma region WideCharToMultiByte and MultiByteToWideChar stuff

    // WChar -> Char
    ConvResult<std::string> to_char(const std::wstring_view& src, CodePage code_page) {
        // prepare result
        std::string dst;

        // if src is empty, direct output
        if (src.empty()) {
            return dst;
        }

        // init WideCharToMultiByte used variables
        // setup src pointer
        LPCWCH lpWideCharStr = reinterpret_cast<LPCWCH>(src.data());
        // check whether source string is too large.
        size_t cSrcSize = src.size();
        if (cSrcSize > std::numeric_limits<int>::max()) return std::unexpected(ConvError::TooLargeLength);
        int cchWideChar = static_cast<int>(src.size());

        // do convertion
        // do a dry-run first to fetch desired size.
        int desired_size = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
        if (desired_size <= 0) return std::unexpected(ConvError::NoDesiredSize);
        // resize dest for receiving result
        dst.resize(static_cast<size_t>(desired_size));
        // do real convertion
        int write_result
            = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, reinterpret_cast<LPSTR>(dst.data()), desired_size, NULL, NULL);
        if (write_result <= 0) return std::unexpected(ConvError::BadWrittenSize);

        return dst;
    }

    // Char -> WChar
    ConvResult<std::wstring> to_wchar(const std::string_view& src, CodePage code_page) {
        // prepare result
        std::wstring dst;

        // if src is empty, direct output
        if (src.empty()) {
            return dst;
        }

        // init WideCharToMultiByte used variables
        // setup src pointer
        LPCCH lpMultiByteStr = reinterpret_cast<LPCCH>(src.data());
        // check whether source string is too large.
        size_t cSrcSize = src.size();
        if (cSrcSize > std::numeric_limits<int>::max()) return std::unexpected(ConvError::TooLargeLength);
        int cbMultiByte = static_cast<int>(src.size());

        // do convertion
        // do a dry-run first to fetch desired size.
        int desired_size = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, NULL, 0);
        if (desired_size <= 0) return std::unexpected(ConvError::NoDesiredSize);
        // resize dest for receiving result
        dst.resize(static_cast<size_t>(desired_size));
        // do real convertion
        int write_result = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, reinterpret_cast<LPWSTR>(dst.data()), desired_size);
        if (write_result <= 0) return std::unexpected(ConvError::BadWrittenSize);

        return dst;
    }

    // Char -> Char
    ConvResult<std::string> to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page) {
        auto first_rv = to_wchar(src, src_code_page);
        return first_rv.and_then([dst_code_page](const auto& src) { return to_char(src, dst_code_page); });
    }

    // WChar -> UTF8
    ConvResult<std::u8string> to_utf8(const std::wstring_view& src) {
        auto rv = to_char(src, CP_UTF8);
        return rv.transform([](const auto& dst) { return NS_YYCC_STRING_REINTERPRET::as_utf8(dst); });
    }

    // UTF8 -> WChar
    ConvResult<std::wstring> to_wchar(const std::u8string_view& src) {
        return to_wchar(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(src), CP_UTF8);
    }

    // Char -> UTF8
    ConvResult<std::u8string> to_utf8(const std::string_view& src, CodePage code_page) {
        auto rv = to_char(src, code_page, CP_UTF8);
        return rv.transform([](const auto& dst) { return NS_YYCC_STRING_REINTERPRET::as_utf8(dst); });
    }

    // UTF8 -> Char
    ConvResult<std::string> to_char(const std::u8string_view& src, CodePage code_page) {
        return to_char(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(src), CP_UTF8, code_page);
    }

#pragma endregion

#pragma region UTF stuff

    // YYC MARK:
    // The convertion between UTF is implemented by c16rtomb, c32rtomb, mbrtoc16 and mbrtoc32.
    // These function is locale related in C++ standard, but in Microsoft STL, it's only for UTF8.
    // So we can use them safely in Win32 environment.
    // Reference:
    // * https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/c16rtomb-c32rtomb1?view=msvc-170
    // * https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/mbrtoc16-mbrtoc323?view=msvc-170

#if defined(YYCC_STL_MSSTL)

    // 1 UTF32 unit can produe 4 UTF8 units or 2 UTF16 units in theory.
    // So we pre-allocate memory for the result to prevent allocating memory multiple times.
    constexpr size_t MULTIPLE_UTF8_TO_UTF16 = 1u;
    constexpr size_t MULTIPLE_UTF16_TO_UTF8 = 2u;
    constexpr size_t MULTIPLE_UTF8_TO_UTF32 = 1u;
    constexpr size_t MULTIPLE_UTF32_TO_UTF8 = 4u;

    // UTF8 -> UTF16
    ConvResult<std::u16string> to_utf16(const std::u8string_view& src) {
        std::u16string dst;
        dst.reserve(src.size() * MULTIPLE_UTF8_TO_UTF16);

        std::mbstate_t state{}; // zero-initialized to initial state
        char16_t c16;
        const char* ptr = reinterpret_cast<const char*>(src.data());
        const char* end = ptr + src.size();

        // YYC MARK:
        // Due to the shitty design of mbrtoc16, it forcely assume that passed string is null-terminated.
        // And the third argument should >= 1.
        // However, our given string is string view which do not have null-terminated guaranteen.
        //
        // So we manually check whether we have reach the tail of string and simulate a fake null terminal.
        // If string is still processing, we pass given string.
        // If we have reach the tail of string, we pass our homemade NULL_TERMINAL to this function to make it works normally.
        //
        // This is a stupid polyfill, however, it I do not do this,
        // there is a bug that the second part of surrogate pair will be dropped in final string,
        // if there is a Unicode character located at the tail of string which need surrogate pair to be presented.
        static const char NULL_TERMINAL = '\0';
        while (
            size_t rc = std::mbrtoc16(&c16, (ptr < end ? ptr : &NULL_TERMINAL), (ptr < end ? end - ptr : sizeof(NULL_TERMINAL)), &state)) {
            if (rc == (size_t) -1) return std::unexpected(ConvError::EncodeUtf8);
            else if (rc == (size_t) -2) return std::unexpected(ConvError::IncompleteUtf8);
            else if (rc == (size_t) -3) dst.push_back(c16); // from earlier surrogate pair
            else {
                dst.push_back(c16);
                ptr += rc;
            }
        }
        return dst;
    }

    // UTF16 -> UTF8
    ConvResult<std::u8string> to_utf8(const std::u16string_view& src) {
        std::u8string dst;
        dst.reserve(src.size() * MULTIPLE_UTF16_TO_UTF8);

        std::mbstate_t state{};
        char mbout[MB_LEN_MAX]{};
        size_t rc = 1; // Assign it to ONE to avoid mismatching surrogate pair checker when string is empty.
        for (char16_t c : src) {
            rc = std::c16rtomb(mbout, c, &state);

            if (rc == (size_t) -1) return std::unexpected(ConvError::InvalidUtf16);
            else dst.append(reinterpret_cast<char8_t*>(mbout), rc);
        }

        if (rc == 0) {
            // YYC MARK:
            // If rc is zero after processing all chars,
            // it means that we are aborted when processing an UTF16 surrogate pair.
            // We should report it as an error.
            return std::unexpected(ConvError::InvalidUtf16);
        }

        // Okey, return result.
        return dst;
    }

    // UTF8 -> UTF32
    ConvResult<std::u32string> to_utf32(const std::u8string_view& src) {
        std::u32string dst;
        dst.reserve(src.size() * MULTIPLE_UTF8_TO_UTF32);

        std::mbstate_t state{};
        char32_t c32;
        const char* ptr = reinterpret_cast<const char*>(src.data());
        const char* end = ptr + src.size();

        while (ptr < end) {
            // YYC MARK:
            // There is no surrogate pair in UTF32,
            // so we do not need do that stupid things in UTF8 to UTF32 functions.
            size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state);

            if (rc == (size_t) -1) return std::unexpected(ConvError::EncodeUtf8);
            else if (rc == (size_t) -2) return std::unexpected(ConvError::IncompleteUtf8);
            else if (rc == (size_t) -3) throw std::runtime_error("no surrogates in UTF-32");
            else dst.push_back(c32);

            ptr += rc;
        }
        return dst;
    }

    // UTF32 -> UTF8
    ConvResult<std::u8string> to_utf8(const std::u32string_view& src) {
        std::u8string dst;
        dst.reserve(src.size() * MULTIPLE_UTF32_TO_UTF8);

        std::mbstate_t state{};
        char mbout[MB_LEN_MAX]{};
        for (char32_t c : src) {
            size_t rc = std::c32rtomb(mbout, c, &state);

            if (rc == (size_t) -1) return std::unexpected(ConvError::InvalidUtf32);
            else dst.append(reinterpret_cast<char8_t*>(mbout), rc);
        }

        // YYC MARK:
        // There is no surrogate pair for UTF32,
        // so this "if" statement only presented in UTF16 to UTF8 function.
        // In this function, we directly return value.
        return dst;
    }

#endif

#pragma endregion

} // namespace yycc::encoding::windows

#endif
