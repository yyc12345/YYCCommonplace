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

// Decide the char type we used
#include <string>
namespace YYCC {
#if defined(__cpp_char8_t)
	using u8char = char8_t;
	using u8string = std::u8string
#else
	using u8char = char;
	using u8string = std::string;
#endif
}
