#include "FsPathPatch.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <stdexcept>

namespace YYCC::FsPathPatch {

	std::filesystem::path FromUTF8Path(const char* u8_path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// convert path to wchar
		std::wstring wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_path, wpath))
			throw std::invalid_argument("Fail to convert given UTF8 string.");

		// call microsoft specified fopen which support wchar as argument.
		return std::filesystem::path(wpath);

#else
		return std::filesystem::path(u8_path);
#endif
	}

	std::string ToUTF8Path(const std::filesystem::path& path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// get and convert to utf8
		std::string u8_path;
		if (!YYCC::EncodingHelper::WcharToUTF8(path.c_str(), u8_path))
			throw std::invalid_argument("Fail to convert to UTF8 string.");

		// return utf8 path
		return u8_path;

#else
		return path.string();
#endif
	}

}
