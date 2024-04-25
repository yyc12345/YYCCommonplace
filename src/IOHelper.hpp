#pragma once
#include <string>

namespace YYCC::IOHelper {

	void GetCmdLine(std::string&);
	FILE* UTF8FOpen(const char* u8_filepath, const char* u8_mode);
}
