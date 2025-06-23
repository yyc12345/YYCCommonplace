#include "panic.hpp"
#include "../macro/feature_probe.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#if defined(YYCC_CPPFEAT_STACKTRACE)
#include <stacktrace>
#endif

namespace yycc::rust::panic {

    void panic(const char* file, int line, const std::string_view& msg) {
        // Output message in stderr.
        auto& dst = std::cerr;

        // TODO: Fix colorful output when finishing `termcolor` lib.

        // Print error message if we support it.
        // // Setup color
        // dst << FOREGROUND<Color::Red>;
        // File name and line number message
        dst << "program paniked at " << std::quoted(file) << ":Ln" << line << std::endl;
        // User custom message
        dst << "note: " << msg << std::endl;
        // Stacktrace message if we support it.
#if defined(YYCC_CPPFEAT_STACKTRACE)
        dst << "stacktrace: " << std::endl;
        dst << std::stacktrace::current() << std::endl;
#else
        dst << "there is no stacktrace because your C++ runtime do not support it." << std::endl;
#endif
        // // Restore color
        // dst << RESET;

        // Make sure all messages are flushed into screen.
        dst.flush();

        // Force exit
        std::abort();
    }

} // namespace yycc::rust::panic
