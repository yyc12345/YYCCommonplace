#pragma once
#include "YYCCInternal.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS

// Define 2 macros to disallow Windows generate MIN and MAX macros
// which cause std::min and std::max can not function as normal.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#endif