#include "StringHelper.hpp"

namespace YYCC::StringHelper {

	bool Printf(std::string& strl, const char* format, ...) {
		va_list argptr;
		va_start(argptr, format);
		bool ret = VPrintf(strl, format, argptr);
		va_end(argptr);
		return ret;
	}

	bool VPrintf(std::string& strl, const char* format, va_list argptr) {
		va_list args1;
		va_copy(args1, argptr);
		va_list args2;
		va_copy(args2, argptr);

		// the return value is desired char count without NULL terminal.
		// minus number means error
		int count = std::vsnprintf(nullptr, 0, format, args1);
		if (count < 0) {
			// invalid length returned by vsnprintf.
			return false;
		}
		va_end(args1);

		// resize std::string to desired count.
		// and pass its length + 1 to std::vsnprintf,
		// because std::vsnprintf only can write "buf_size - 1" chars with a trailing NULL.
		// however std::vsnprintf already have a trailing NULL, so we plus 1 for it.
		strl.resize(count);
		int write_result = std::vsnprintf(strl.data(), strl.size() + 1, format, args2);
		va_end(args2);
		
		if (write_result < 0 || write_result > count) {
			// invalid write result in vsnprintf.
			return false;
		}

		return true;
	}

	
	std::string Printf(const char* format, ...) {
		std::string ret;
		
		va_list argptr;
		va_start(argptr, format);
		VPrintf(ret, format, argptr);
		va_end(argptr);

		return ret;
	}

	std::string VPrintf(const char* format, va_list argptr) {
		std::string ret;

		va_list argcpy;
		va_copy(argcpy, argptr);
		VPrintf(ret, format, argcpy);
		va_end(argcpy);

		return ret;
	}

}
