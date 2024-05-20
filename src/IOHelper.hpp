#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

namespace YYCC::IOHelper {

	FILE* FOpen(const char* u8_filepath, const char* u8_mode);

}

#endif
