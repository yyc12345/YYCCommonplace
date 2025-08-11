#pragma once

// Include a common used STL header for convenient test.
#include <cinttypes>

#if defined(_MSVC_STL_VERSION)
#define YYCC_STL_MSSTL
#elif defined(__GLIBCXX__) || defined(__GLIBCPP__)
#define YYCC_STL_GNUSTL
#elif defined(_LIBCPP_VERSION)
#define YYCC_STL_CLANGSTL
#else
#error "Current STL is not supported!"
#endif

namespace yycc::macro::stl {

    /// @brief The STL implementation kind.
    enum class StlKind {
        MSSTL,    ///< Microsoft STL
        GNUSTL,   ///< GNU STL
        CLANGSTL  ///< Clang STL
    };

    /**
     * @brief Fetch the STL implementation
     * @return The kind of STL implementation.
     */
    inline constexpr StlKind get_stl() {
#if defined(YYCC_STL_MSSTL)
        return StlKind::MSSTL;
#elif defined(YYCC_STL_GNUSTL)
        return StlKind::GNUSTL;
#else
        return StlKind::CLANGSTL;
#endif
    }

} // namespace yycc::macro::stl
