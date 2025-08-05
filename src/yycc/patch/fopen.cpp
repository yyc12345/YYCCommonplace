#include "fopen.hpp"
#include "../macro/os_detector.hpp"
#include "../string/reinterpret.hpp"

#if defined(YYCC_OS_WINDOWS)
#include "../windows/import_guard_head.hpp"
#include <Windows.h>
#include "../windows/import_guard_tail.hpp"
#endif

#define REINTERPRET ::yycc::string::reinterpret

namespace yycc::patch::fopen {

    std::FILE* fopen(const char8_t* u8_filepath, const char8_t* u8_mode) {
        // TODO: Fix this after finish Windows encoding
// #if defined(YYCC_OS_WINDOWS)

//         // convert mode and file path to wchar
//         std::wstring wmode, wpath;
//         if (!YYCC::EncodingHelper::UTF8ToWchar(u8_mode, wmode))
//             return nullptr;
//         if (!YYCC::EncodingHelper::UTF8ToWchar(u8_filepath, wpath))
//             return nullptr;

//         // call microsoft specified fopen which support wchar as argument.
//         return _wfopen(wpath.c_str(), wmode.c_str());

// #else
//         return std::fopen(REINTERPRET::as_ordinary(u8_filepath), REINTERPRET::as_ordinary(u8_mode));
// #endif
        return std::fopen(REINTERPRET::as_ordinary(u8_filepath), REINTERPRET::as_ordinary(u8_mode));
    }

}
