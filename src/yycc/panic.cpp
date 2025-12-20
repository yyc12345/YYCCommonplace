#include "panic.hpp"
#include "carton/termcolor.hpp"
#include "patch/stream.hpp"
#include "patch/libcxx/stacktrace.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stacktrace>

#define TERMCOLOR ::yycc::carton::termcolor

using namespace yycc::patch::stream;

namespace yycc::panic {

    void panic(const char* file, int line, const std::u8string_view& msg) {
        // Output message in stderr.
        auto& dst = std::cerr;

        // Print error message if we support it.
        // Setup color
        dst << TERMCOLOR::foreground(TERMCOLOR::Color::Red);
        // File name and line number message
        dst << "program paniked at " << std::quoted(file) << ":Ln" << line << std::endl;
        // User custom message
        dst << "note: " << msg << std::endl;
        // Stacktrace message if we support it.
        dst << "stacktrace: " << std::endl;
        dst << std::stacktrace::current() << std::endl;
        // Restore color
        dst << TERMCOLOR::reset();

        // Make sure all messages are flushed into screen.
        dst.flush();

        // Force exit
        std::abort();
    }

} // namespace yycc::panic
