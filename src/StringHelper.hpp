#pragma once
#include <string>
#include <cstdarg>

namespace YYCC::StringHelper {
	
	bool Printf(std::string& strl, const char* format, ...);
	bool VPrintf(std::string& strl, const char* format, va_list argptr);

	std::string Printf(const char* format, ...);
	std::string VPrintf(const char* format, va_list argptr);

}
