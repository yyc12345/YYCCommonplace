#pragma once

// Check pointer size macro
#if (defined(YYCC_PTRSIZE_32) + defined(YYCC_PTRSIZE_64)) != 1
#error "Current environment used pointer size is not supported!"
#endif

namespace yycc::macro::ptr_size {

    /// @brief The pointer size kind.
    enum class PtrSizeKind {
        Bits32, ///< 32-bit environment
        Bits64  ///< 64-bit environment
    };

    /**
     * @brief Fetch the pointer size
     * @return The kind of pointer size.
     */
    inline constexpr PtrSizeKind get_ptr_size() {
#if defined(YYCC_PTRSIZE_32)
        return PtrSizeKind::Bits32;
#else
        return PtrSizeKind::Bits64;
#endif
    }

} // namespace yycc::macro::ptr_size
