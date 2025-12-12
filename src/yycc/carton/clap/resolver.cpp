#include "resolver.hpp"
#include "../../env.hpp"
#include <stdexcept>
#include <ranges>

#define TYPES ::yycc::carton::clap::types
#define APPLICATION ::yycc::carton::clap::application
#define ENV ::yycc::env

namespace yycc::carton::clap::resolver {

#pragma region Core

    /// @brief Core capture function
    template<std::ranges::viewable_range V>
    static TYPES::ClapResult<std::map<TYPES::Token, std::optional<std::u8string>>> capture(const APPLICATION::Application& app, V&& vars) {
        std::map<TYPES::Token, std::optional<std::u8string>> values;
        const auto& variables = app.get_variables();

        // Iterate variable list and process them one by one.
        for (const auto& [name, value] : vars) {
            // Check whether capture this variable.
            auto opt_token = variables.find_name(name);
            if (!opt_token.has_value()) continue;
            auto token = opt_token.value();

            // Check whether it is captured.
            // If it is already captured, report error,
            // because environment variable can not be duplicated.
            if (values.contains(token)) {
                throw std::runtime_error("impossible duplicated environment variable name.");
            }

            // Finally check whether it care value, and insert data.
            if (variables.get_variable(token).is_care_value()) {
                values.emplace(token, value);
            } else {
                values.emplace(token, std::nullopt);
            }
        }

        return values;
    }

#pragma endregion

#pragma region Resolver Class

    TYPES::ClapResult<Resolver> Resolver::from_user(const APPLICATION::Application& app,
                                                    const std::vector<std::pair<std::u8string_view, std::u8string_view>>& vars) {
        auto rv = capture(app, std::views::all(vars));
        if (rv.has_value()) return Resolver(std::move(rv.value()));
        else return std::unexpected(rv.error());
    }

    TYPES::ClapResult<Resolver> Resolver::from_system(const APPLICATION::Application& app) {
        auto rv_vars = ENV::get_vars();
        if (!rv_vars.has_value()) return std::unexpected(TYPES::ClapError::Others);
        auto vars = std::move(rv_vars.value());

        auto rv = capture(app, vars | std::views::transform([](const auto& p) {
                                   return std::make_pair<std::u8string_view, std::u8string_view>(p.first, p.second);
                               }));
        if (rv.has_value()) return Resolver(std::move(rv.value()));
        else return std::unexpected(rv.error());
    }

    Resolver::Resolver(decltype(Resolver::values)&& values) : values(std::move(values)) {}

    Resolver::~Resolver() {}

    bool Resolver::has_variable(TYPES::Token token) const {
        return this->values.contains(token);
    }

    TYPES::ClapResult<bool> Resolver::get_flag_variable(TYPES::Token token) const {
        auto finder = this->values.find(token);
        if (finder == this->values.end()) {
            // Not found.
            return false;
        } else {
            // Found.
            const auto& val = finder->second;
            if (val.has_value()) throw std::logic_error("get flag variable as value variable.");
            else return true;
        }
    }

    TYPES::ClapResult<std::u8string_view> Resolver::get_raw_value_variable(TYPES::Token token) const {
        auto finder = this->values.find(token);
        if (finder == this->values.end()) {
            // Not found.
            return std::unexpected(TYPES::ClapError::NotCaptured);
        } else {
            // Found.
            const auto& val = finder->second;
            if (val.has_value()) return std::u8string_view(val.value());
            else throw std::logic_error("get value variable as flag variable.");
        }
    }

#pragma endregion

} // namespace yycc::carton::clap::resolver
