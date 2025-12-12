#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/env.hpp>
#include <filesystem>

#define ENV ::yycc::env

namespace yycctest::env {

    constexpr char8_t VAR_NAME[] = u8"HOMER";
    constexpr char8_t VAR_VALUE[] = u8"doh";

    TEST(Env, EnvVar) {
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

    TEST(Env, EnvVars) {
        auto rv = ENV::get_vars();
        ASSERT_TRUE(rv.has_value());
        EXPECT_FALSE(rv.value().empty());
    }

    TEST(Env, CurrentDir) {
        auto rv = ENV::current_dir();
        ASSERT_TRUE(rv.has_value());
        EXPECT_TRUE(std::filesystem::is_directory(rv.value()));
    }

    TEST(Env, CurrentExe) {
        auto rv = ENV::current_exe();
        ASSERT_TRUE(rv.has_value());
        EXPECT_TRUE(std::filesystem::is_regular_file(rv.value()));
    }

    TEST(Env, HomeDir) {
        auto rv = ENV::home_dir();
        ASSERT_TRUE(rv.has_value());
        EXPECT_TRUE(std::filesystem::is_directory(rv.value()));
    }

    TEST(Env, TempDir) {
        auto rv = ENV::temp_dir();
        ASSERT_TRUE(rv.has_value());
        EXPECT_TRUE(std::filesystem::is_directory(rv.value()));
    }

    TEST(Env, EnvArgs) {
        auto rv = ENV::get_args();
        ASSERT_TRUE(rv.has_value());
        EXPECT_FALSE(rv.value().empty());
    }

} // namespace yycctest::env
