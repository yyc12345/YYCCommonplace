#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/version_cmp.hpp>

namespace yycctest::macro::version_cmp {

    TEST(MacroVersionCmp, Same) {
        // Test for same version.
        EXPECT_TRUE(YYCC_VERCMP_E(1, 2, 3, 1, 2, 3));
        EXPECT_FALSE(YYCC_VERCMP_NE(1, 2, 3, 1, 2, 3));
        EXPECT_FALSE(YYCC_VERCMP_G(1, 2, 3, 1, 2, 3));
        EXPECT_TRUE(YYCC_VERCMP_GE(1, 2, 3, 1, 2, 3));
        EXPECT_TRUE(YYCC_VERCMP_NL(1, 2, 3, 1, 2, 3));
        EXPECT_FALSE(YYCC_VERCMP_L(1, 2, 3, 1, 2, 3));
        EXPECT_TRUE(YYCC_VERCMP_LE(1, 2, 3, 1, 2, 3));
        EXPECT_TRUE(YYCC_VERCMP_NG(1, 2, 3, 1, 2, 3));
    }

    TEST(MacroVersionCmp, Math) {
        // In version number, 1.2.10 is greater than 1.2.9
        EXPECT_FALSE(YYCC_VERCMP_E(1, 2, 10, 1, 2, 9));
        EXPECT_TRUE(YYCC_VERCMP_NE(1, 2, 10, 1, 2, 9));
        EXPECT_TRUE(YYCC_VERCMP_G(1, 2, 10, 1, 2, 9));
        EXPECT_TRUE(YYCC_VERCMP_GE(1, 2, 10, 1, 2, 9));
        EXPECT_TRUE(YYCC_VERCMP_NL(1, 2, 10, 1, 2, 9));
        EXPECT_FALSE(YYCC_VERCMP_L(1, 2, 10, 1, 2, 9));
        EXPECT_FALSE(YYCC_VERCMP_LE(1, 2, 10, 1, 2, 9));
        EXPECT_FALSE(YYCC_VERCMP_NG(1, 2, 10, 1, 2, 9));
    }

}
