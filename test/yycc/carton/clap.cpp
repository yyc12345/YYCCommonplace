#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/carton/clap.hpp>
#include <yycc/prelude.hpp>

#define CLAP ::yycc::carton::clap

namespace yycctest::carton::clap {

    using Token = CLAP::types::Token;
    namespace validator = CLAP::validator;

    TEST(CartonClap, BadOption) {
        // No short name and long name.
        EXPECT_ANY_THROW(auto opt = CLAP::option::Option(std::nullopt, std::nullopt, std::nullopt, u8""));
        // Empty short name or long name.
        EXPECT_ANY_THROW(auto opt = CLAP::option::Option(u8"", std::nullopt, std::nullopt, u8""));
        EXPECT_ANY_THROW(auto opt = CLAP::option::Option(std::nullopt, u8"", std::nullopt, u8""));
        // Bad short name
        EXPECT_ANY_THROW(auto opt = CLAP::option::Option(u8"fuck", std::nullopt, std::nullopt, u8""));
        EXPECT_ANY_THROW(auto opt = CLAP::option::Option(u8"-", std::nullopt, std::nullopt, u8""));
    }

    TEST(CartonClap, BadOptions) {
        // Duplicated.
        {
            auto options = CLAP::option::OptionCollection();
            options.add_option(CLAP::option::Option(u8"a", std::nullopt, std::nullopt, u8""));
            EXPECT_ANY_THROW(options.add_option(CLAP::option::Option(u8"a", std::nullopt, std::nullopt, u8"")));
        }
        // Duplicate between long name and short name.
        {
            auto options = CLAP::option::OptionCollection();
            options.add_option(CLAP::option::Option(u8"a", std::nullopt, std::nullopt, u8""));
            EXPECT_ANY_THROW(options.add_option(CLAP::option::Option(std::nullopt, u8"a", std::nullopt, u8"")));
        }
    }

    TEST(CartonClap, BadVariable) {
        // Empty name
        EXPECT_ANY_THROW(auto var = CLAP::variable::Variable(u8"", u8"", true));
    }

    TEST(CartonClap, BadVariables) {
        // Duplicated.
        {
            auto variables = CLAP::variable::VariableCollection();
            variables.add_variable(CLAP::variable::Variable(u8"a", u8"", true));
            EXPECT_ANY_THROW(variables.add_variable(CLAP::variable::Variable(u8"a", u8"", true)));
        }
    }

    class CartonClap : public ::testing::Test {
    protected:
        CartonClap() :
            // YYC MARK:
            // The fucking C++ must order I initialize this class which do not have default ctor in there.
            // So I forcely initialize it with dummy data and reset it in ctor body.
            // The reason why I do not use static function to intialize this is that I also need specify some class members when creating this.
            app(CLAP::summary::Summary(u8"", u8"", u8"", u8""), CLAP::option::OptionCollection(), CLAP::variable::VariableCollection()) {
            // Setup basic infos
            auto summary = CLAP::summary::Summary(u8"TestClap", u8"yyc12345", u8"1.0.0", u8"This is the test of clap.");

            // Add options
            auto options = CLAP::option::OptionCollection();
            int_option = options.add_option(CLAP::option::Option(u8"i", u8"int", u8"<integer>", u8"integral argument"));
            float_option = options.add_option(CLAP::option::Option(u8"f", std::nullopt, u8"<float>", u8""));
            string_option = options.add_option(CLAP::option::Option(std::nullopt, u8"string", u8"<string>", u8""));
            clamped_float_option = options.add_option(CLAP::option::Option(std::nullopt, u8"clamped-float", u8"<float>", u8""));
            novalue_option = options.add_option(CLAP::option::Option(u8"b", std::nullopt, std::nullopt, u8""));

            // Add variables
            auto variables = CLAP::variable::VariableCollection();
            int_variable = variables.add_variable(CLAP::variable::Variable(u8"int", u8"", true));
            clamped_int_variable = variables.add_variable(CLAP::variable::Variable(u8"clamped-int", u8"", true));
            novalue_variable = variables.add_variable(CLAP::variable::Variable(u8"b", u8"", false));

            // Create final application
            app = CLAP::application::Application(std::move(summary), std::move(options), std::move(variables));
        }
        ~CartonClap() override = default;

        // Options
        Token int_option;
        using IntOptionValidator = validator::IntegralValidator<i32>;
        Token float_option;
        using FloatOptionValidator = validator::FloatingPointValidator<float>;
        Token string_option;
        using StringOptionValidator = validator::StringValidator;
        Token clamped_float_option;
        using ClampedFloatOptionValidator = validator::FloatingPointValidator<float, -1.0f, 1.0f>;
        Token novalue_option;
        // Variables
        Token int_variable;
        using IntVariableValidator = validator::IntegralValidator<i32>;
        Token clamped_int_variable;
        using ClampedFloatVariableValidator = validator::FloatingPointValidator<float, -1.0f, 1.0f>;
        Token novalue_variable;

        // Application
        CLAP::application::Application app;
    };

    TEST_F(CartonClap, Manual) {
        std::stringstream ss;
        auto manual = CLAP::manual::Manual(app);

        // TODO: It may be more precious check for this.
        // But I don't want to write it anymore.

        // Two manual output should not be empty.
        ss.str("");
        manual.print_version(ss);
        EXPECT_FALSE(ss.view().empty());

        ss.str("");
        manual.print_help(ss);
        EXPECT_FALSE(ss.view().empty());
    }

    TEST_F(CartonClap, ParserNormal) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-i", u8"114514"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_TRUE(result.has_value());
        auto parser = std::move(result.value());

        // Check that int argument was captured with correct value
        {
            auto value = parser.get_value_option<IntOptionValidator>(int_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), 114514);
        }
        // Check that other arguments were not captured
        {
            auto value = parser.get_value_option<FloatOptionValidator>(float_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_value_option<StringOptionValidator>(string_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_value_option<ClampedFloatOptionValidator>(clamped_float_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_flag_option(novalue_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), false);
        }
    }

    TEST_F(CartonClap, ParserNothing) {
        const std::vector<std::u8string_view> args = {u8"exec"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_TRUE(result.has_value());
        auto parser = std::move(result.value());

        // Check that all arguments were not captured
        {
            auto value = parser.get_value_option<IntOptionValidator>(int_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_value_option<FloatOptionValidator>(float_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_value_option<StringOptionValidator>(string_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_value_option<ClampedFloatOptionValidator>(clamped_float_option);
            EXPECT_FALSE(value.has_value());
        }
        {
            auto value = parser.get_flag_option(novalue_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), false);
        }
    }

    TEST_F(CartonClap, ParserInvalidName) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-?", u8"114514"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_FALSE(result.has_value());
    }

    TEST_F(CartonClap, ParserDuplicatedAssign) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-i", u8"114514", u8"--int", u8"114514"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_FALSE(result.has_value());
    }

    TEST_F(CartonClap, ParserUnexpectedValue) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-i", u8"114514", u8"1919810"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_FALSE(result.has_value());
    }

    TEST_F(CartonClap, ParserLostValue) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-i"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_FALSE(result.has_value());
    }

    TEST_F(CartonClap, ParserBadCast) {
        const std::vector<std::u8string_view> args = {u8"exec", u8"-i", u8"114514", u8"--clamped-float", u8"114.0"};
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_TRUE(result.has_value());
        auto parser = std::move(result.value());

        // Check okey cast
        {
            auto value = parser.get_value_option<IntOptionValidator>(int_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), 114514);
        }
        // Check bad cast
        {
            auto value = parser.get_value_option<ClampedFloatOptionValidator>(clamped_float_option);
            EXPECT_FALSE(value.has_value());
        }
    }

    TEST_F(CartonClap, ParserFull) {
        const std::vector<std::u8string_view> args = {
            u8"exec", // Program self.
            u8"-i",   // Int option
            u8"114514",
            u8"-f", // Float option
            u8"2.0",
            u8"--string", // String option
            u8"fuck",
            u8"--clamped-float", // Clamped flpat option
            u8"0.5",
            u8"-b", // No value option
        };
        auto result = CLAP::parser::Parser::from_user(app, args);
        ASSERT_TRUE(result.has_value());
        auto parser = std::move(result.value());

        // Check that all options were not captured
        {
            auto value = parser.get_value_option<IntOptionValidator>(int_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), 114514);
        }
        {
            auto value = parser.get_value_option<FloatOptionValidator>(float_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), 2.0f);
        }
        {
            auto value = parser.get_value_option<StringOptionValidator>(string_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), u8"fuck");
        }
        {
            auto value = parser.get_value_option<ClampedFloatOptionValidator>(clamped_float_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), 0.5f);
        }
        {
            auto value = parser.get_flag_option(novalue_option);
            ASSERT_TRUE(value.has_value());
            EXPECT_EQ(value.value(), true);
        }
    }

} // namespace yycctest::carton::clap