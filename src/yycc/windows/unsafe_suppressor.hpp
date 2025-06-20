#pragma once
#include "../macro/os_detector.hpp"

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
