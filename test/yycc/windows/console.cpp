#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/windows/console.hpp>

#define CONSOLE ::yycc::windows::console

namespace yycctest::windows::console {
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

    TEST(WindowsConsole, ColorfulConsole) {
        auto rv = CONSOLE::colorful_console();
        // Set colorful console should always be success.
        // Unless it is not a TTY.
        EXPECT_TRUE(rv.has_value() || rv.error() == CONSOLE::ExecError::NotTty);
    }

#endif
}
