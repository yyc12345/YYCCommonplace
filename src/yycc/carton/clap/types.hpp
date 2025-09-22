#pragma once
#include <expected>
#include <string_view>

namespace yycc::carton::clap::types {

    enum class ClapError {

    };

    template<typename T>
    using ClapResult = std::expected<T, ClapError>;

    inline constexpr std::u8string_view DASH = u8"-";
    inline constexpr std::u8string_view DOUBLE_DASH = u8"--";

    using Token = size_t;

} // namespace yycc::carton::clap::types
