#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/termcolor.hpp>
#include <yycc/cenum.hpp>

#define TERMCOLOR ::yycc::carton::termcolor
#define CENUM ::yycc::cenum

using namespace std::literals::string_view_literals;
using Color = TERMCOLOR::Color;
using Attribute = TERMCOLOR::Attribute;

namespace yycctest::carton::termcolor {

    TEST(CartonTermcolor, Lowlevel) {
        EXPECT_EQ(TERMCOLOR::foreground(Color::Default), u8"");
        EXPECT_EQ(TERMCOLOR::foreground(Color::Red), u8"\033[31m");
        EXPECT_EQ(TERMCOLOR::foreground(Color::LightRed), u8"\033[91m");

        EXPECT_EQ(TERMCOLOR::background(Color::Default), u8"");
        EXPECT_EQ(TERMCOLOR::background(Color::Red), u8"\033[41m");
        EXPECT_EQ(TERMCOLOR::background(Color::LightRed), u8"\033[101m");

        EXPECT_EQ(TERMCOLOR::style(Attribute::Default), u8"");
        EXPECT_EQ(TERMCOLOR::style(Attribute::Italic), u8"\033[3m");
        EXPECT_EQ(TERMCOLOR::styles(CENUM::merge(Attribute::Italic, Attribute::Bold)),
                  u8"\033[1m"
                  "\033[3m");

        EXPECT_EQ(TERMCOLOR::reset(), u8"\033[0m"sv);
    }

    TEST(CartonTermcolor, Highlevel) {
        EXPECT_EQ(TERMCOLOR::colored(u8"hello"sv), u8"hello\033[0m");
        EXPECT_EQ(TERMCOLOR::colored(u8"hello"sv, Color::LightWhite, Color::Red, CENUM::merge(Attribute::Italic, Attribute::Bold)),
                  u8"\033[97m"
                  "\033[41m"
                  "\033[1m"
                  "\033[3m"
                  "hello"
                  "\033[0m");
    }

} // namespace yycctest::carton::termcolor
