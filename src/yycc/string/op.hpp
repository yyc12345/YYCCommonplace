#pragma once
#include <string>
#include <string_view>
#include <cstdarg>
#include <functional>
#include <vector>
#include <expected>

namespace yycc::string::op {

    enum class FormatError {
        NullFormat,     ///< Given format string is nullptr.
        NoDesiredSize,  ///< Fail to fetch the expected size of result.
        BadWrittenSize, ///< The written size is different with expected size.
    };

    template<typename T>
    using FormatResult = std::expected<T, FormatError>;

    /**
     * @brief Perform an UTF8 string formatting operation.
     * @param[in] format The format string.
     * @param[in] ... Argument list of format string.
     * @return The formatted result, or the fail reason.
    */
    FormatResult<std::u8string> printf(const char8_t* format, ...);
    /**
     * @brief Perform an UTF8 string formatting operation.
     * @param[in] format The format string.
     * @param[in] argptr Argument list of format string.
     * @return The formatted result, or the fail reason.
    */
    FormatResult<std::u8string> vprintf(const char8_t* format, va_list argptr);
    /**
     * @brief Perform an ordinary string formatting operation.
     * @param[in] format The format string.
     * @param[in] ... Argument list of format string.
     * @return The formatted result, or the fail reason.
    */
    FormatResult<std::string> printf(const char* format, ...);
    /**
     * @brief Perform an ordinary string formatting operation.
     * @param[in] format The format string.
     * @param[in] argptr Argument list of format string.
     * @return The formatted result, or the fail reason.
    */
    FormatResult<std::string> vprintf(const char* format, va_list argptr);

    /**
	 * @brief Modify given string with all occurrences of substring \e old replaced by \e new.
	 * @param[in,out] strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	*/
    void replace(std::u8string& strl, const std::u8string_view& _from_strl, const std::u8string_view& _to_strl);
    /**
	 * @brief Return a copy with all occurrences of substring \e old replaced by \e new.
	 * @param[in] _strl The string for replacing
	 * @param[in] _from_strl The \e old string.
	 * @param[in] _to_strl The \e new string.
	 * @return The result of replacement.
	*/
    std::u8string replace(const std::u8string_view& _strl, const std::u8string_view& _from_strl, const std::u8string_view& _to_strl);

    /**
	 * @brief The data provider of general join function.
	 * @details
	 * For programmer using lambda to implement this function pointer:
	 * \li During calling, implementation should assign the reference of string view passed in argument
	 * to the string which need to be joined.
	 * \li Function return true to continue joining. otherwise return false to stop joining. 
	 * The argument content assigned in the calling returning false is not included in join process.
	*/
    using JoinDataProvider = std::function<bool(std::u8string_view&)>;
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
    std::u8string join(JoinDataProvider fct_data, const std::u8string_view& delimiter);
    /**
	 * @brief Specialized join function for standard library container.
	 * @tparam InputIt 
	 * Must meet the requirements of LegacyInputIterator.
     * It also can be dereferenced and then implicitly converted to std::u8string_view.
	 * @param[in] first The beginning of the range of elements to join.
	 * @param[in] last The terminal of the range of elements to join (exclusive).
	 * @param[in] delimiter The delimiter used for joining.
	 * @return The result string of joining.
	*/
    template<class InputIt>
    std::u8string join(InputIt first, InputIt last, const std::u8string_view& delimiter) {
        return join(
            [&first, &last](std::u8string_view& view) -> bool {
                // if we reach tail, return false to stop join process
                if (first == last) return false;
                // otherwise fetch data, inc iterator and return.
                view = *first;
                ++first;
                return true;
            },
            delimiter);
    }

    /**
	 * @brief Convert given string to lowercase.
	 * @param[in,out] strl The string to be lowercase.
	*/
    void lower(std::u8string& strl);
    /**
	 * @brief Return a copy of the string converted to lowercase.
	 * @param[in] strl The string to be lowercase.
	 * @return The copy of the string converted to lowercase.
	*/
    std::u8string to_lower(const std::u8string_view& strl);
    /**
	 * @brief Convert given string to uppercase.
	 * @param[in,out] strl The string to be uppercase.
	*/
    void upper(std::u8string& strl);
    /**
	 * @brief Return a copy of the string converted to uppercase.
	 * @param[in] strl The string to be uppercase.
	 * @return The copy of the string converted to uppercase.
	*/
    std::u8string to_upper(const std::u8string_view& strl);

    // TODO:
    // Add strip, lstrip and rstrip functions.

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
     * @see Split(const std::u8string_view&, const char8_t*)
    */
    std::vector<std::u8string_view> split(const std::u8string_view& strl, const std::u8string_view& _delimiter);
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
    std::vector<std::u8string> split_owned(const std::u8string_view& strl, const std::u8string_view& _delimiter);

    // TODO:
    // Add lazy_split(const std::u8string_view& strl, const std::u8string_view& _delimiter);
    // Once we add it, we need redirect all split function into it.

} // namespace yycc::string::op
