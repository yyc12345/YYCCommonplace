#pragma once
#include <string>
#include <string_view>

/**
 * @brief Provides utilities for reinterpretation between UTF-8 and ordinary string types.
 * @details
 * Please note that there is no encoding convertion happended in this namespace provided functions.
 * They just simply reinterpret one string to another string.
 * The validation of UTF8 string is guaranteed by user self.
 */
namespace yycc::string::reinterpret {

    /**
     * @brief Reinterpret ordinary char type to UTF-8 char type.
     * @param[in] src Source ordinary char value.
     * @return UTF8 char value.
     */
    char8_t as_utf8(const char& src);
	/**
	 * @brief Reinterpret ordinary C-string to UTF-8 string (const version).
	 * @param[in] src Source ordinary string
	 * @return Pointer to UTF-8 encoded string
	 */
    const char8_t* as_utf8(const char* src);
	/**
	 * @brief Reinterpret ordinary C-string as an UTF-8 string (non-const version).
	 * @param[in] src Source ordinary string
	 * @return Pointer to UTF-8 encoded string
	 */
    char8_t* as_utf8(char* src);
	/**
	 * @brief Reinterpret ordinary string view to copied UTF-8 string.
	 * @param[in] src Source ordinary string view
	 * @return UTF-8 encoded string
	 */
    std::u8string as_utf8(const std::string_view& src);
	/**
	 * @brief Reinterpret ordinary string view to UTF-8 string view.
	 * @param[in] src Source ordinary string view
	 * @return UTF-8 encoded string view
	 */
    std::u8string_view as_utf8_view(const std::string_view& src);

    /**
     * @brief Reinterpret UTF-8 char type to ordinary char type.
     * @param[in] src Source UTF-8 char value.
     * @return Ordinary char value.
     */
    char as_ordinary(char8_t src);
	/**
	 * @brief Reinterpret UTF-8 C-string to ordinary string (const version).
	 * @param[in] src Source UTF-8 string
	 * @return Pointer to ordinary string
	 */
    const char* as_ordinary(const char8_t* src);
	/**
	 * @brief Reinterpret UTF-8 C-string to ordinary string (non-const version).
	 * @param[in] src Source UTF-8 string
	 * @return Pointer to ordinary string
	 */
    char* as_ordinary(char8_t* src);
	/**
	 * @brief Reinterpret UTF-8 string view to ordinary string.
	 * @param[in] src Source UTF-8 string view
	 * @return Ordinary string
	 */
    std::string as_ordinary(const std::u8string_view& src);
	/**
	 * @brief Reinterpret UTF-8 string view to ordinary string view
	 * @param[in] src Source UTF-8 string view
	 * @return Ordinary string view
	 */
    std::string_view as_ordinary_view(const std::u8string_view& src);

}
