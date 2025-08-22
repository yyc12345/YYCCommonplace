#include "console.hpp"
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include <cstdio>

#include "import_guard_head.hpp"
#include <Windows.h>
#include <io.h>
#include "import_guard_tail.hpp"

namespace yycc::windows::console {

    static ExecResult<void> colorful_fs(FILE* fs) {
        if (!_isatty(_fileno(fs))) {
            return std::unexpected(ExecError::NotTty);
        }

        HANDLE h_output;
        DWORD dw_mode;

        h_output = (HANDLE) _get_osfhandle(_fileno(fs));
        if (!GetConsoleMode(h_output, &dw_mode)) {
            return std::unexpected(ExecError::GetMode);
        }
        if (!SetConsoleMode(h_output, dw_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT)) {
            return std::unexpected(ExecError::SetMode);
        }

        return {};
    }

    ExecResult<void> colorful_console() {
        return colorful_fs(stdout).and_then([]() { return colorful_fs(stderr); });
    }

} // namespace yycc::windows::console

#endif
