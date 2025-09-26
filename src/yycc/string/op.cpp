#include "op.hpp"
#include <type_traits>
#include <algorithm>
#include <stdexcept>

namespace yycc::string::op {

#pragma region Printf VPrintf

    /// @brief The concept for all viable char type in printf function family
    template<typename TChar>
    concept PrintfSupportedChar = std::is_same_v<TChar, char> || std::is_same_v<TChar, char8_t>;

    template<PrintfSupportedChar TChar>
    static std::basic_string<TChar> generic_printf(const TChar* format, va_list argptr) {
        // Prepare result
        std::basic_string<TChar> rv;

        // Prepare variable arguments
        va_list args1;
        va_copy(args1, argptr);
        va_list args2;
        va_copy(args2, argptr);

        // The return value is desired char count without NULL terminal.
        // Negative number means error.
        int count = std::vsnprintf(nullptr, 0, reinterpret_cast<const char*>(format), args1);
        // Check expected size.
        if (count < 0) {
            // Invalid length returned by vsnprintf.
            // This may be caused by invalid format string
            throw std::logic_error("fail to determine the size of formatted string");
        }
        va_end(args1);

        // Resize std::string to desired count, and pass its length + 1 to std::vsnprintf,
        // Because std::vsnprintf only can write "buf_size - 1" chars with a trailing NULL.
        // However std::vsnprintf already have a trailing NULL, so we plus 1 for it.
        rv.resize(count);
        int write_result = std::vsnprintf(reinterpret_cast<char*>(rv.data()), rv.size() + 1, reinterpret_cast<const char*>(format), args2);
        va_end(args2);
        // Check written size.
        if (write_result < 0 || write_result > count) {
            // Invalid write result in vsnprintf.
            // Idk why this can happen.
            throw std::logic_error("the size of written formatted string is not expected");
        }

        // Return value
        return rv;
    }

    std::u8string printf(const char8_t* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        auto rv = vprintf(format, argptr);
        va_end(argptr);
        return rv;
    }

    std::u8string vprintf(const char8_t* format, va_list argptr) {
        return generic_printf(format, argptr);
    }

    std::string printf(const char* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        auto rv = vprintf(format, argptr);
        va_end(argptr);
        return rv;
    }

    std::string vprintf(const char* format, va_list argptr) {
        return generic_printf(format, argptr);
    }

#pragma endregion

#pragma region Replace

    void replace(std::u8string& strl, const std::u8string_view& _from_strl, const std::u8string_view& _to_strl) {
        // Reference: https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string

        // check requirements
        // from string should not be empty
        std::u8string from_strl(_from_strl);
        std::u8string to_strl(_to_strl);
        if (from_strl.empty()) return;

        // start replace one by one
        size_t start_pos = 0;
        while ((start_pos = strl.find(from_strl, start_pos)) != std::u8string::npos) {
            strl.replace(start_pos, from_strl.size(), to_strl);
            start_pos += to_strl.size(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    std::u8string replace(const std::u8string_view& _strl, const std::u8string_view& _from_strl, const std::u8string_view& _to_strl) {
        // prepare result
        std::u8string strl(_strl);
        replace(strl, _from_strl, _to_strl);
        // return value
        return strl;
    }

#pragma endregion

#pragma region Join

    std::u8string join(JoinDataProvider fct_data, const std::u8string_view& delimiter) {
        std::u8string ret;
        bool is_first = true;
        std::u8string_view element;

        // fetch element
        while (fct_data(element)) {
            // insert delimiter
            if (is_first) is_first = false;
            else {
                // append delimiter.
                ret.append(delimiter);
            }

            // insert element if it is not empty
            if (!element.empty()) ret.append(element);
        }

        return ret;
    }

#pragma endregion

#pragma region Upper Lower

    template<bool BIsToLower>
    static void generic_lower_upper(std::u8string& strl) {
        // References:
        // https://en.cppreference.com/w/cpp/algorithm/transform
        // https://en.cppreference.com/w/cpp/string/byte/tolower
        std::transform(strl.cbegin(), strl.cend(), strl.begin(), [](unsigned char c) -> char {
            if constexpr (BIsToLower) return std::tolower(c);
            else return std::toupper(c);
        });
    }

    void lower(std::u8string& strl) {
        generic_lower_upper<true>(strl);
    }

    std::u8string to_lower(const std::u8string_view& strl) {
        std::u8string ret(strl);
        lower(ret);
        return ret;
    }

    void upper(std::u8string& strl) {
        generic_lower_upper<false>(strl);
    }

    std::u8string to_upper(const std::u8string_view& strl) {
        // same as Lower, just replace char transform function.
        std::u8string ret(strl);
        upper(ret);
        return ret;
    }

#pragma endregion

#pragma region Split

    // Reference:
    // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

#pragma region Lazy Split Iterator

    LazySplitIterator::LazySplitIterator(std::optional<std::u8string_view> strl, const std::u8string_view& delimiter) :
        m_current_str(std::nullopt), m_next_str(strl), m_delimiter(delimiter) {
        // We can archive result by assign string into next string,
        // and call next operator.
        ++(*this);
    }

    LazySplitIterator::reference LazySplitIterator::operator*() const {
        return m_current_str.value();
    }

    LazySplitIterator::pointer LazySplitIterator::operator->() const {
        return &m_current_str.value();
    }

    LazySplitIterator& LazySplitIterator::operator++() {
        // move next string to current string and clear next string
        m_current_str = m_next_str;
        m_next_str = std::nullopt;

        // check whether there is string to be spliited
        if (m_current_str.has_value()) {
            // extract string
            const auto strl = m_current_str.value();

            // if delimiter is empty, return original string.
            // if string need to be splitted is empty, return original string (empty string).
            if (strl.empty() || m_delimiter.empty()) {
                m_current_str = strl;
                m_next_str = std::nullopt;
            } else {
                // start spliting
                size_t current = 0;
                if ((current = strl.find(m_delimiter)) != std::u8string_view::npos) {
                    // We still can find items, split it and put into 2 string view respectively.
                    m_current_str = strl.substr(0, current);
                    m_next_str = strl.substr(current + m_delimiter.size());
                } else {
                    // We can not find any more delimiter, so this is the last item
                    m_current_str = strl;
                    m_next_str = std::nullopt;
                }
            }
        } else {
            // No value. Initialize as an end iterator.
            m_current_str = std::nullopt;
            m_next_str = std::nullopt;
        }

        // return self
        return *this;
    }

    LazySplitIterator LazySplitIterator::operator++(int) {
        LazySplitIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool LazySplitIterator::operator==(const LazySplitIterator& other) const {
        return (this->m_current_str == other.m_current_str) && (this->m_next_str == other.m_next_str)
               && (this->m_delimiter == other.m_delimiter);
    }

    bool LazySplitIterator::operator!=(const LazySplitIterator& other) const {
        return !(*this == other);
    }

#pragma endregion

#pragma region Lazy Split

    LazySplit::LazySplit(const std::u8string_view& strl, const std::u8string_view& delimiter) : m_strl(strl), m_delimiter(delimiter) {}

    LazySplitIterator LazySplit::begin() const {
        return LazySplitIterator(m_strl, m_delimiter);
    }

    LazySplitIterator LazySplit::end() const {
        // Pass std::nullopt to indicate end.
        return LazySplitIterator(std::nullopt, m_delimiter);
    }

#pragma endregion

    LazySplit lazy_split(const std::u8string_view& strl, const std::u8string_view& delimiter) {
        return LazySplit(strl, delimiter);
    }

    std::vector<std::u8string_view> split(const std::u8string_view& strl, const std::u8string_view& delimiter) {
        // use lazy split iterator to do the splitting
        std::vector<std::u8string_view> elems;
        for (const auto& strl_view : lazy_split(strl, delimiter)) {
            elems.emplace_back(strl_view);
        }
        return elems;
    }

    std::vector<std::u8string> split_owned(const std::u8string_view& strl, const std::u8string_view& delimiter) {
        // use lazy split iterator to do the splitting
        std::vector<std::u8string> elems;
        for (const auto& strl_view : lazy_split(strl, delimiter)) {
            elems.emplace_back(std::u8string(strl_view));
        }
        return elems;
    }

#pragma endregion

} // namespace yycc::string::op
