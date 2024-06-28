#include "FsPathPatch.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <stdexcept>

namespace YYCC::FsPathPatch {

	std::filesystem::path FromUTF8Path(const yycc_char8_t* u8_path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// convert path to wchar
		std::wstring wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_path, wpath))
			throw std::invalid_argument("Fail to convert given UTF8 string.");

		// return path with wchar_t ctor
		return std::filesystem::path(wpath);
		
#else
		return std::filesystem::path(EncodingHelper::ToNative(u8_path));
#endif
	}

	yycc_u8string ToUTF8Path(const std::filesystem::path& path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// get and convert to utf8
		yycc_u8string u8_path;
		if (!YYCC::EncodingHelper::WcharToUTF8(path.c_str(), u8_path))
			throw std::invalid_argument("Fail to convert to UTF8 string.");

		// return utf8 path
		return u8_path;

#else
		return EncodingHelper::ToUTF8(path.string());
#endif
	}

}
