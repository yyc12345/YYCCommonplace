#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <cstdio>

namespace YYCC::IOHelper {
	
	/**
	 * @brief Set given FILE* as UTF8 mode.
	 * @param fs[in] The FILE* need to be set as UTF8 mode.
	 * @return True if success, otherwise false.
	 * @warning Once this function success, you can not use any narrow char function on this FILE*,
	 * such as std::fputs, std::fprintf and etc. You only can use wide char function on it,
	 * or use the functions provided in this namespace by providing UTF8 string as their argument.
	*/
	bool futf8(FILE* fs);

	FILE* fopen(const char* u8_filepath, const char* u8_mode);

}

#endif
