#pragma once
#include "YYCCInternal.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS

// Windows also will generate following macros
// which may cause the function sign is different in Windows and other platforms.
// So we simply remove them.
#undef GetObject
#undef GetClassName
#undef LoadImage
#undef GetTempPath

#endif