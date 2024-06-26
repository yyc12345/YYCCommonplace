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
// Also define an universal macro to create UTF8 string literal.
// And do a polyfill if no embedded char8_t type.
#include <string>
namespace YYCC {
#if defined(__cpp_char8_t)
	using yycc_char8_t = char8_t;
	using yycc_u8string = std::u8string;

#define _YYCC_U8(strl) u8 ## strl
#define YYCC_U8(strl) (_YYCC_U8(strl))
#else
	using yycc_char8_t = unsigned char;
	using yycc_u8string = std::basic_string<yycc_char8_t>;

#define _YYCC_U8(strl) u8 ## strl
#define YYCC_U8(strl) (reinterpret_cast<const yycc_char8_t*>(_YYCC_U8(strl)))
#endif
}

