#pragma once
#include "../macro/os_detector.hpp"
#include "../macro/stl_detector.hpp"
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include <expected>

/**
 * @brief The namespace provide patches for Windows console.
 */
namespace yycc::windows::console {

    /// @brief Error occurs in this module.
    enum class ExecError {
        NotTty,  ///< Given stream is not TTY.
        GetMode, ///< Can not get stream mode.
        SetMode, ///< Can not set stream mode.
    };

    /// @brief Result type used in this module.
    template<typename T>
    using ExecResult = std::expected<T, ExecError>;

    /**
	 * @brief Enable console color support for Windows.
	 * @details This actually is enable virtual console feature for \c stdout and \c stderr.
	 * @return Nothing or error occurs.
	*/
    ExecResult<void> colorful_console();

}

#endif
