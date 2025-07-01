#include <gtest/gtest.h>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <yycc.hpp>
#include <yycc/patch/contains.hpp>

#include <yycc/prelude/rust.hpp>

#define FN_CONTAINS ::yycc::patch::contains::contains

namespace yycctest::patch::contains {

    TEST(PatchContains, Contains) {
        // Set
        {
            std::set<u32> collection{
                UINT32_C(1),
                UINT32_C(3),
                UINT32_C(5),
            };
            EXPECT_TRUE(FN_CONTAINS(collection, 1));
            EXPECT_FALSE(FN_CONTAINS(collection, 2));
        }
        // Unordered set
        {
            std::unordered_set<u32> collection{
                UINT32_C(1),
                UINT32_C(3),
                UINT32_C(5),
            };
            EXPECT_TRUE(FN_CONTAINS(collection, 1));
            EXPECT_FALSE(FN_CONTAINS(collection, 2));
        }
        // Map
        {
            std::map<u32, u32> collection{
                {UINT32_C(1), UINT32_C(2)},
                {UINT32_C(3), UINT32_C(4)},
                {UINT32_C(5), UINT32_C(6)},
            };
            EXPECT_TRUE(FN_CONTAINS(collection, 1));
            EXPECT_FALSE(FN_CONTAINS(collection, 2));
        }
        // Unordered Map
        {
            std::unordered_map<u32, u32> collection{
                {UINT32_C(1), UINT32_C(2)},
                {UINT32_C(3), UINT32_C(4)},
                {UINT32_C(5), UINT32_C(6)},
            };
            EXPECT_TRUE(FN_CONTAINS(collection, 1));
            EXPECT_FALSE(FN_CONTAINS(collection, 2));
        }
    }

} // namespace yycctest::patch::contains
