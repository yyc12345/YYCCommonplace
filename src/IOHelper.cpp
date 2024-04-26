#include "IOHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "EncodingHelper.hpp"
#include <cstdio>
#include <iostream>

namespace YYCC::IOHelper {

	void Gets(std::string& u8cmd) {
		std::wstring wcmd;
		std::getline(std::wcin, wcmd);
		YYCC::EncodingHelper::WcharToChar(wcmd, u8cmd, CP_UTF8);
	}

	FILE* FOpen(const char* u8_filepath, const char* u8_mode) {
		std::wstring wmode, wpath;
		bool suc = YYCC::EncodingHelper::CharToWchar(u8_mode, wmode, CP_UTF8);
		suc = suc && YYCC::EncodingHelper::CharToWchar(u8_filepath, wpath, CP_UTF8);

		if (suc) {
			return _wfopen(wpath.c_str(), wmode.c_str());
		} else {
			// fallback
			return std::fopen(u8_filepath, u8_mode);
		}
	}

}

#endif
