#include "StdPatch.hpp"

#include "EncodingHelper.hpp"
#include <string>
#include <stdexcept>

namespace YYCC::StdPatch {

	std::filesystem::path ToStdPath(const yycc_u8string_view& u8_path) {
#if YYCC_OS == YYCC_OS_WINDOWS

		// convert path to wchar
		std::wstring wpath;
		if (!YYCC::EncodingHelper::UTF8ToWchar(u8_path, wpath))
			throw std::invalid_argument("Fail to convert given UTF8 string.");
		// return path with wchar_t ctor
		return std::filesystem::path(wpath);
		
#else
		std::string cache = YYCC::EncodingHelper::ToOrdinary(u8_path);
		return std::filesystem::path(cache.c_str());
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
