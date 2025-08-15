#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/stl_detector.hpp>

#define STL ::yycc::macro::stl

namespace yycctest::macro::stl {

    TEST(MacroStl, Main) {
        auto rv = STL::get_stl();
#if defined(YYCC_STL_MSSTL)
        EXPECT_EQ(rv, STL::StlKind::MsStl);
#elif defined(YYCC_STL_GNUSTL)
        EXPECT_EQ(rv, STL::StlKind::GnuStl);
#else
        EXPECT_EQ(rv, STL::StlKind::ClangStl);
#endif
    }

} // namespace yycctest::macro::stl
