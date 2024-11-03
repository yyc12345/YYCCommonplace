#pragma once

#pragma region Library Version and Comparison Macros

#define YYCC_VER_MAJOR 1
#define YYCC_VER_MINOR 3
#define YYCC_VER_PATCH 0

/// @brief Return true if left version number is equal to right version number, otherwise false.
#define YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3) ((av1) == (bv1) && (av2) == (bv2) && (av3) == (bv3))
/// @brief Return true if left version number is not equal to right version number, otherwise false.
#define YYCC_VERCMP_NE(av1, av2, av3, bv1, bv2, bv3) (!YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is greater than right version number, otherwise false.
#define YYCC_VERCMP_G(av1, av2, av3, bv1, bv2, bv3) ( \
	((av1) > (bv1)) || \
	((av1) == (bv1) && (av2) > (bv2)) || \
	((av1) == (bv1) && (av2) == (bv2) && (av3) > (bv3)) \
)
/// @brief Return true if left version number is greater than or equal to right version number, otherwise false.
#define YYCC_VERCMP_GE(av1, av2, av3, bv1, bv2, bv3) (YYCC_VERCMP_G(av1, av2, av3, bv1, bv2, bv3) || YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is not lower than right version number, otherwise false.
#define YYCC_VERCMP_NL(av1, av2, av3, bv1, bv2, bv3) YYCC_VERCMP_GE(av1, av2, av3, bv1, bv2, bv3)
/// @brief Return true if left version number is lower than right version number, otherwise false.
#define YYCC_VERCMP_L(av1, av2, av3, bv1, bv2, bv3) ( \
	((av1) < (bv1)) || \
	((av1) == (bv1) && (av2) < (bv2)) || \
	((av1) == (bv1) && (av2) == (bv2) && (av3) < (bv3)) \
)
/// @brief Return true if left version number is lower than or equal to right version number, otherwise false.
#define YYCC_VERCMP_LE(av1, av2, av3, bv1, bv2, bv3) (YYCC_VERCMP_L(av1, av2, av3, bv1, bv2, bv3) || YYCC_VERCMP_E(av1, av2, av3, bv1, bv2, bv3))
/// @brief Return true if left version number is not greater than right version number, otherwise false.
#define YYCC_VERCMP_NG(av1, av2, av3, bv1, bv2, bv3) YYCC_VERCMP_LE(av1, av2, av3, bv1, bv2, bv3)

#pragma endregion

#pragma region Operating System Identifier Macros

// Define operating system macros
#define YYCC_OS_WINDOWS 2
#define YYCC_OS_LINUX 3
// Check current operating system.
#if defined(_WIN32)
#define YYCC_OS YYCC_OS_WINDOWS
#else
#define YYCC_OS YYCC_OS_LINUX
#endif

#pragma endregion

#pragma region Windows Shitty Behavior Disable Macros

// If we are in Windows,
// we need add 2 macros to disable Windows shitty warnings and errors of
// depracted functions and not secure functions.
#if YYCC_OS == YYCC_OS_WINDOWS

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#if !defined(_CRT_NONSTDC_NO_DEPRECATE)
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#endif

#pragma endregion

#pragma region YYCC UTF8 Types

// Define the UTF8 char type we used.
// And do a polyfill if no embedded char8_t type.

#include <string>
#include <string_view>

/**
 * @brief Library core namespace
 * @details Almost library functions are located in this namespace.
*/
namespace YYCC {
#if defined(__cpp_char8_t)
	using yycc_char8_t = char8_t;
	using yycc_u8string = std::u8string;
	using yycc_u8string_view = std::u8string_view;
#else
	using yycc_char8_t = unsigned char;
	using yycc_u8string = std::basic_string<yycc_char8_t>;
	using yycc_u8string_view = std::basic_string_view<yycc_char8_t>;
#endif
}
/**
	\typedef YYCC::yycc_char8_t
	\brief YYCC UTF8 char type.
	\details
	This char type is an alias to \c std::char8_t if your current C++ standard support it.
	Otherwise it is defined as <TT>unsigned char</TT> as C++ 20 stdandard does.
*/
/**
	\typedef YYCC::yycc_u8string
	\brief YYCC UTF8 string container type.
	\details
	This type is defined as \c std::basic_string<yycc_char8_t>.
	It is equal to \c std::u8string if your current C++ standard support it.
*/
/**
	\typedef YYCC::yycc_u8string_view
	\brief YYCC UTF8 string view type.
	\details
	This type is defined as \c std::basic_string_view<yycc_char8_t>.
	It is equal to \c std::u8string_view if your current C++ standard support it.
*/

#pragma endregion

#pragma region Batch Class Move / Copy Function Macros

	/// @brief Explicitly remove copy (\c constructor and \c operator\=) for given class.
#define YYCC_DEL_CLS_COPY(CLSNAME) \
	CLSNAME(const CLSNAME&) = delete; \
    CLSNAME& operator=(const CLSNAME&) = delete;

	/// @brief Explicitly remove move (\c constructor and \c operator\=) for given class.
#define YYCC_DEL_CLS_MOVE(CLSNAME) \
	CLSNAME(CLSNAME&&) = delete; \
    CLSNAME& operator=(CLSNAME&&) = delete;

	/// @brief Explicitly remove (copy and move) (\c constructor and \c operator\=) for given class.
#define YYCC_DEL_CLS_COPY_MOVE(CLSNAME) \
	YYCC_DEL_CLS_COPY(CLSNAME) \
	YYCC_DEL_CLS_MOVE(CLSNAME)

	/// @brief Explicitly set default copy (\c constructor and \c operator\=) for given class.
#define YYCC_DEF_CLS_COPY(CLSNAME) \
	CLSNAME(const CLSNAME&) = default; \
    CLSNAME& operator=(const CLSNAME&) = default;

	/// @brief Explicitly set default move (\c constructor and \c operator\=) for given class.
#define YYCC_DEF_CLS_MOVE(CLSNAME) \
	CLSNAME(CLSNAME&&) = default; \
    CLSNAME& operator=(CLSNAME&&) = default;

	/// @brief Explicitly set default (copy and move) (\c constructor and \c operator\=) for given class.
#define YYCC_DEF_CLS_COPY_MOVE(CLSNAME) \
	YYCC_DEF_CLS_COPY(CLSNAME) \
	YYCC_DEF_CLS_MOVE(CLSNAME)

#pragma endregion


