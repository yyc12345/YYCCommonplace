#include "path.hpp"
#include "../macro/os_detector.hpp"
#include <stdexcept>

#define NS_YYCC_STRING ::yycc::string

namespace yycc::patch::path {

    // TODO: Fix this after finish encoding parts.

    // TODO: Add native implementation if C++ support it.
    // So we need add feature test macro at the same time.

    std::filesystem::path to_std_path(const NS_YYCC_STRING::u8string_view& u8_path) {
// #if YYCC_OS == YYCC_OS_WINDOWS

//         // convert path to wchar
//         std::wstring wpath;
//         if (!YYCC::EncodingHelper::UTF8ToWchar(u8_path, wpath))
//             throw std::invalid_argument("Fail to convert given UTF8 string.");
//         // return path with wchar_t ctor
//         return std::filesystem::path(wpath);

// #else
//         std::string cache = YYCC::EncodingHelper::ToOrdinary(u8_path);
//         return std::filesystem::path(cache.c_str());
// #endif
        return std::filesystem::path();
    }

    NS_YYCC_STRING::u8string to_u8string(const std::filesystem::path& path) {
// #if YYCC_OS == YYCC_OS_WINDOWS

//         // get and convert to utf8
//         NS_YYCC_STRING::u8string u8_path;
//         if (!YYCC::EncodingHelper::WcharToUTF8(path.c_str(), u8_path))
//             throw std::invalid_argument("Fail to convert to UTF8 string.");

//         // return utf8 path
//         return u8_path;

// #else
//         return EncodingHelper::ToUTF8(path.string());
// #endif
        return NS_YYCC_STRING::u8string();
    }

}
