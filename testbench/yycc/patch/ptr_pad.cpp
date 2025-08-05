#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/string/op.hpp>
#include <yycc/patch/ptr_pad.hpp>
#include <cinttypes>

#define OP ::yycc::string::op

namespace yycctest::patch::ptr_pad {

    TEST(PatchPtrPad, Normal) {
        auto rv = OP::printf(u8"0x%" PRIXPTR_LPAD PRIXPTR, nullptr);
        EXPECT_TRUE(rv.has_value());

#if defined(YYCC_PTRSIZE_32)
        EXPECT_EQ(rv.value(), u8"0x00000000");
#else
        EXPECT_EQ(rv.value(), u8"0x0000000000000000");
#endif
    }

}
