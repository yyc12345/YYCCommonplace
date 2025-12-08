#include "resolver.hpp"
#include "types.hpp"
#include <string_view>

#define TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::resolver {

#pragma region Misc

    /// @brief The states of parser internal state machine.
    enum class ParserState {
        Normal,       ///< Normal state. Expect an option.
        WaitingValue, ///< Waiting an associated value.
    };
    /// @brief The state machine context prepared for parser.
    struct ParserContext {
        ParserContext() : state(ParserState::Normal), opt_waiting(std::nullopt) {}
        YYCC_DEFAULT_COPY_MOVE(ParserContext)

        ParserState state;                       ///< Current state.
        std::optional<TYPES::Token> opt_waiting; ///< The token to the option waiting for associated value.
    };

    /// @brief The kind of argument.
    enum class ArgumentKind { LongName, ShortName, Value };
    /// @brief Representing a classified argument.
    struct ClassifiedArgument {
        ClassifiedArgument(const std::u8string_view& arg) {
            if (arg.starts_with(TYPES::DOUBLE_DASH)) {
                kind = ArgumentKind::LongName;
                content = arg.substr(TYPES::DOUBLE_DASH.length());
            } else if (arg.starts_with(TYPES::DASH)) {
                kind = ArgumentKind::ShortName;
                content = arg.substr(TYPES::DASH.length());
            } else {
                kind = ArgumentKind::Value;
                content = arg;
            }
        }
        YYCC_DEFAULT_COPY_MOVE(ClassifiedArgument)

        /// @brief The kind of argument.
        ArgumentKind kind;
        /**
         * @brief The data of argument.
         * @details For long and short name, it is the body of option, the words removing any leading dash.
         * For value, it just the value self.
         */
        std::u8string_view content;
    };

#pragma endregion

#pragma region Core

#pragma endregion

#pragma region Parser Class

    Parser::Parser(const NS_YYCC_CLAP_APPLICATION::Application& app) {}

    Parser::~Parser() {}

    bool Parser::has_option(TYPES::Token token) const {
        // TODO:
        return false;
    }

    TYPES::ClapResult<bool> Parser::get_flag_option(TYPES::Token token) const {
        // TODO:
        return TYPES::ClapResult<bool>();
    }

    TYPES::ClapResult<std::u8string_view> Parser::get_raw_value_option(TYPES::Token token) const {
        // TODO:
        return TYPES::ClapResult<std::u8string_view>();
    }
#pragma endregion

} // namespace yycc::carton::clap::resolver
