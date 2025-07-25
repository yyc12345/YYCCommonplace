#pragma once

// Check endian
#if (defined(YYCC_ENDIAN_LITTLE) + defined(YYCC_ENDIAN_BIG)) != 1
#error "Current system endian (byte order) is not supported!"
#endif

namespace yycc::macro::endian {

    /// @brief The endian kind of OS.
    enum class EndianKind {
        Little, ///< Little endian.
        Big,    ///< Big endian.
    };

    /**
     * @brief Fetch the endian of OS.
     * @return The endian of OS.
     */
    inline constexpr EndianKind get_endian() {
#if defined(YYCC_ENDIAN_LITTLE)
        return EndianKind::Little;
#else
        return EndianKind::Big;
#endif
    }

} // namespace yycc::macro::endian
