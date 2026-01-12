#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/windows/dialog.hpp>

#define DIALOG ::yycc::windows::dialog

namespace yycctest::windows::dialog {
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

    TEST(WindowsDialog, Normal) {
        // TODO:
        // I temporaryly disable all dialog open functions in this function after testing them.
        // Because they need human to operate them to finish the test.
        // Once I find a better way to do automatic test (maybe send message to these dialogs to close them?)
        // I will add them back.

        // Prepare parameters
        DIALOG::FileDialog params;
        auto& filters = params.configure_file_types();
        filters.add_filter(u8"Microsoft Word (*.docx; *.doc)", {u8"*.docx", u8"*.doc"});
        filters.add_filter(u8"Microsoft Excel (*.xlsx; *.xls)", {u8"*.xlsx", u8"*.xls"});
        filters.add_filter(u8"Microsoft PowerPoint (*.pptx; *.ppt)", {u8"*.pptx", u8"*.ppt"});
        filters.add_filter(u8"Text File (*.txt)", {u8"*.txt"});
        filters.add_filter(u8"All Files (*.*)", {u8"*.*"});
        params.set_default_file_type_index(1u);

        //// Open file
        //{
        //    auto rv = DIALOG::open_file(params);
        //    EXPECT_TRUE(rv.has_value());
        //}
        //// Open files
        //{
        //    auto rv = DIALOG::open_files(params);
        //    EXPECT_TRUE(rv.has_value());
        //}
        //// Save file
        //{
        //    auto rv = DIALOG::save_file(params);
        //    EXPECT_TRUE(rv.has_value());
        //}

        // Clear file filters for following operations
        params.clear();
        params.set_default_file_type_index(0u);

        //// Open folder
        //{
        //    auto rv = DIALOG::open_folder(params);
        //    EXPECT_TRUE(rv.has_value());
        //}
    }

#endif
}
