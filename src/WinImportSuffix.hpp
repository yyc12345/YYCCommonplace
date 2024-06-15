// It is by design that no pragma once or #if to prevent deplicated including.
// Because this header is the part of wrapper, not a real header.
// #pragma once

#include "YYCCInternal.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS

// Windows also will generate following macros
// which may cause the function sign is different in Windows and other platforms.
// So we simply remove them.
// Because #undef will not throw error if there are no matched macro,
// so we simply #undef them directly.
#undef GetObject
#undef GetClassName
#undef LoadImage
#undef GetTempPath
#undef GetModuleFileName

#endif