#pragma once
#include "YYCCInternal.hpp"

#include <filesystem>
#include <string>
#include <string_view>

namespace YYCC::StdPatch {
	
	/**
	 * @brief Constructs \c std::filesystem::path from UTF8 path.
	 * @param[in] u8_path UTF8 path string for building.
	 * @return \c std::filesystem::path instance.
	 * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
	*/
	std::filesystem::path ToStdPath(const yycc_u8string_view& u8_path);

	/**
	 * @brief Returns the UTF8 representation of given \c std::filesystem::path.
	 * @param[in] path The \c std::filesystem::path instance converting to UTF8 path.
	 * @return The UTF8 representation of given \c std::filesystem::path.
	 * @exception std::invalid_argument Fail to convert to UTF8 string.
	*/
	yycc_u8string ToUTF8Path(const std::filesystem::path& path);

}
