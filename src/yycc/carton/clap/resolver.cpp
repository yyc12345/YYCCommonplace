#include "resolver.hpp"
#include "types.hpp"

#define TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::resolver {

#pragma region Core

#pragma endregion

#pragma region Resolver Class

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
            auto val = finder->second;
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
            auto val = finder->second;
            if (val.has_value()) return std::u8string_view(val.value());
            else throw std::logic_error("get value variable as flag variable.");
        }
    }

#pragma endregion

} // namespace yycc::carton::clap::resolver
