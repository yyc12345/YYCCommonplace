#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/os_detector.hpp>

#define OS ::yycc::macro::os

namespace yycctest::macro::os {

    TEST(MacroOs, Main) {
        auto rv = OS::get_os();
#if defined(YYCC_OS_WINDOWS)
        EXPECT_EQ(rv, OS::OsKind::Windows);
#elif defined(YYCC_OS_LINUX)
        EXPECT_EQ(rv, OS::OsKind::Linux);
#else
        EXPECT_EQ(rv, OS::OsKind::MacOs);
#endif
    }

} // namespace yycctest::macro::os
