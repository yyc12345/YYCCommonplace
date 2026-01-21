#include "option.hpp"
#include "../../patch/format.hpp"
#include <stdexcept>

#define TYPES ::yycc::carton::clap::types
#define FORMAT ::yycc::patch::format

namespace yycc::carton::clap::option {

#pragma region Option

    Option::Option(std::optional<std::u8string_view> short_name,
                   std::optional<std::u8string_view> long_name,
                   std::optional<std::u8string_view> value_hint,
                   const std::u8string_view& description) :
        short_name(short_name), long_name(long_name), value_hint(value_hint), description(description) {
        if (!short_name.has_value() && !long_name.has_value()) {
            throw std::logic_error("must have at least one name, short or long name");
        }

        if (short_name.has_value()) {
            const auto& short_name_value = short_name.value();
            if (!legal_short_name(short_name_value)) {
                throw std::logic_error(FORMAT::format("invalid short name {}", short_name_value));
            }
        }
        if (long_name.has_value()) {
            const auto& long_name_value = long_name.value();
            if (!legal_long_name(long_name_value)) {
                throw std::logic_error(FORMAT::format("invalid long name {}", long_name_value));
            }
        }
    }

    Option::~Option() {}

    bool Option::has_value() const {
        return this->value_hint.has_value();
    }

    std::optional<std::u8string_view> Option::get_short_name() const {
        return this->short_name;
    }

    std::optional<std::u8string_view> Option::get_long_name() const {
        return this->long_name;
    }

    std::optional<std::u8string_view> Option::get_value_hint() const {
        return this->value_hint;
    }

    std::u8string_view Option::get_description() const {
        return this->description;
    }

    std::u8string Option::to_showcase_name() const {
        if (short_name.has_value()) {
            if (long_name.has_value()) {
                return FORMAT::format(u8"{}{} {}{}", TYPES::DASH, short_name.value(), TYPES::DOUBLE_DASH, long_name.value());
            } else {
                return FORMAT::format(u8"{}{}", TYPES::DASH, short_name.value());
            }
        } else {
            if (long_name.has_value()) {
                return FORMAT::format(u8"{}{}", TYPES::DOUBLE_DASH, long_name.value());
            } else {
                throw std::runtime_error("both long name and short name are empty");
            }
        }
    }

    std::u8string Option::to_showcase_value() const {
        if (value_hint.has_value()) {
            return FORMAT::format(u8"<{}>", value_hint.value());
        } else {
            return {};
        }
    }

    bool Option::legal_short_name(const std::u8string_view& name) {
        if (name.empty()) return false;
        if (name.starts_with(TYPES::DASH)) return false;
        return true;
    }

    bool Option::legal_long_name(const std::u8string_view& name) {
        if (name.empty()) return false;
        return true;
    }

#pragma endregion

#pragma region Registered Option

    RegisteredOption::RegisteredOption(TYPES::Token token, Option&& option) : token(token), option(std::move(option)) {}

    RegisteredOption::~RegisteredOption() {}

    TYPES::Token RegisteredOption::get_token() const {
        return this->token;
    }

    const Option& RegisteredOption::get_option() const {
        return this->option;
    }

#pragma endregion

#pragma region Option Collection

    OptionCollection::OptionCollection() : short_names(), long_names(), options() {}

    OptionCollection::~OptionCollection() {}

    TYPES::Token OptionCollection::add_option(Option&& opt) {
        auto token = this->options.size();

        const auto& short_name = opt.get_short_name();
        if (short_name.has_value()) {
            std::u8string short_name_value(short_name.value());
            if (this->long_names.contains(short_name_value)) {
                throw std::logic_error(
                    FORMAT::format("short name {} is duplicated with same long name", short_name_value));
            }
            auto [_, ok] = this->short_names.try_emplace(short_name_value, token);
            if (!ok) {
                throw std::logic_error(FORMAT::format("duplicate short name {}", short_name_value));
            }
        }
        const auto& long_name = opt.get_long_name();
        if (long_name.has_value()) {
            std::u8string long_name_value(long_name.value());
            if (this->short_names.contains(long_name_value)) {
                throw std::logic_error(
                    FORMAT::format("long name {} is duplicated with same short name", long_name_value));
            }
            auto [_, ok] = this->long_names.try_emplace(long_name_value, token);
            if (!ok) {
                throw std::logic_error(FORMAT::format("duplicate long name {}", long_name_value));
            }
        }

        this->options.emplace_back(RegisteredOption(token, std::move(opt)));
        return token;
    }

    std::optional<TYPES::Token> OptionCollection::find_long_name(const std::u8string_view& long_name) const {
        auto finder = this->long_names.find(std::u8string(long_name));
        if (finder == this->long_names.end()) return std::nullopt;
        else return finder->second;
    }

    std::optional<TYPES::Token> OptionCollection::find_short_name(const std::u8string_view& short_name) const {
        auto finder = this->short_names.find(std::u8string(short_name));
        if (finder == this->short_names.end()) return std::nullopt;
        else return finder->second;
    }

    bool OptionCollection::has_option(TYPES::Token token) const {
        return token < this->options.size();
    }

    const Option& OptionCollection::get_option(TYPES::Token token) const {
        return this->options.at(token).get_option();
    }

    const std::vector<RegisteredOption>& OptionCollection::all_options() const {
        return this->options;
    }

    size_t OptionCollection::length() const {
        return this->options.size();
    }

    bool OptionCollection::empty() const {
        return this->options.empty();
    }

#pragma endregion

} // namespace yycc::carton::clap::option
