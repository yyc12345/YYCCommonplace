#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/constraint.hpp>

#include <yycc/prelude.hpp>

#define CONSTRAINT ::yycc::constraint::Constraint

namespace yycctest::constraint {

    template<typename T>
    bool check(const T& value) {
        return false;
    }

    template<typename T>
    T clamp(const T& value) {
        return value;
    }

    TEST(Constraint, Normal) {
        CONSTRAINT<u32> instance(check<u32>, clamp<u32>);
        EXPECT_TRUE(instance.support_check());
        EXPECT_TRUE(instance.support_clamp());
        EXPECT_FALSE(instance.check(0));
        EXPECT_EQ(instance.clamp(0), 0);
    }

    TEST(Constraint, SomeNone) {
        {
            CONSTRAINT<u32> instance(check<u32>, nullptr);
            EXPECT_TRUE(instance.support_check());
            EXPECT_FALSE(instance.support_clamp());
            EXPECT_FALSE(instance.check(0));
        }
        {
            CONSTRAINT<u32> instance(nullptr, clamp<u32>);
            EXPECT_FALSE(instance.support_check());
            EXPECT_TRUE(instance.support_clamp());
            EXPECT_EQ(instance.clamp(0), 0);
        }
    }

    TEST(Constraint, AllNone) {
        CONSTRAINT<u32> instance(nullptr, nullptr);
        EXPECT_FALSE(instance.support_check());
        EXPECT_FALSE(instance.support_clamp());
    }

}
