#pragma once
#include "../macro/printf_checker.hpp"
#include <string>
#include <string_view>
#include <cstdarg>
#include <functional>
#include <vector>
#include <optional>
#include <iterator>

namespace yycc::string::op {

#pragma region Printf

    // YYC MARK:
    // Shitty __attribute__((format(gnu_printf, (A), (B)))) force the type of format string is const char*.
    // My function signature will cause compile error which can not be removed by any switches.
    // I guess Clang may have same issue.
    // So I sadly disable format string check for printf in UTF8 char type.

    /**
     * @brief Perform an UTF8 string formatting operation.
     * @param[in] format The format string.
     * @param[in] ... Argument list of format string.
     * @return The formatted result.
    */
    std::u8string printf(/*YYCC_PRINTF_CHECK_FMTSTR*/ const char8_t* format, ...) /*YYCC_PRINTF_CHECK_ATTR(1, 2)*/;
    /**
     * @brief Perform an UTF8 string formatting operation.
     * @param[in] format The format string.
     * @param[in] argptr Argument list of format string.
     * @return The formatted result.
    */
    std::u8string vprintf(const char8_t* format, va_list argptr);
    /**
     * @brief Perform an ordinary string formatting operation.
     * @param[in] format The format string.
     * @param[in] ... Argument list of format string.
     * @return The formatted result.
    */
    std::string printf(YYCC_PRINTF_CHECK_FMTSTR const char* format, ...) YYCC_PRINTF_CHECK_ATTR(1, 2);
    /**
     * @brief Perform an ordinary string formatting operation.
     * @param[in] format The format string.
     * @param[in] argptr Argument list of format string.
     * @return The formatted result.
    */
    std::string vprintf(const char* format, va_list argptr);

#pragma endregion

#pragma region Replace

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

#pragma endregion

#pragma region Join

    /**
	 * @brief The data provider of general join function.
	 * @details
     * This data provider is more like Rust iterator.
	 * For programmer using lambda to implement this function pointer:
     * \li Return \c std::nullopt if there is no more data for join.
     * \li Return \c std::u8string_view for the data of join.
	*/
    using JoinDataProvider = std::function<std::optional<std::u8string_view>()>;
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
    template<std::input_iterator InputIt>
        requires std::is_constructible_v<std::u8string_view, std::iter_value_t<InputIt>>
    std::u8string join(InputIt first, InputIt last, const std::u8string_view& delimiter) {
        return join(
            [&first, &last]() -> std::optional<std::u8string_view> {
                // if we reach tail, return std::nullopt to stop join process
                if (first == last) return std::nullopt;
                // otherwise fetch data, inc iterator and return.
                return std::u8string_view(*(first++));
            },
            delimiter);
    }

#pragma endregion

#pragma region Lower Upper

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

#pragma endregion

#pragma region Strip

    /**
     * @brief Remove leading and trailing whitespace from the string.
     * @param[in,out] strl The string to be stripped.
     * @param[in] words The characters to be stripped.
     * @return The string view with leading and trailing whitespace removed.
     */
    std::u8string_view strip(const std::u8string_view& strl, const std::u8string_view& words);

    /**
     * @brief Remove leading whitespace from the string.
     * @param[in,out] strl The string to be stripped.
     * @param[in] words The characters to be stripped.
     * @return The string view with leading whitespace removed.
     */
    std::u8string_view lstrip(const std::u8string_view& strl, const std::u8string_view& words);

    /**
     * @brief Remove trailing whitespace from the string.
     * @param[in,out] strl The string to be stripped.
     * @param[in] words The characters to be stripped.
     * @return The string view with trailing whitespace removed.
     */
    std::u8string_view rstrip(const std::u8string_view& strl, const std::u8string_view& words);

#pragma endregion

#pragma region Split

    /**
     * @brief Iterator class for lazy splitting of strings.
     */
    class LazySplitIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::u8string_view;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::u8string_view*;
        using reference = const std::u8string_view&;

    private:
        /**
         * @brief Current splitted item.
         * @details Currently already splitted item for user fetching.
         * If this value is std::nullopt, it means that we are reach the split process endpoint.
         */
        std::optional<std::u8string_view> m_current_str;
        /**
         * @brief The string passed to the next of iterator.
         * @details It actually the remains after split excluding delimiter.
         */
        std::optional<std::u8string_view> m_next_str;
        std::u8string_view m_delimiter; ///< Delimiter

    public:
        LazySplitIterator(std::optional<std::u8string_view> strl, const std::u8string_view& delimiter);

        reference operator*() const;
        pointer operator->() const;
        LazySplitIterator& operator++();
        LazySplitIterator operator++(int);
        bool operator==(const LazySplitIterator& other) const;
        bool operator!=(const LazySplitIterator& other) const;
    };

    /**
     * @brief Class for lazy splitting of strings.
     */
    class LazySplit {
    private:
        std::u8string_view m_strl;      ///< Original string
        std::u8string_view m_delimiter; ///< Delimiter

    public:
        LazySplit(const std::u8string_view& strl, const std::u8string_view& delimiter);
        LazySplitIterator begin() const;
        LazySplitIterator end() const;
    };

    /**
     * @brief Lazily split given string with specified delimiter.
     * @param[in] strl The string need to be splitting.
     * @param[in] delimiter The delimiter for splitting.
     * @return
     * LazySplit object that can be used in range-based for loops.
     * \par
     * Every items in result is a splitted entries.
     * If given string or delimiter are empty,
     * the result container will only contain one entry which is equal to given string.
     */
    LazySplit lazy_split(const std::u8string_view& strl, const std::u8string_view& delimiter);
    /**
     * @brief Split given string with specified delimiter as string view.
     * @details
     * If your split involve large items, please consider using lazy_split(),
     * because it split entries one by one rather than one time output.
     * @param[in] strl The string need to be splitting.
     * @param[in] delimiter The delimiter for splitting.
     * @return Split result in string view format.
     * @see lazy_split() for more about aplit rules.
    */
    std::vector<std::u8string_view> split(const std::u8string_view& strl, const std::u8string_view& delimiter);
    /**
	 * @brief Split given string with specified delimiter.
     * @details
     * If there is no requirement about storing result, 
     * please consider using split() becuase it use less memory.
	 * @param[in] strl The string need to be splitting.
	 * @param[in] delimiter The delimiter for splitting.
	 * @return Split result in string format.
     * @see lazy_split() for more about aplit rules.
	*/
    std::vector<std::u8string> split_owned(const std::u8string_view& strl, const std::u8string_view& delimiter);

#pragma endregion

} // namespace yycc::string::op
