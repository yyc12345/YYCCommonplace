#pragma once
#include <optional>

/// @brief The reproduction of Rust Option type.
/// @details
/// This namespace reproduce Rust Option type, and its members Some and None in C++.
/// However Option is not important than Result, so its implementation is very casual.
namespace yycc::rust::option {

    template<typename T>
    using Option = std::optional<T>;

    template<typename OptionType, typename... Args>
    OptionType Some(Args &&... args) {
        return OptionType(std::in_place, std::forward<Args>(args)...);
    }

    template<typename OptionType>
    OptionType None() {
        return OptionType(std::nullopt);
    }

}
