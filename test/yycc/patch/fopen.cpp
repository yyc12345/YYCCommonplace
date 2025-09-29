#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/patch/fopen.hpp>

#define FOPEN ::yycc::patch::fopen

namespace yycctest::patch::fopen {

    TEST(PatchFopen, Normal) {
        FILE* handle;

#if defined(YYCC_OS_WINDOWS)
        // In Windows, we can always visit NUL device.
        handle = FOPEN::fopen(u8"NUL", u8"wb");
#else
        // In other system following UNIX design, we can visit /dev/null device.
        handle = FOPEN::fopen(u8"/dev/null", u8"wb");
#endif

        ASSERT_TRUE(handle != nullptr);
        std::fclose(handle);
    }

} // namespace yycctest::patch::fopen
