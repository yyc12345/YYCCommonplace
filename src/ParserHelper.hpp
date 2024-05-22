#pragma once
#include <string>
#include <cinttypes>

namespace YYCC::ParserHelper {

	bool Parse(const std::string& strl, int& ret, int base = 10);
	int Parse(const std::string& strl, int base = 10);

}
