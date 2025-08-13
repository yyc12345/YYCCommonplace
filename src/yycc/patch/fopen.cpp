#include "fopen.hpp"
#include "../macro/os_detector.hpp"

#if defined(YYCC_OS_WINDOWS)
// Include header file including MSVCRT specific function.
#include <wchar.h>
// Include encoding convertion header
#include "../encoding/windows.hpp"
// Define namespace macro
#define ENC ::yycc::encoding::windows
#else
// Include reinterpret header and define namespace macro
#include "../string/reinterpret.hpp"
#define REINTERPRET ::yycc::string::reinterpret
#endif

namespace yycc::patch::fopen {

    std::FILE* fopen(const char8_t* u8_filepath, const char8_t* u8_mode) {
#if defined(YYCC_OS_WINDOWS)
        // convert mode and file path to wchar
        auto wmode = ENC::to_wchar(u8_mode);
        auto wpath = ENC::to_wchar(u8_filepath);

        // check convertion success
        if (wmode.has_value() && wpath.has_value()) {
            // call microsoft specified fopen which support wchar as argument.
            return _wfopen(wpath.value().c_str(), wmode.value().c_str());
        } else {
            // fail to convert encoding
            return nullptr;
        }
#else
        return std::fopen(REINTERPRET::as_ordinary(u8_filepath), REINTERPRET::as_ordinary(u8_mode));
#endif
    }

} // namespace yycc::patch::fopen
