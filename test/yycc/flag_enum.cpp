#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/flag_enum.hpp>
#include <cinttypes>

#include <yycc/prelude.hpp>

#define FLAG_ENUM ::yycc::flag_enum

namespace yycctest::flag_enum {

    enum class TestEnum : u8 {
        Bit1 = 0b00000001,
        Bit2 = 0b00000010,
        Bit3 = 0b00000100,
        Bit4 = 0b00001000,
        Bit5 = 0b00010000,
        Bit6 = 0b00100000,
        Bit7 = 0b01000000,
        Bit8 = 0b10000000,
        Empty = 0b00000000,
        InvBit8 = 0b01111111,
        MergedBit247 = Bit2 + Bit4 + Bit7,
    };

    TEST(FlagEnum, Merge) {
        EXPECT_EQ(FLAG_ENUM::merge(TestEnum::Bit2, TestEnum::Bit4, TestEnum::Bit7), TestEnum::MergedBit247);
    }

    TEST(FlagEnum, Invert) {
        EXPECT_EQ(FLAG_ENUM::invert(TestEnum::Bit8), TestEnum::InvBit8);
    }

    TEST(FlagEnum, Mask) {
        TestEnum src = FLAG_ENUM::merge(TestEnum::Bit2, TestEnum::Bit4);
        TestEnum val;

        val = src;
        FLAG_ENUM::mask(val, TestEnum::Bit2);
        EXPECT_EQ(val, TestEnum::Bit2);

        val = src;
        FLAG_ENUM::mask(val, TestEnum::Bit4);
        EXPECT_EQ(val, TestEnum::Bit4);

        val = src;
        FLAG_ENUM::mask(val, TestEnum::Bit3);
        EXPECT_EQ(val, TestEnum::Empty);
    }

    TEST(FlagEnum, Add) {
        TestEnum val = TestEnum::Bit2;
        FLAG_ENUM::add(val, TestEnum::Bit4, TestEnum::Bit7);
        EXPECT_EQ(val, TestEnum::MergedBit247);
    }

    TEST(FlagEnum, Remove) {
        TestEnum val = TestEnum::MergedBit247;
        FLAG_ENUM::remove(val, TestEnum::Bit2, TestEnum::Bit7);
        EXPECT_EQ(val, TestEnum::Bit4);
    }

    TEST(FlagEnum, Has) {
        TestEnum val = TestEnum::MergedBit247;
        EXPECT_TRUE(FLAG_ENUM::has(val, TestEnum::Bit2));
        EXPECT_FALSE(FLAG_ENUM::has(val, TestEnum::Bit3));
        EXPECT_TRUE(FLAG_ENUM::has(val, TestEnum::Bit4));
        EXPECT_TRUE(FLAG_ENUM::has(val, TestEnum::Bit7));
    }

    TEST(FlagEnum, Boolean) {
        EXPECT_FALSE(FLAG_ENUM::boolean(TestEnum::Empty));
        EXPECT_TRUE(FLAG_ENUM::boolean(TestEnum::Bit1));
        EXPECT_TRUE(FLAG_ENUM::boolean(TestEnum::InvBit8));
        EXPECT_TRUE(FLAG_ENUM::boolean(TestEnum::MergedBit247));
    }

    TEST(FlagEnum, Integer) {
        EXPECT_EQ(FLAG_ENUM::integer(TestEnum::Empty), UINT8_C(0));
        EXPECT_EQ(FLAG_ENUM::integer(TestEnum::Bit1), UINT8_C(1));
    }

}
