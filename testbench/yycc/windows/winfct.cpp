#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/windows/winfct.hpp>

#define WINFCT ::yycc::windows::winfct

namespace yycctest::windows::winfct {
#if defined(YYCC_OS_WINDOWS)

    TEST(WindowsWinFct, GetCurrentModule) {
        auto rv = WINFCT::get_current_module();
        EXPECT_TRUE(rv.has_value());
    }

    TEST(WindowsWinFct, GetTempDirectory) {
        auto rv = WINFCT::get_temp_directory();
        EXPECT_TRUE(rv.has_value());
    }

    TEST(WindowsWinFct, GetModuleFileName) {
        auto handle = WINFCT::get_current_module();
        ASSERT_TRUE(handle.has_value());

        auto rv = WINFCT::get_module_file_name(handle.value());
        EXPECT_TRUE(rv.has_value());
    }

    TEST(WindowsWinFct, IsValidCodePage) {
        EXPECT_TRUE(WINFCT::is_valid_code_page(437));
        EXPECT_TRUE(WINFCT::is_valid_code_page(65001));

        EXPECT_FALSE(WINFCT::is_valid_code_page(6161));
    }

#if defined(YYCC_STL_MSSTL)

    TEST(WindowsWinFct, GetKnownPath) {
        auto rv = WINFCT::get_known_path(WINFCT::KnownDirectory::LocalAppData);
        EXPECT_TRUE(rv.has_value());
    }

#endif

    // YYC MARK:
    // I can't test CopyFile, MoveFile and DeleteFile.

#endif
} // namespace yycctest::windows::winfct
