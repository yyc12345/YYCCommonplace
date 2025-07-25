#pragma once

#if (defined(YYCC_CC_MSVC) + defined(YYCC_CC_GCC) + defined(YYCC_CC_CLANG)) != 1
#error "Current compiler is not supported!"
#endif

namespace yycc::macro::compiler {

    /// @brief The kind of compiler.
    enum class CompilerKind {
        Msvc,  ///< MSVC
        Gcc,   ///< GCC
        Clang, ///< Clang
    };

    /**
     * @brief Fetch the compiler type.
     * @return The kind of compiler.
     */
    inline constexpr CompilerKind get_compiler() {
#if defined(YYCC_CC_MSVC)
        return CompilerKind::Msvc;
#elif defined(YYCC_CC_GCC)
        return CompilerKind::Gcc;
#else
        return CompilerKind::Clang;
#endif
    }

} // namespace yycc::macro::compiler
