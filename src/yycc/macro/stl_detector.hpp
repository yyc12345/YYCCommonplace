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
