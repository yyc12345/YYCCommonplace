#pragma once

// Define operating system macros
#define YYCC_OS_WINDOWS 2
#define YYCC_OS_LINUX 3
// Check current operating system.
#if defined(_WIN32)
#define YYCC_OS YYCC_OS_WINDOWS
#else
#define YYCC_OS YYCC_OS_LINUX
#endif

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

// Define the UTF8 char type we used.
// And do a polyfill if no embedded char8_t type.
#include <string>
#include <string_view>
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
	/**
		\typedef yycc_char8_t
		\brief YYCC UTF8 char type.
		\details
		This char type is an alias to \c std::char8_t if your current C++ standard support it.
		Otherwise it is defined as <TT>unsigned char</TT> as C++ 20 stdandard does.
	*/
	/**
		\typedef yycc_u8string
		\brief YYCC UTF8 string container type.
		\details
		This type is defined as \c std::basic_string<yycc_char8_t>.
		It is equal to \c std::u8string if your current C++ standard support it.
	*/
	/**
		\typedef yycc_u8string_view
		\brief YYCC UTF8 string view type.
		\details
		This type is defined as \c std::basic_string_view<yycc_char8_t>.
		It is equal to \c std::u8string_view if your current C++ standard support it.
	*/


}

