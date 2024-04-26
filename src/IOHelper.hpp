#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <string>

namespace YYCC::IOHelper {

	void Gets(std::string& u8cmd);
	FILE* FOpen(const char* u8_filepath, const char* u8_mode);

}

#endif
