#include "parser.hpp"
#include "types.hpp"
#include "../../env.hpp"
#include <stdexcept>
#include <ranges>

#define TYPES ::yycc::carton::clap::types
#define APPLICATION ::yycc::carton::clap::application
#define ENV ::yycc::env

namespace yycc::carton::clap::parser {

#pragma region Misc

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

    /// @brief The states of parser internal state machine.
    enum class ParserState {
        Normal,       ///< Normal state. Expect an option.
        WaitingValue, ///< Waiting an associated value.
    };
    /// @brief The state machine context prepared for parser.
    struct ParserContext {
        ParserContext(const APPLICATION::Application& app) : state(ParserState::Normal), opt_waiting(std::nullopt), app(app), values() {}
        // We have a "const" reference member so we delete these.
        YYCC_DELETE_COPY_MOVE(ParserContext)

        ParserState state;                                           ///< Current state.
        std::optional<TYPES::Token> opt_waiting;                     ///< The token to the option waiting for associated value.
        const APPLICATION::Application& app;                         ///< Associated application.
        std::map<TYPES::Token, std::optional<std::u8string>> values; ///< The container storing captured options.
    };

#pragma endregion

#pragma region Core

    /// @brief Core capture function.
    template<std::ranges::viewable_range V>
    static TYPES::ClapResult<std::map<TYPES::Token, std::optional<std::u8string>>> capture(const APPLICATION::Application& app, V&& args) {
        // Create context.
        ParserContext ctx(app);

        // Fetch commandline arguments
        // And skip the first argument because it is the path to executable.
        // Then start to execute until all arguments are consumed.
        for (const auto& arg : args | std::views::drop(1)) {
            // Fetch argument kind
            ClassifiedArgument classified_arg(arg);

            // Execute handler according to state.
            TYPES::ClapResult<void> rv;
            switch (ctx.state) {
                case ParserState::Normal:
                    rv = normal_state(ctx, classified_arg);
                    break;
                case ParserState::WaitingValue:
                    rv = waiting_value_state(ctx, classified_arg);
                    break;
            }
            if (!rv.has_value()) {
                return std::unexpected(rv.error());
            }
        }

        // If the final state is waiting value,
        // it means that the last option lose its asociated value.
        // So we need report error.
        if (ctx.state == ParserState::WaitingValue && ctx.opt_waiting.has_value()) {
            return std::unexpected(TYPES::ClapError::LostValue);
        }

        // Return capture result.
        return ctx.values;
    }

    /// @brief The handler for state machine Normal state.
    static TYPES::ClapResult<void> normal_state(ParserContext& ctx, const ClassifiedArgument& arg) {
        // Do thing according all registered options.
        const auto& options = ctx.app.get_options();

        // In normal state, we should fetch a name.
        // Check whether this name is existing, report error if not.
        std::optional<TYPES::Token> opt_token;
        switch (arg.kind) {
            // We can not meet associated value in normal state.
            case ArgumentKind::Value:
                return std::unexpected(TYPES::ClapError::UnexpectedValue);
            // Fetch token according to long name or short name.
            case ArgumentKind::LongName:
                opt_token = options.find_long_name(arg.content);
                break;
            case ArgumentKind::ShortName:
                opt_token = options.find_short_name(arg.content);
                break;
        }
        if (!opt_token.has_value()) {
            return std::unexpected(TYPES::ClapError::InvalidName);
        }
        TYPES::Token token = opt_token.value();

        // Check whether this token has been captured.
        // If it is, report error.
        if (ctx.values.contains(token)) {
            return std::unexpected(TYPES::ClapError::DuplicatedAssign);
        }

        // Finally, check whether this token has associated value.
        // If it has, set state machine, otherwise push it into capture list directly.
        if (options.get_option(token).has_value()) {
            ctx.opt_waiting = token;
            ctx.state = ParserState::WaitingValue;
        } else {
            ctx.values.emplace(token, std::nullopt);
        }

        return {};
    }

    /// @brief The handler for state machine WaitingValue state.
    static TYPES::ClapResult<void> waiting_value_state(ParserContext& ctx, const ClassifiedArgument& arg) {
        // Do thing according all registered options.
        const auto& options = ctx.app.get_options();
        TYPES::Token token = ctx.opt_waiting.value();
        switch (arg.kind) {
            case ArgumentKind::LongName:
            case ArgumentKind::ShortName:
                // Got option name when waiting associated value.
                // It means that previous option lost associated value.
                return std::unexpected(TYPES::ClapError::LostValue);
            case ArgumentKind::Value:
                // We got associated value.
                // Push it with option waiting value into capture list.
                ctx.values.emplace(token, arg.content);
                // Clear token waiting value.
                ctx.opt_waiting = std::nullopt;
                // Reset state at last.
                ctx.state = ParserState::Normal;
        }

        return {};
    }

#pragma endregion

#pragma region Parser Class

    TYPES::ClapResult<Parser> Parser::from_user(const APPLICATION::Application& app, const std::vector<std::u8string_view>& args) {
        auto rv = capture(app, std::views::all(args));
        if (rv.has_value()) return Parser(std::move(rv.value()));
        else return std::unexpected(rv.error());
    }

    TYPES::ClapResult<Parser> Parser::from_system(const APPLICATION::Application& app) {
        auto args = ENV::get_args();
        auto rv = capture(app, args | std::views::transform([](const auto& s) {
                                   return std::u8string_view(s);
                               }));
        if (rv.has_value()) return Parser(std::move(rv.value()));
        else return std::unexpected(rv.error());
    }

    Parser::Parser(decltype(Parser::values)&& value) : values(std::move(values)) {}

    Parser::~Parser() {}

    bool Parser::has_option(TYPES::Token token) const {
        return this->values.contains(token);
    }

    TYPES::ClapResult<bool> Parser::get_flag_option(TYPES::Token token) const {
        auto finder = this->values.find(token);
        if (finder == this->values.end()) {
            // Not found.
            return false;
        } else {
            // Found.
            auto val = finder->second;
            if (val.has_value()) throw std::logic_error("get flag option as value option.");
            else return true;
        }
    }

    TYPES::ClapResult<std::u8string_view> Parser::get_raw_value_option(TYPES::Token token) const {
        auto finder = this->values.find(token);
        if (finder == this->values.end()) {
            // Not found.
            return std::unexpected(TYPES::ClapError::NotCaptured);
        } else {
            // Found.
            auto val = finder->second;
            if (val.has_value()) return std::u8string_view(val.value());
            else throw std::logic_error("get value option as flag option.");
        }
    }

#pragma endregion

} // namespace yycc::carton::clap::parser
