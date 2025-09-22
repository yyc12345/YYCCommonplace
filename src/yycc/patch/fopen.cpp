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
        // Convert encoding first, and call MSVCRT specified fopen which support wchar as argument.
        // Reference: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170
        return _wfopen(ENC::to_wchar(u8_filepath).value().c_str(), ENC::to_wchar(u8_mode).value().c_str());
#else
        return std::fopen(REINTERPRET::as_ordinary(u8_filepath), REINTERPRET::as_ordinary(u8_mode));
#endif
    }

} // namespace yycc::patch::fopen
