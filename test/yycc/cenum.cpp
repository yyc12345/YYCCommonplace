#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/cenum.hpp>
#include <cinttypes>

#include <yycc/prelude.hpp>

#define CENUM ::yycc::cenum

namespace yycctest::cenum {

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

    TEST(CEnum, Merge) {
        EXPECT_EQ(CENUM::merge(TestEnum::Bit2, TestEnum::Bit4, TestEnum::Bit7), TestEnum::MergedBit247);
    }

    TEST(CEnum, Invert) {
        EXPECT_EQ(CENUM::invert(TestEnum::Bit8), TestEnum::InvBit8);
    }

    TEST(CEnum, Mask) {
        TestEnum src = CENUM::merge(TestEnum::Bit2, TestEnum::Bit4);
        TestEnum val;

        val = src;
        CENUM::mask(val, TestEnum::Bit2);
        EXPECT_EQ(val, TestEnum::Bit2);

        val = src;
        CENUM::mask(val, TestEnum::Bit4);
        EXPECT_EQ(val, TestEnum::Bit4);

        val = src;
        CENUM::mask(val, TestEnum::Bit3);
        EXPECT_EQ(val, TestEnum::Empty);
    }

    TEST(CEnum, Add) {
        TestEnum val = TestEnum::Bit2;
        CENUM::add(val, TestEnum::Bit4, TestEnum::Bit7);
        EXPECT_EQ(val, TestEnum::MergedBit247);
    }

    TEST(CEnum, Remove) {
        TestEnum val = TestEnum::MergedBit247;
        CENUM::remove(val, TestEnum::Bit2, TestEnum::Bit7);
        EXPECT_EQ(val, TestEnum::Bit4);
    }

    TEST(CEnum, Has) {
        TestEnum val = TestEnum::MergedBit247;
        EXPECT_TRUE(CENUM::has(val, TestEnum::Bit2));
        EXPECT_FALSE(CENUM::has(val, TestEnum::Bit3));
        EXPECT_TRUE(CENUM::has(val, TestEnum::Bit4));
        EXPECT_TRUE(CENUM::has(val, TestEnum::Bit7));
    }

    TEST(CEnum, Boolean) {
        EXPECT_FALSE(CENUM::boolean(TestEnum::Empty));
        EXPECT_TRUE(CENUM::boolean(TestEnum::Bit1));
        EXPECT_TRUE(CENUM::boolean(TestEnum::InvBit8));
        EXPECT_TRUE(CENUM::boolean(TestEnum::MergedBit247));
    }

    TEST(CEnum, Integer) {
        EXPECT_EQ(CENUM::integer(TestEnum::Empty), UINT8_C(0));
        EXPECT_EQ(CENUM::integer(TestEnum::Bit1), UINT8_C(1));
    }

}
