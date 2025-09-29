#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/windows/com.hpp>

#define COM ::yycc::windows::com

namespace yycctest::windows::com {
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

    TEST(WindowsCom, IsInitialized) {
        // COM environment should always be ready.
        EXPECT_TRUE(COM::is_initialized());
    }

#endif
}
