#pragma once
#include "YYCCInternal.hpp"

#include <filesystem>

namespace YYCC::FsPathPatch {
	
	/**
	 * @brief Constructs the path from a UTF8 character sequence
	 * @param[in] u8_path UTF8 path string for building this std::filesystem::path.
	 * @return std::filesystem::path instance.
	 * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
	*/
	std::filesystem::path FromUTF8Path(const yycc_char8_t* u8_path);

	/**
	 * @brief Returns the UTF8 representation of the pathname
	 * @param path[in] The string to be output.
	 * @return UTF8 encoded string representing given path.
	 * @exception std::invalid_argument Fail to parse to UTF8 string.
	*/
	yycc_u8string ToUTF8Path(const std::filesystem::path& path);

}
