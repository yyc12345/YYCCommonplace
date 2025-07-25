#pragma once

// Check OS macro
#if (defined(YYCC_OS_WINDOWS) + defined(YYCC_OS_LINUX)) != 1
#error "Current operating system is not supported!"
#endif

namespace yycc::macro::os {

    /// @brief The operating system kind.
    enum class OsKind {
        Windows, ///< Microsoft Windows
        Linux,   ///< GNU/Linux
    };

    /**
     * @brief Fetch the operating system
     * @return The kind of operating system.
     */
    inline constexpr OsKind get_os() {
#if defined(YYCC_OS_WINDOWS)
        return OsKind::Windows;
#else
        return OsKind::Linux;
#endif
    }

} // namespace yycc::macro::os
