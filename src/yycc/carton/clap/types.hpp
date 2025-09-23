#pragma once
#include <expected>
#include <string_view>

namespace yycc::carton::clap::types {

    /// @brief All possible error kind occurs in this module.
    enum class ClapError {

    };

    /// @brief The result type used in this module.
    template<typename T>
    using ClapResult = std::expected<T, ClapError>;

    /// @brief The dash prefix used for short name of option.
    inline constexpr std::u8string_view DASH = u8"-";
    /// @brief The double dash prefix used by long name of option.
    inline constexpr std::u8string_view DOUBLE_DASH = u8"--";

    /**
     * @brief An unique token type.
     * @details
     * When outside code registering an option or variable,
     * there must be a token returned by manager.
     * When outside code want to visit this registered item again,
     * they should provide this token returned when registering.
     * 
     * Its value actually is the index of its stored vector.
     * So this type is an alias to vector size type.
     */
    using Token = size_t;

} // namespace yycc::carton::clap::types
