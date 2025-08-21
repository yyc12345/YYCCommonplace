#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/tabulate.hpp>
#include <yycc/string/reinterpret.hpp>
#include <sstream>

#define TABULATE ::yycc::carton::tabulate
#define REINTERPRET ::yycc::string::reinterpret

namespace yycctest::carton::tabulate {

    class CartonTabulate : public ::testing::Test {
    protected:
        CartonTabulate() : table(3u), ss() {
            // setup basic data
            table.set_prefix(u8"# ");
            table.set_header({u8"中文1", u8"中文2", u8"中文3"});
            table.set_bar(u8"===");
            table.add_row({u8"a", u8"b", u8"c"});
        }
        ~CartonTabulate() override = default;

        void expected_print(const std::u8string_view& exp) {
            ss.str("");
            table.print(ss);
            EXPECT_EQ(REINTERPRET::as_utf8_view(ss.view()), exp);
        }

        TABULATE::Tabulate table;
        std::stringstream ss;
    };

    TEST_F(CartonTabulate, Full) {
        table.show_header(true);
        table.show_bar(true);
        expected_print(u8"# 中文1 中文2 中文3 \n"
                       u8"# ===   ===   ===   \n"
                       u8"# a     b     c     \n");
    }

    TEST_F(CartonTabulate, NoHeader) {
        table.show_header(false);
        table.show_bar(true);
        expected_print(u8"# === === === \n"
                       u8"# a   b   c   \n");
    }

    TEST_F(CartonTabulate, NoBar) {
        table.show_header(true);
        table.show_bar(false);
        expected_print(u8"# 中文1 中文2 中文3 \n"
                       u8"# a     b     c     \n");
    }

    TEST_F(CartonTabulate, OnlyData) {
        table.show_header(false);
        table.show_bar(false);
        expected_print(u8"# a b c \n");
    }

} // namespace yycctest::carton::tabulate
