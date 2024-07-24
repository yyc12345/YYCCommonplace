#pragma once
#include "YYCCInternal.hpp"

#include <filesystem>

/**
 * @brief \c std::filesystem::path related patches for UTF8 compatibility
 * @details
 * See also \ref fs_path_patch.
*/
namespace YYCC::FsPathPatch {
	
	/**
	 * @brief Constructs \c std::filesystem::path from UTF8 path.
	 * @param[in] u8_path UTF8 path string for building.
	 * @return \c std::filesystem::path instance.
	 * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
	*/
	std::filesystem::path FromUTF8Path(const yycc_char8_t* u8_path);

	/**
	 * @brief Returns the UTF8 representation of given \c std::filesystem::path.
	 * @param[in] path The \c std::filesystem::path instance converting to UTF8 path.
	 * @return The UTF8 representation of given \c std::filesystem::path.
	 * @exception std::invalid_argument Fail to convert to UTF8 string.
	*/
	yycc_u8string ToUTF8Path(const std::filesystem::path& path);

}
