#include "panic.hpp"
#include "../carton/termcolor.hpp"
#include "../string/reinterpret.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stacktrace>

#define TERMCOLOR ::yycc::carton::termcolor
#define REINTERPRET ::yycc::string::reinterpret

namespace yycc::rust::panic {

    void panic(const char* file, int line, const std::string_view& msg) {
        // Output message in stderr.
        auto& dst = std::cerr;

        // Print error message if we support it.
        // Setup color
        dst << REINTERPRET::as_ordinary_view(TERMCOLOR::foreground(TERMCOLOR::Color::Red));
        // File name and line number message
        dst << "program paniked at " << std::quoted(file) << ":Ln" << line << std::endl;
        // User custom message
        dst << "note: " << msg << std::endl;
        // Stacktrace message if we support it.
        dst << "stacktrace: " << std::endl;
        dst << std::stacktrace::current() << std::endl;
        // Restore color
        dst << REINTERPRET::as_ordinary_view(TERMCOLOR::reset());;

        // Make sure all messages are flushed into screen.
        dst.flush();

        // Force exit
        std::abort();
    }

} // namespace yycc::rust::panic
