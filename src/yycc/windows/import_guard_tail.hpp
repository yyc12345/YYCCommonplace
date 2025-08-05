// It is by design that no pragma once or #if to prevent deplicated including.
// Because this header is the part of wrapper, not a real header.
// #pragma once

#include "../macro/os_detector.hpp"

#if defined(YYCC_OS_WINDOWS)

// Windows also will generate following macros which may cause 
// the function sign is different in Windows and other platforms.
// So we simply remove them.
// At the same time, because `#undef` will not throw error if there are no matched macro,
// we can simply use `#undef` to remove them directly.
#undef GetObject
#undef GetClassName
#undef LoadImage
#undef GetTempPath
#undef GetModuleFileName
#undef CopyFile
#undef MoveFile
#undef DeleteFile

#endif
