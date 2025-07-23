#include "windows.hpp"

#if defined(YYCC_OS_WINDOWS)

#include "../string/reinterpret.hpp"
#include <limits>
#include <stdexcept>
#include <cuchar>

#include "../windows/import_guard_head.hpp"
#include <Windows.h>
#include "../windows/import_guard_tail.hpp"

#define NS_YYCC_STRING ::yycc::string
#define NS_YYCC_STRING_REINTERPRET ::yycc::string::reinterpret
#define NS_YYCC_PATCH_EXPECTED ::yycc::patch::expected

namespace yycc::encoding::windows {

#pragma region Help Macros

#define CONVFN_TYPE1(fct_name, src_char_type, dst_char_type, ...) \
    namespace expected = NS_YYCC_PATCH_EXPECTED; \
    auto rv = priv_##fct_name(src, ##__VA_ARGS__); \
    if (expected::is_value(rv)) { \
        dst = std::move(expected::get_value(rv)); \
        return true; \
    } else { \
        return false; \
    }

#define CONVFN_TYPE2(fct_name, src_char_type, dst_char_type, ...) \
    std::basic_string<dst_char_type> rv; \
    if (fct_name(src, rv, ##__VA_ARGS__)) return rv; \
    else throw std::runtime_error("fail to convert string in Win32 function");

#pragma endregion

#pragma region WChar -> Char

    ConvResult<std::string> priv_to_char(const std::wstring_view& src, CodePage code_page) {
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
        if (cSrcSize > std::numeric_limits<int>::max()) return ConvError::TooLargeLength;
        int cchWideChar = static_cast<int>(src.size());

        // do convertion
        // do a dry-run first to fetch desired size.
        int desired_size = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
        if (desired_size <= 0) return ConvError::NoDesiredSize;
        // resize dest for receiving result
        dst.resize(static_cast<size_t>(desired_size));
        // do real convertion
        int write_result
            = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, reinterpret_cast<LPSTR>(dst.data()), desired_size, NULL, NULL);
        if (write_result <= 0) return ConvError::BadWrittenSize;

        return dst;
    }

    bool to_char(const std::wstring_view& src, std::string& dst, CodePage code_page) {
        CONVFN_TYPE1(to_char, wchar_t, char, code_page);
    }

    std::string to_char(const std::wstring_view& src, CodePage code_page) {
        CONVFN_TYPE2(to_char, wchar_t, char, code_page);
    }

#pragma endregion

#pragma region Char -> WChar

    ConvResult<std::wstring> priv_to_wchar(const std::string_view& src, CodePage code_page) {
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
        if (cSrcSize > std::numeric_limits<int>::max()) return ConvError::TooLargeLength;
        int cbMultiByte = static_cast<int>(src.size());

        // do convertion
        // do a dry-run first to fetch desired size.
        int desired_size = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, NULL, 0);
        if (desired_size <= 0) return ConvError::NoDesiredSize;
        // resize dest for receiving result
        dst.resize(static_cast<size_t>(desired_size));
        // do real convertion
        int write_result = MultiByteToWideChar(code_page, 0, lpMultiByteStr, cbMultiByte, reinterpret_cast<LPWSTR>(dst.data()), desired_size);
        if (write_result <= 0) return ConvError::BadWrittenSize;

        return dst;
    }

    bool to_wchar(const std::string_view& src, std::wstring& dst, CodePage code_page) {
        CONVFN_TYPE1(to_wchar, char, wchar_t, code_page);
    }

    std::wstring to_wchar(const std::string_view& src, CodePage code_page) {
        CONVFN_TYPE2(to_wchar, char, wchar_t, code_page);
    }

#pragma endregion

#pragma region Char -> Char

    ConvResult<std::string> priv_to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page) {
        namespace expected = NS_YYCC_PATCH_EXPECTED;

        // Perform first convertion
        auto first_rv = priv_to_wchar(src, src_code_page);
        if (expected::is_error(first_rv)) {
            return expected::get_error(first_rv);
        }
        // Perform second convertion
        auto second_rv = to_char(std::get<std::wstring>(first_rv), dst_code_page);
        return second_rv;
    }

    bool to_char(const std::string_view& src, std::string& dst, CodePage src_code_page, CodePage dst_code_page) {
        CONVFN_TYPE1(to_char, char, char, src_code_page, dst_code_page);
    }

    std::string to_char(const std::string_view& src, CodePage src_code_page, CodePage dst_code_page) {
        CONVFN_TYPE2(to_char, char, char, src_code_page, dst_code_page);
    }

#pragma endregion

#pragma region WChar -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::wstring_view& src) {
        namespace expected = NS_YYCC_PATCH_EXPECTED;

        auto rv = priv_to_char(src, CP_UTF8);
        if (expected::is_value(rv)) {
            return NS_YYCC_STRING_REINTERPRET::as_utf8(expected::get_value(rv));
        } else {
            return expected::get_error(rv);
        }
    }

    bool to_utf8(const std::wstring_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, wchar_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string to_utf8(const std::wstring_view& src) {
        CONVFN_TYPE2(to_utf8, wchar_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region UTF8 -> WChar

    ConvResult<std::wstring> priv_to_wchar(const NS_YYCC_STRING::u8string_view& src) {
        return priv_to_wchar(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(src), CP_UTF8);
    }

    bool to_wchar(const NS_YYCC_STRING::u8string_view& src, std::wstring& dst) {
        CONVFN_TYPE1(to_wchar, NS_YYCC_STRING::u8char, wchar_t);
    }

    std::wstring to_wchar(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_wchar, NS_YYCC_STRING::u8char, wchar_t);
    }

#pragma endregion

#pragma region Char -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::string_view& src, CodePage code_page) {
        namespace expected = NS_YYCC_PATCH_EXPECTED;

        auto rv = priv_to_char(src, code_page, CP_UTF8);
        if (expected::is_value(rv)) {
            return NS_YYCC_STRING_REINTERPRET::as_utf8(expected::get_value(rv));
        } else {
            return expected::get_error(rv);
        }
    }

    bool to_utf8(const std::string_view& src, NS_YYCC_STRING::u8string& dst, CodePage code_page) {
        CONVFN_TYPE1(to_utf8, char, NS_YYCC_STRING::u8char, code_page);
    }

    NS_YYCC_STRING::u8string to_utf8(const std::string_view& src, CodePage code_page) {
        CONVFN_TYPE2(to_utf8, char, NS_YYCC_STRING::u8char, code_page);
    }

#pragma endregion

#pragma region UTF8 -> Char

    ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page) {
        return priv_to_char(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(src), CP_UTF8, code_page);
    }

    bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst, CodePage code_page) {
        CONVFN_TYPE1(to_char, NS_YYCC_STRING::u8char, char, code_page);
    }

    std::string to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page) {
        CONVFN_TYPE2(to_char, NS_YYCC_STRING::u8char, char, code_page);
    }

#pragma endregion

    // YYC MARK:
    // The convertion between UTF is implemented by c16rtomb, c32rtomb, mbrtoc16 and mbrtoc32.
    // These function is locale related in C++ standard, but in Microsoft STL, it's only for UTF8.
    // So we can use them safely in Win32 environment.

    // 1 UTF32 unit can produe 4 UTF8 units or 2 UTF16 units in theory.
    // So we pre-allocate memory for the result to prevent allocating memory multiple times.
    constexpr size_t MULTIPLE_UTF8_TO_UTF16 = 1u;
    constexpr size_t MULTIPLE_UTF16_TO_UTF8 = 2u;
    constexpr size_t MULTIPLE_UTF8_TO_UTF32 = 1u;
    constexpr size_t MULTIPLE_UTF32_TO_UTF8 = 4u;

#pragma region UTF8 -> UTF16

    ConvResult<std::u16string> priv_to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        std::u16string dst;
        dst.reserve(src.size() * MULTIPLE_UTF8_TO_UTF16);

        std::mbstate_t state{}; // zero-initialized to initial state
        char16_t c16;
        const char* ptr = reinterpret_cast<const char*>(src.data());
        const char* end = ptr + src.size();

        while (ptr < end) {
            size_t rc = std::mbrtoc16(&c16, ptr, end - ptr, &state);

            if (rc == (size_t) -1) return ConvError::EncodeUtf8;
            else if (rc == (size_t) -2) return ConvError::IncompleteUtf8;
            else if (rc != (size_t) -3) dst.push_back(c16); // from earlier surrogate pair
            else {
                dst.push_back(c16);
                ptr += rc;
            }
        }
        return dst;
    }

    bool to_utf16(const NS_YYCC_STRING::u8string_view& src, std::u16string& dst) {
        CONVFN_TYPE1(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }

    std::u16string to_utf16(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf16, NS_YYCC_STRING::u8char, char16_t);
    }

#pragma endregion

#pragma region UTF16 -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u16string_view& src) {
        NS_YYCC_STRING::u8string dst;
        dst.reserve(src.size() * MULTIPLE_UTF16_TO_UTF8);

        std::mbstate_t state{};
        char mbout[MB_LEN_MAX]{};
        for (char16_t c : src) {
            std::size_t rc = std::c16rtomb(mbout, c, &state);
            if (rc != (std::size_t) -1) dst.append(reinterpret_cast<NS_YYCC_STRING::u8char*>(mbout), rc);
            else return ConvError::InvalidUtf16;
        }
        return dst;
    }

    bool to_utf8(const std::u16string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string to_utf8(const std::u16string_view& src) {
        CONVFN_TYPE2(to_utf8, char16_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#pragma region UTF8 -> UTF32

    ConvResult<std::u32string> priv_to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        std::u32string dst;
        dst.reserve(src.size() * MULTIPLE_UTF8_TO_UTF32);

        std::mbstate_t state{};
        char32_t c32;
        const char* ptr = reinterpret_cast<const char*>(src.data());
        const char* end = ptr + src.size();

        while (ptr < end) {
            size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state);

            if (rc == (size_t) -1) return ConvError::EncodeUtf8;
            else if (rc == (size_t) -2) return ConvError::IncompleteUtf8;
            else if (rc != (size_t) -3) throw std::runtime_error("no surrogates in UTF-32");
            else dst.push_back(c32);

            ptr += rc;
        }
        return dst;
    }

    bool to_utf32(const NS_YYCC_STRING::u8string_view& src, std::u32string& dst) {
        CONVFN_TYPE1(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }

    std::u32string to_utf32(const NS_YYCC_STRING::u8string_view& src) {
        CONVFN_TYPE2(to_utf32, NS_YYCC_STRING::u8char, char32_t);
    }

#pragma endregion

#pragma region UTF32 -> UTF8

    ConvResult<NS_YYCC_STRING::u8string> priv_to_utf8(const std::u32string_view& src) {
        NS_YYCC_STRING::u8string dst;
        dst.reserve(src.size() * MULTIPLE_UTF32_TO_UTF8);

        std::mbstate_t state{};
        char mbout[MB_LEN_MAX]{};
        for (char32_t c : src) {
            std::size_t rc = std::c32rtomb(mbout, c, &state);
            if (rc != (std::size_t) -1) dst.append(reinterpret_cast<NS_YYCC_STRING::u8char*>(mbout), rc);
            else return ConvError::InvalidUtf32;
        }
        return dst;
    }

    bool to_utf8(const std::u32string_view& src, NS_YYCC_STRING::u8string& dst) {
        CONVFN_TYPE1(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }

    NS_YYCC_STRING::u8string to_utf8(const std::u32string_view& src) {
        CONVFN_TYPE2(to_utf8, char32_t, NS_YYCC_STRING::u8char);
    }

#pragma endregion

#undef CONVFN_TYPE1
#undef CONVFN_TYPE2

} // namespace yycc::encoding::windows

#endif
