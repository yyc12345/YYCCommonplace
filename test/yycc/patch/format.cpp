#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/patch/format.hpp>

#define FORMAT ::yycc::patch::format

namespace yycctest::patch::format {

    static constexpr char8_t PROBE[] = u8"hello";
    static std::u8string PROBE_STRING(PROBE);
    static constexpr std::u8string_view PROBE_STRING_VIEW(PROBE);

    TEST(PatchFormat, OrdinaryFormat) {
        auto rv = FORMAT::format("{:c}{}{}{}{}{} world!",
                                 PROBE[0],
                                 PROBE_STRING.data(),
                                 PROBE_STRING.c_str(),
                                 PROBE,
                                 PROBE_STRING,
                                 PROBE_STRING_VIEW);
        EXPECT_EQ(rv, "hhellohellohellohellohello world!");
    }

    TEST(PatchFormat, Utf8Format) {
        auto rv = FORMAT::format(u8"{:c}{}{}{}{}{} world!",
                                 PROBE[0],
                                 PROBE_STRING.data(),
                                 PROBE_STRING.c_str(),
                                 PROBE,
                                 PROBE_STRING,
                                 PROBE_STRING_VIEW);
        EXPECT_EQ(rv, u8"hhellohellohellohellohello world!");
    }

}
