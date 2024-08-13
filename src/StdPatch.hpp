#pragma once
#include "YYCCInternal.hpp"

#include <filesystem>
#include <string>
#include <string_view>

/**
 * @brief \c Standard library related patches for UTF8 compatibility and the limitation of C++ standard version.
 * @details
 * See also \ref std_patch.
*/
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

#pragma region StartsWith EndsWith

	// Reference:
	// https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with
	// https://en.cppreference.com/w/cpp/string/basic_string_view/ends_with
	// https://en.cppreference.com/w/cpp/string/basic_string/starts_with
	// https://en.cppreference.com/w/cpp/string/basic_string/ends_with
	
#pragma region String View

	/**
	 * @brief Checks if the string view begins with the given prefix
	 * @param[in] that The string view to find.
	 * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string_view<CharT, Traits>& that, std::basic_string_view<CharT, Traits> sv) noexcept {
		return std::basic_string_view<CharT, Traits>(that.data(), std::min(that.size(), sv.size())) == sv;
	}
	/**
	 * @brief Checks if the string view begins with the given prefix
	 * @param[in] that The string view to find.
	 * @param[in] ch A single character.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string_view<CharT, Traits>& that, CharT ch) noexcept {
		return !that.empty() && Traits::eq(that.front(), ch);
	}
	/**
	 * @brief Checks if the string view begins with the given prefix
	 * @param[in] that The string view to find.
	 * @param[in] s A null-terminated character string.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string_view<CharT, Traits>& that, const CharT* s) noexcept {
		return StartsWith(that, std::basic_string_view(s));
	}
	
	/**
	 * @brief Checks if the string view ends with the given suffix
	 * @param[in] that The string view to find.
	 * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string_view<CharT, Traits>& that, std::basic_string_view<CharT, Traits> sv) noexcept {
		return that.size() >= sv.size() && that.compare(that.size() - sv.size(), std::basic_string_view<CharT, Traits>::npos, sv);
	}
	/**
	 * @brief Checks if the string view ends with the given suffix
	 * @param[in] that The string view to find.
	 * @param[in] ch A single character.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string_view<CharT, Traits>& that, CharT ch) noexcept {
		return !that.empty() && Traits::eq(that.back(), ch);
	}
	/**
	 * @brief Checks if the string view ends with the given suffix
	 * @param[in] that The string view to find.
	 * @param[in] s A null-terminated character string.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string_view<CharT, Traits>& that, const CharT* s) noexcept {
		return EndsWith(that, std::basic_string_view(s));
	}
	
#pragma endregion

#pragma region String

	/**
	 * @brief Checks if the string begins with the given prefix
	 * @param[in] that The string to find.
	 * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string<CharT, Traits>& that, std::basic_string_view<CharT, Traits> sv) noexcept {
		return StartsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), sv);
	}
	/**
	 * @brief Checks if the string begins with the given prefix
	 * @param[in] that The string to find.
	 * @param[in] ch A single character.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string<CharT, Traits>& that, CharT ch) noexcept {
		return StartsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), ch);
	}
	/**
	 * @brief Checks if the string begins with the given prefix
	 * @param[in] that The string to find.
	 * @param[in] s A null-terminated character string.
	 * @return True if the string view begins with the provided prefix, false otherwise.
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool StartsWith(const std::basic_string<CharT, Traits>& that, const CharT* s) noexcept {
		return StartsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), s);
	}
	
	/**
	 * @brief Checks if the string ends with the given suffix
	 * @param[in] that The string to find.
	 * @param[in] sv A string view which may be a result of implicit conversion from \c std::basic_string.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string<CharT, Traits>& that, std::basic_string_view<CharT, Traits> sv) noexcept {
		return EndsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), sv);
	}
	/**
	 * @brief Checks if the string ends with the given suffix
	 * @param[in] that The string to find.
	 * @param[in] ch A single character.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string<CharT, Traits>& that, CharT ch) noexcept {
		return EndsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), ch);
	}
	/**
	 * @brief Checks if the string ends with the given suffix
	 * @param[in] that The string to find.
	 * @param[in] s A null-terminated character string.
	 * @return True if the string view ends with the provided suffix, false otherwise. 
	*/
	template<class CharT, class Traits = std::char_traits<CharT>>
	bool EndsWith(const std::basic_string<CharT, Traits>& that, const CharT* s) noexcept {
		return EndsWith(std::basic_string_view<CharT, Traits>(that.data(), that.size()), s);
	}
	
#pragma endregion

#pragma endregion

#pragma region Contain

	/**
	 * @brief Checks if there is an element with key equivalent to key in the container.
	 * @details 
	 * The polyfill to \c Contains function of unordered and ordered associative container.
	 * Because this function only present after C++ 20.
	 * This function will use our custom polyfill if the version of C++ standard you are using lower than C++ 20.
	 * Otherwise it will fallback to vanilla standard library function.
	 * @tparam _TContainer 
	 * The type of container. This container must have \c find() and \c end() member functions.
	 * @tparam _TKey 
	 * The type of key of container.
	 * If the container is a set, this type is the type of item in set.
	 * If the container is a map, this type is the key type of map.
	 * @param[in] container The reference to container to find.
	 * @param[in] key Key value of the element to search for 
	 * @return True if there is such an element, otherwise false.
	 * @remarks
	 * This template function do not have constraint check.
	 * If container type has \c find() and \c end() member functions, this template function will be created without any error.
	 * However, this function should be used for standard library associative container according to its original purpose.
	 * It means that the type of container usually and should be one of following types:
	 * \li \c std::set
	 * \li \c std::multiset
	 * \li \c std::map
	 * \li \c std::multimap
	 * \li \c std::unordered_set
	 * \li \c std::unordered_multiset
	 * \li \c std::unordered_map
	 * \li \c std::unordered_multimap
	*/
	template<class _TContainer, class _TKey>
	bool Contains(const _TContainer& container, const _TKey& key) {
#if __cplusplus < 202002L
		return container.find(key) != container.end();
#else
		return container.contains(key);
#endif
	}

#pragma endregion

}
