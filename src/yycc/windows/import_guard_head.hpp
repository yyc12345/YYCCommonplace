// It is by design that no pragma once or #if to prevent deplicated including.
// Because this header is the part of wrapper, not a real header.
// #pragma once

#include "../macro/os_detector.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS

// Define 2 macros to disallow Windows generate MIN and MAX macros
// which cause std::min and std::max can not function as normal.
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#endif
