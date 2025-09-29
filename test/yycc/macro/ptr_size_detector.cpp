#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/ptr_size_detector.hpp>

#define PTR_SIZE ::yycc::macro::ptr_size

namespace yycctest::macro::ptr_size {

    TEST(MacroPtrSize, Main) {
        auto rv = PTR_SIZE::get_ptr_size();
#if defined(YYCC_PTRSIZE_32)
        EXPECT_EQ(rv, PTR_SIZE::PtrSizeKind::Bits32);
#else
        EXPECT_EQ(rv, PTR_SIZE::PtrSizeKind::Bits64);
#endif
    }

} // namespace yycctest::macro::ptr_size
