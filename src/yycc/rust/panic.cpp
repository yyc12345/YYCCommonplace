#include "panic.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stacktrace>

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
        dst << "stacktrace: " << std::endl;
        dst << std::stacktrace::current() << std::endl;
        // // Restore color
        // dst << RESET;

        // Make sure all messages are flushed into screen.
        dst.flush();

        // Force exit
        std::abort();
    }

} // namespace yycc::rust::panic
