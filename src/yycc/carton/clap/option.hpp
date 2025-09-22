#pragma once
#include "types.hpp"
#include "../../macro/class_copy_move.hpp"
#include "../../string/op.hpp"
#include <optional>
#include <stdexcept>
#include <format>
#include <string>

#define NS_YYCC_CLAP_TYPES ::yycc::carton::clap::types

namespace yycc::carton::clap::option {

    class Option {
    public:
        Option(std::optional<std::u8string_view> short_name,
               std::optional<std::u8string_view> long_name,
               std::optional<std::u8string_view> value_hint,
               const std::u8string& description) :
            short_name(short_name), long_name(long_name), value_hint(value_hint), description(description) {
            if (!short_name.has_value() && !long_name.has_value()) {
                throw std::logic_error("must have at least one name, short or long name");
            }

            if (short_name.has_value()) {
                const auto& short_name_value = short_name.value();
                if (!legal_short_name(short_name_value)) {
                    throw std::logic_error(std::format("invalid short name {}", short_name_value));
                }
            }
            if (long_name.has_value()) {
                const auto& long_name_value = long_name.value();
                if (!legal_long_name(long_name_value)) {
                    throw std::logic_error(std::format("invalid long name {}", long_name_value));
                }
            }
        }
        ~Option() {}
        YYCC_DEFAULT_COPY_MOVE(Option)

    public:
        std::optional<std::u8string_view> get_short_name() const { return this->short_name; }
        std::optional<std::u8string_view> get_long_name() const { return this->long_name; }
        std::optional<std::u8string_view> get_value_hint() const { return this->value_hint; }
        std::u8string_view get_description() const { return this->description; }

        std::u8string to_showcase_name() {
            namespace op = ::yycc::string::op;

            if (short_name.has_value()) {
                if (long_name.has_value()) {
                    
                } else {
                
                }
            } else {
                if (long_name.has_value()) {
                    op::printf
                } else {
                    throw std::runtime_error("both long name and short name are empty");
                }
            }
        }

    private:
        static bool legal_short_name(const std::u8string_view& name) {
            if (name.empty()) return false;
            if (name.starts_with(NS_YYCC_CLAP_TYPES::DASH)) return false;
            return true;
        }
        static bool legal_long_name(const std::u8string_view& name) {
            if (name.empty()) return false;
            return true;
        }

    private:
        std::optional<std::u8string> short_name;
        std::optional<std::u8string> long_name;
        std::optional<std::u8string> value_hint;
        std::u8string description;
    };

} // namespace yycc::carton::clap::option

#undef NS_YYCC_CLAP_TYPES
