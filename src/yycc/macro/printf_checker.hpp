#pragma once
#include "compiler_detector.hpp"
#include "stl_detector.hpp"

// YYC MARK:
// This code is copied from Qt project.

#if defined(YYCC_CC_GCC)
// GCC has its special attribute
#define YYCC_PRINTF_CHECK_ATTR(A, B) __attribute__((format(gnu_printf, (A), (B))))
#elif defined(YYCC_CC_CLANG)
// Clang use its own attribute
#define YYCC_PRINTF_CHECK_ATTR(A, B) __attribute__((format(printf, (A), (B))))
#else
// Other CC do not support this (like MSVC), skip it
#define YYCC_PRINTF_CHECK_ATTR(A, B)
#endif

#if defined(YYCC_STL_MSSTL)
// On Microsoft STL, we can use some mechanisms to check it.
#include "../windows/import_guard_head.hpp"
#include <sal.h>
#include "../windows/import_guard_tail.hpp"
#define YYCC_PRINTF_CHECK_FMTSTR _Printf_format_string_
#else
// Other STL do not have this.
#define YYCC_PRINTF_CHECK_FMTSTR
#endif
