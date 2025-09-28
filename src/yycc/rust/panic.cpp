#include "panic.hpp"
#include "../carton/termcolor.hpp"
#include "../patch/stream.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>

#define TERMCOLOR ::yycc::carton::termcolor

using namespace yycc::patch::stream;

namespace yycc::rust::panic {

    // TODO:
    // I sadly remove the stacktrace feature for panic function.
    // Because in GCC, it has link error (can be fixed by extra link option).
    // In Clang, it even lack the whole header file.
    // It seems that STL providers are not ready for this feature. So I decide remove it entirely.
    // Once every STL probiders have ready for this, I will add it back.

    void panic(const char* file, int line, const std::string_view& msg) {
        // Output message in stderr.
        auto& dst = std::cerr;

        // Print error message if we support it.
        // Setup color
        dst << TERMCOLOR::foreground(TERMCOLOR::Color::Red);
        // File name and line number message
        dst << "program paniked at " << std::quoted(file) << ":Ln" << line << std::endl;
        // User custom message
        dst << "note: " << msg << std::endl;
        // Restore color
        dst << TERMCOLOR::reset();

        // Make sure all messages are flushed into screen.
        dst.flush();

        // Force exit
        std::abort();
    }

} // namespace yycc::rust::panic
