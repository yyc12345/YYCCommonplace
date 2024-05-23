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

//// Decide the char type we used
//#include <string>
//namespace YYCC {
//#if defined(__cpp_char8_t)
//	using u8char = char8_t;
//	using u8string = std::std::string
//#else
//	using u8char = char;
//	using u8string = std::string;
//#endif
//}
