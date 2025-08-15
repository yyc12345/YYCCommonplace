#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/endian_detector.hpp>

#define ENDIAN ::yycc::macro::endian

namespace yycctest::macro::endian {

    TEST(MacroEndian, Main) {
        auto rv = ENDIAN::get_endian();
#if defined(YYCC_ENDIAN_LITTLE)
        EXPECT_EQ(rv, ENDIAN::EndianKind::Little);
#else
        EXPECT_EQ(rv, ENDIAN::EndianKind::Big);
#endif
    }

} // namespace yycctest::macro::endian
