#pragma once
#include "../string.hpp"
#include <filesystem>

#define NS_YYCC_STRING ::yycc::string

/**
 * @brief \c Standard library related patches for UTF8 compatibility and the limitation of C++ standard version.
 * @details
 * See also \ref std_patch.
*/
namespace yycc::patch::path {

    /**
     * @brief Constructs \c std::filesystem::path from UTF8 path.
     * @param[in] u8_path UTF8 path string for building.
     * @return \c std::filesystem::path instance.
     * @exception std::invalid_argument Fail to parse given UTF8 string (maybe invalid?).
    */
    std::filesystem::path to_std_path(const NS_YYCC_STRING::u8string_view& u8_path);

    /**
     * @brief Returns the UTF8 representation of given \c std::filesystem::path.
     * @param[in] path The \c std::filesystem::path instance converting to UTF8 path.
     * @return The UTF8 representation of given \c std::filesystem::path.
     * @exception std::invalid_argument Fail to convert to UTF8 string.
    */
    NS_YYCC_STRING::u8string to_u8string(const std::filesystem::path& path);

} // namespace yycc::patch::path

#undef NS_YYCC_STRING
