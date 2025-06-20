#pragma once
#include <cstdarg>
#include <functional>
#include <vector>
#include "../string.hpp"

#define NS_YYCC_STRING ::yycc::string

namespace yycc::string::op {

	/**
	 * @brief Perform a string formatting operation.
	 * @param[out] strl 
	 * The string container receiving the result.
	 * There is no guarantee that the content is not modified when function failed.
	 * @param[in] format The format string.
	 * @param[in] ... Argument list of format string.
	 * @return True if success, otherwise false.
	*/
	bool printf(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8char* format, ...);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[out] strl 
	 * The string container receiving the result.
	 * There is no guarantee that the content is not modified when function failed.
	 * @param[in] format The format string.
	 * @param[in] argptr Argument list of format string.
	 * @return True if success, otherwise false.
	*/
	bool vprintf(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8char* format, va_list argptr);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[in] format The format string.
	 * @param[in] ... Argument list of format string.
	 * @return The formatting result. Empty string if error happened.
	*/
	NS_YYCC_STRING::u8string printf(const NS_YYCC_STRING::u8char* format, ...);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[in] format The format string.
	 * @param[in] argptr Argument list of format string.
	 * @return The formatting result. Empty string if error happened.
	*/
	NS_YYCC_STRING::u8string vprintf(const NS_YYCC_STRING::u8char* format, va_list argptr);

	/**
	 * @brief Modify given string with all occurrences of substring \e old replaced by \e new.
	 * @param[in,out] strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	*/
	void replace(NS_YYCC_STRING::u8string& strl, const NS_YYCC_STRING::u8string_view& _from_strl, const NS_YYCC_STRING::u8string_view& _to_strl);
	/**
	 * @brief Return a copy with all occurrences of substring \e old replaced by \e new.
	 * @param[in] _strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	 * @return The result of replacement.
	*/
	NS_YYCC_STRING::u8string replace(const NS_YYCC_STRING::u8string_view& _strl, const NS_YYCC_STRING::u8string_view& _from_strl, const NS_YYCC_STRING::u8string_view& _to_strl);

	/**
	 * @brief The data provider of general join function.
	 * @details
	 * For programmer using lambda to implement this function pointer:
	 * \li During calling, implementation should assign the reference of string view passed in argument
	 * to the string which need to be joined.
	 * \li Function return true to continue joining. otherwise return false to stop joining. 
	 * The argument content assigned in the calling returning false is not included in join process.
	*/
	using JoinDataProvider = std::function<bool(NS_YYCC_STRING::u8string_view&)>;
	/**
	 * @brief Universal join function.
	 * @details
	 * This function use function pointer as a general data provider interface,
	 * so this function suit for all types container.
	 * You can use this universal join function for any custom container by
	 * using C++ lambda syntax to create a code block adapted to this function pointer.
	 * @param[in] fct_data The function pointer in JoinDataProvider type prividing the data to be joined.
	 * @param[in] decilmer The decilmer used for joining.
	 * @return The result string of joining.
	*/
	NS_YYCC_STRING::u8string join(JoinDataProvider fct_data, const NS_YYCC_STRING::u8string_view& decilmer);
	/**
	 * @brief Specialized join function for standard library container.
	 * @tparam InputIt 
	 * Must meet the requirements of LegacyInputIterator.
	 * It also can be dereferenced and then implicitly converted to NS_YYCC_STRING::u8string_view.
	 * @param[in] first The beginning of the range of elements to join.
	 * @param[in] last The terminal of the range of elements to join (exclusive).
	 * @param[in] decilmer The decilmer used for joining.
	 * @return The result string of joining.
	*/
	template<class InputIt>
	NS_YYCC_STRING::u8string join(InputIt first, InputIt last, const NS_YYCC_STRING::u8string_view& decilmer) {
		return join([&first, &last](NS_YYCC_STRING::u8string_view& view) -> bool {
			// if we reach tail, return false to stop join process
			if (first == last) return false;
			// otherwise fetch data, inc iterator and return.
			view = *first;
			++first;
			return true;
		}, decilmer);
	}

	/**
	 * @brief Convert given string to lowercase.
	 * @param[in,out] strl The string to be lowercase.
	*/
	void lower(NS_YYCC_STRING::u8string& strl);
	/**
	 * @brief Return a copy of the string converted to lowercase.
	 * @param[in] strl The string to be lowercase.
	 * @return The copy of the string converted to lowercase.
	*/
	NS_YYCC_STRING::u8string lower(const NS_YYCC_STRING::u8string_view& strl);
	/**
	 * @brief Convert given string to uppercase.
	 * @param[in,out] strl The string to be uppercase.
	*/
	void upper(NS_YYCC_STRING::u8string& strl);
	/**
	 * @brief Return a copy of the string converted to uppercase.
	 * @param[in] strl The string to be uppercase.
	 * @return The copy of the string converted to uppercase.
	*/
	NS_YYCC_STRING::u8string upper(const NS_YYCC_STRING::u8string_view& strl);

	/**
	 * @brief Split given string with specified decilmer.
	 * @param[in] strl The string need to be splitting.
	 * @param[in] _decilmer The decilmer for splitting.
	 * @return 
	 * The split result.
	 * \par
	 * If given string or decilmer are empty,
	 * the result container will only contain 1 entry which is equal to given string.
	*/
	std::vector<NS_YYCC_STRING::u8string> split(const NS_YYCC_STRING::u8string_view& strl, const NS_YYCC_STRING::u8string_view& _decilmer);
	/**
	 * @brief Split given string with specified decilmer as string view.
	 * @param[in] strl The string need to be splitting.
	 * @param[in] _decilmer The decilmer for splitting.
	 * @return 
	 * The split result with string view format.
	 * This will not produce any copy of original string.
	 * \par
	 * If given string or decilmer are empty,
	 * the result container will only contain 1 entry which is equal to given string.
	 * @see Split(const NS_YYCC_STRING::u8string_view&, const NS_YYCC_STRING::u8char*)
	*/
	std::vector<NS_YYCC_STRING::u8string_view> split_view(const NS_YYCC_STRING::u8string_view& strl, const NS_YYCC_STRING::u8string_view& _decilmer);

}

#undef NS_YYCC_STRING
