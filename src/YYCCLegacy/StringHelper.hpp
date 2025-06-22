#pragma once
#include "YYCCInternal.hpp"

#include <string>
#include <cstdarg>
#include <functional>
#include <vector>

/**
 * @brief The helper containing string operations
 * @details
 * See also \ref string_helper.
*/
namespace YYCC::StringHelper {

	/**
	 * @brief Perform a string formatting operation.
	 * @param[out] strl 
	 * The string container receiving the result.
	 * There is no guarantee that the content is not modified when function failed.
	 * @param[in] format The format string.
	 * @param[in] ... Argument list of format string.
	 * @return True if success, otherwise false.
	*/
	bool Printf(yycc_u8string& strl, const yycc_char8_t* format, ...);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[out] strl 
	 * The string container receiving the result.
	 * There is no guarantee that the content is not modified when function failed.
	 * @param[in] format The format string.
	 * @param[in] argptr Argument list of format string.
	 * @return True if success, otherwise false.
	*/
	bool VPrintf(yycc_u8string& strl, const yycc_char8_t* format, va_list argptr);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[in] format The format string.
	 * @param[in] ... Argument list of format string.
	 * @return The formatting result. Empty string if error happened.
	*/
	yycc_u8string Printf(const yycc_char8_t* format, ...);
	/**
	 * @brief Perform a string formatting operation.
	 * @param[in] format The format string.
	 * @param[in] argptr Argument list of format string.
	 * @return The formatting result. Empty string if error happened.
	*/
	yycc_u8string VPrintf(const yycc_char8_t* format, va_list argptr);

	/**
	 * @brief Modify given string with all occurrences of substring \e old replaced by \e new.
	 * @param[in,out] strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	*/
	void Replace(yycc_u8string& strl, const yycc_u8string_view& _from_strl, const yycc_u8string_view& _to_strl);
	/**
	 * @brief Return a copy with all occurrences of substring \e old replaced by \e new.
	 * @param[in] _strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	 * @return The result of replacement.
	*/
	yycc_u8string Replace(const yycc_u8string_view& _strl, const yycc_u8string_view& _from_strl, const yycc_u8string_view& _to_strl);

	/**
	 * @brief The data provider of general join function.
	 * @details
	 * For programmer using lambda to implement this function pointer:
	 * \li During calling, implementation should assign the reference of string view passed in argument
	 * to the string which need to be joined.
	 * \li Function return true to continue joining. otherwise return false to stop joining. 
	 * The argument content assigned in the calling returning false is not included in join process.
	*/
	using JoinDataProvider = std::function<bool(yycc_u8string_view&)>;
	/**
	 * @brief Universal join function.
	 * @details
	 * This function use function pointer as a general data provider interface,
	 * so this function suit for all types container.
	 * You can use this universal join function for any custom container by
	 * using C++ lambda syntax to create a code block adapted to this function pointer.
	 * @param[in] fct_data The function pointer in JoinDataProvider type prividing the data to be joined.
	 * @param[in] delimiter The delimiter used for joining.
	 * @return The result string of joining.
	*/
	yycc_u8string Join(JoinDataProvider fct_data, const yycc_u8string_view& delimiter);
	/**
	 * @brief Specialized join function for standard library container.
	 * @tparam InputIt 
	 * Must meet the requirements of LegacyInputIterator.
	 * It also can be dereferenced and then implicitly converted to yycc_u8string_view.
	 * @param[in] first The beginning of the range of elements to join.
	 * @param[in] last The terminal of the range of elements to join (exclusive).
	 * @param[in] delimiter The delimiter used for joining.
	 * @return The result string of joining.
	*/
	template<class InputIt>
	yycc_u8string Join(InputIt first, InputIt last, const yycc_u8string_view& delimiter) {
		return Join([&first, &last](yycc_u8string_view& view) -> bool {
			// if we reach tail, return false to stop join process
			if (first == last) return false;
			// otherwise fetch data, inc iterator and return.
			view = *first;
			++first;
			return true;
		}, delimiter);
	}

	/**
	 * @brief Convert given string to lowercase.
	 * @param[in,out] strl The string to be lowercase.
	*/
	void Lower(yycc_u8string& strl);
	/**
	 * @brief Return a copy of the string converted to lowercase.
	 * @param[in] strl The string to be lowercase.
	 * @return The copy of the string converted to lowercase.
	*/
	yycc_u8string Lower(const yycc_u8string_view& strl);
	/**
	 * @brief Convert given string to uppercase.
	 * @param[in,out] strl The string to be uppercase.
	*/
	void Upper(yycc_u8string& strl);
	/**
	 * @brief Return a copy of the string converted to uppercase.
	 * @param[in] strl The string to be uppercase.
	 * @return The copy of the string converted to uppercase.
	*/
	yycc_u8string Upper(const yycc_u8string_view& strl);

	/**
	 * @brief Split given string with specified delimiter.
	 * @param[in] strl The string need to be splitting.
	 * @param[in] _delimiter The delimiter for splitting.
	 * @return 
	 * The split result.
	 * \par
	 * If given string or delimiter are empty,
	 * the result container will only contain 1 entry which is equal to given string.
	*/
	std::vector<yycc_u8string> Split(const yycc_u8string_view& strl, const yycc_u8string_view& _delimiter);
	/**
	 * @brief Split given string with specified delimiter as string view.
	 * @param[in] strl The string need to be splitting.
	 * @param[in] _delimiter The delimiter for splitting.
	 * @return 
	 * The split result with string view format.
	 * This will not produce any copy of original string.
	 * \par
	 * If given string or delimiter are empty,
	 * the result container will only contain 1 entry which is equal to given string.
	 * @see Split(const yycc_u8string_view&, const yycc_char8_t*)
	*/
	std::vector<yycc_u8string_view> SplitView(const yycc_u8string_view& strl, const yycc_u8string_view& _delimiter);

}
