#pragma once

// Hint for C++ feature detection:
// __cplusplus macro need special compiler switch enabled when compiling.
// So we use _MSVC_LANG check it instead.

// ===== C++ Version =====

// Detect C++ 20
#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #define YYCC_CPPFEAT_GE_CPP20
#endif

// Detect C++ 23
#if __cplusplus >= 202302L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202302L)
    #define YYCC_CPPFEAT_GE_CPP23
#endif

// ===== C++ Features =====

// Check whether there is support of UTF8 string system.
#if defined(__cpp_char8_t) || defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_UTF8
#endif

// Check whether there is support of `contains` for `set` and `map` including their varients.
#if defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_CONTAINS
#endif

// Check whether there is support of `starts_with` and `ends_with` for `basic_string`.
#if defined(__cpp_lib_starts_ends_with) || defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_STARTS_ENDS_WITH
#endif

// Check whether there is support of `std::expected`.
#if defined(__cpp_lib_expected) || defined(YYCC_CPPFEAT_GE_CPP23)
    #define YYCC_CPPFEAT_EXPECTED
#endif

// Check whether there is support of `std::format`.
#if defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_FORMAT
#endif

// Check whether there is support of `__VA_OPT__`
#if defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_VA_OPT
#endif

// Check whether there is support of `std::stacktrace` and its formatter.
#if (defined(__cpp_lib_starts_ends_with) && defined(__cpp_lib_formatters)) || defined(YYCC_CPPFEAT_GE_CPP23)
    #define YYCC_CPPFEAT_STACKTRACE
#endif
