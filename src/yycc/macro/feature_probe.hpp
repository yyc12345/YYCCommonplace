#pragma once

// Hint for C++ feature detection:
// __cplusplus macro need special compiler switch enabled when compiling.
// So we use _MSVC_LANG check it instead.

// Detect C++ 20
#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
    #define YYCC_CPPFEAT_GE_CPP20
#endif

// Detect C++ 23
#if __cplusplus >= 202302L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202302L)
    #define YYCC_CPPFEAT_GE_CPP23
#endif

// Check whether there is support of `contains` for `set` and `map` including their varients.
#if defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPFEAT_CONTAINS
#endif

// Check whether there is support of `starts_with` and `ends_with` for `basic_string`.
#if defined(__cpp_lib_starts_ends_with) || defined(YYCC_CPPFEAT_GE_CPP20)
    #define YYCC_CPPTEST_STARTS_ENDS_WITH
#endif

// Check whether there is support of `std::expected`.
#if defined(__cpp_lib_expected) || defined(YYCC_CPPFEAT_GE_CPP23)
    #define YYCC_CPPTEST_EXPECTED
#endif
