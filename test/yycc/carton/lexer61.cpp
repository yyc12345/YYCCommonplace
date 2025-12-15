#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/lexer61.hpp>

#define LEXER61 ::yycc::carton::lexer61

namespace yycctest::carton::lexer61 {

    TEST(CartonLexer61, EmptyCommand) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        EXPECT_TRUE(value.empty());
    }

    TEST(CartonLexer61, SingleArgument) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"program");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 1);
        EXPECT_EQ(value[0], u8"program");
    }

    TEST(CartonLexer61, MultipleArguments) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"program arg1 arg2 arg3");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 4);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg1");
        EXPECT_EQ(value[2], u8"arg2");
        EXPECT_EQ(value[3], u8"arg3");
    }

    TEST(CartonLexer61, LeadingSpaces) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"   program arg");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 2);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg");
    }

    TEST(CartonLexer61, TrailingSpaces) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"program arg   ");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 2);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg");
    }

    TEST(CartonLexer61, EmbeddedSpaces) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"program    arg1     arg2");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg1");
        EXPECT_EQ(value[2], u8"arg2");
    }

    TEST(CartonLexer61, SingleQuotedArgs) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"program 'arg with spaces' arg2");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg with spaces");
        EXPECT_EQ(value[2], u8"arg2");
    }

    TEST(CartonLexer61, DoubleQuotedArgs) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(program "arg with spaces" arg2)");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"arg with spaces");
        EXPECT_EQ(value[2], u8"arg2");
    }

    TEST(CartonLexer61, MixedQuotes) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(program "double quoted 'single inside'" 'single quoted "double inside"')");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"double quoted 'single inside'");
        EXPECT_EQ(value[2], u8"single quoted \"double inside\"");
    }

    TEST(CartonLexer61, EscapedCharacters) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(program escaped\ space "quoted with \" quote" 'single with \' quote')");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 4);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"escaped space");
        EXPECT_EQ(value[2], u8"quoted with \" quote");
        EXPECT_EQ(value[3], u8"single with \' quote");
    }

    TEST(CartonLexer61, ChineseCharacters) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8"程序 中文 参数");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"程序");
        EXPECT_EQ(value[1], u8"中文");
        EXPECT_EQ(value[2], u8"参数");
    }

    TEST(CartonLexer61, ChineseWithQuotes) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(程序 "中文 参数" '另一个"引号"参数')");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"程序");
        EXPECT_EQ(value[1], u8"中文 参数");
        EXPECT_EQ(value[2], u8"另一个\"引号\"参数");
    }

    TEST(CartonLexer61, UnclosedQuoteError) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(program "unclosed quote)");
        ASSERT_FALSE(result.has_value());
    }

    TEST(CartonLexer61, EmptyArgumentInQuotes) {
        LEXER61::Lexer61 lexer;
        auto result = lexer.lex(u8R"(program "" '')");
        ASSERT_TRUE(result.has_value());

        auto value = std::move(result.value());
        ASSERT_EQ(value.size(), 3);
        EXPECT_EQ(value[0], u8"program");
        EXPECT_EQ(value[1], u8"");
        EXPECT_EQ(value[2], u8"");
    }

} // namespace yycctest::carton::lexer61