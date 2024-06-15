#pragma once
#include "YYCCInternal.hpp"

#include <filesystem>

/**
 * @brief The patch namespace resolving \c std::filesystem::path encoding issue.
 * @details
 * This patch is Windows oriented.
 * If you are in Windows, this patch will perform extra operations to achieve goals,
 * and in other platforms, they just redirect request to corresponding vanilla C++ functions.
 * 
 * As you know, the underlying char type of \c std::filesystem::path is \c wchar_t on Windows,
 * and in other platforms, it is simple \c char.
 * Due to this, if you passing UTF8 char sequence to \c std::filesystem::path on Windows,
 * the library implementation will assume your input is based on current Windows code page, not UTF8.
 * And the final path stored in \c std::filesystem::path is not what you expcected.
 * 
 * This patch namespace always use UTF8 as its argument. There is no ambiguous issue.
 * You should use the functions provided by this namespace on any platforms 
 * instead of vanilla \c std::filesystem::path functions.
*/
namespace YYCC::FsPathPatch {
	
	/**
	 * @brief Constructs the path from a UTF8 character sequence
	 * @param[in] u8_path UTF8 path string for building this std::filesystem::path.
	 * @return std::filesystem::path instance.
	 * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
	*/
	std::filesystem::path FromUTF8Path(const char* u8_path);

	/**
	 * @brief Returns the UTF8 representation of the pathname
	 * @param path[in] The string to be output.
	 * @return UTF8 encoded string representing given path.
	 * @exception std::invalid_argument Fail to parse to UTF8 string.
	*/
	std::string ToUTF8Path(const std::filesystem::path& path);

}
