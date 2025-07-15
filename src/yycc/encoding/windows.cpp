#include "windows.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS

#include "../string/reinterpret.hpp"
#include <limits>
#include <stdexcept>

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
        int desired_size
            = WideCharToMultiByte(code_page, 0, lpWideCharStr, cchWideChar, NULL, 0, NULL, NULL);
        if (desired_size <= 0) return ConvError::NoDesiredSize;
        // resize dest for receiving result
        dst.resize(static_cast<size_t>(desired_size));
        // do real convertion
        int write_result = WideCharToMultiByte(code_page,
                                               0,
                                               lpWideCharStr,
                                               cchWideChar,
                                               reinterpret_cast<LPSTR>(dst.data()),
                                               desired_size,
                                               NULL,
                                               NULL);
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
        int write_result = MultiByteToWideChar(code_page,
                                               0,
                                               lpMultiByteStr,
                                               cbMultiByte,
                                               reinterpret_cast<LPWSTR>(dst.data()),
                                               desired_size);
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

    bool to_char(const std::string_view& src,
                 std::string& dst,
                 CodePage src_code_page,
                 CodePage dst_code_page) {
        CONVFN_TYPE1(to_char, char, char, src_code_page, dst_code_page);
    }

    std::string to_char(const std::string_view& src,
                        CodePage src_code_page,
                        CodePage dst_code_page) {
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

    ConvResult<std::string> priv_to_char(const NS_YYCC_STRING::u8string_view& src,
                                         CodePage code_page) {
        return priv_to_char(NS_YYCC_STRING_REINTERPRET::as_ordinary_view(src), CP_UTF8, code_page);
    }

    bool to_char(const NS_YYCC_STRING::u8string_view& src, std::string& dst, CodePage code_page) {
        CONVFN_TYPE1(to_char, NS_YYCC_STRING::u8char, char, code_page);
    }

    std::string to_char(const NS_YYCC_STRING::u8string_view& src, CodePage code_page) {
        CONVFN_TYPE2(to_char, NS_YYCC_STRING::u8char, char, code_page);
    }

#pragma endregion

#undef CONVFN_TYPE1
#undef CONVFN_TYPE2
#undef CONVFCT_TYPE4

} // namespace yycc::encoding::windows

#endif
