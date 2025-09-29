#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/rust/env.hpp>

#define ENV ::yycc::rust::env

namespace yycctest::rust::env {

    constexpr char8_t VAR_NAME[] = u8"HOMER";
    constexpr char8_t VAR_VALUE[] = u8"doh";

    TEST(RustEnv, All) {
        // Write a new variable should okey
        {
            auto rv = ENV::set_var(VAR_NAME, VAR_VALUE);
            ASSERT_TRUE(rv.has_value());
        }

        // After writing, we can fetch it and check its value.
        {
            auto rv = ENV::get_var(VAR_NAME);
            ASSERT_TRUE(rv.has_value());
            EXPECT_EQ(rv.value(), VAR_VALUE);
        }

        // The we can delete it.
        {
            auto rv = ENV::del_var(VAR_NAME);
            ASSERT_TRUE(rv.has_value());
        }

        // Delete inexisting variable also should be okey
        {
            auto rv = ENV::del_var(VAR_NAME);
            ASSERT_TRUE(rv.has_value());
        }

        // After deleting, we can not fetch it anymore.
        {
            auto rv = ENV::get_var(VAR_NAME);
            ASSERT_FALSE(rv.has_value());
        }
    }

}
