#include "op.hpp"

#include <algorithm>

namespace yycc::string::op {

#pragma region Printf VPrintf

    template<typename TChar>
        requires(sizeof(TChar) == sizeof(char))
    static FormatResult<std::basic_string<TChar>> generic_printf(const TChar* format, va_list argptr) {
        // Prepare result
        std::basic_string<TChar> rv;

        // Check format
        if (format == nullptr) return std::unexpected(FormatError::NullFormat);

        // Prepare variable arguments
        va_list args1;
        va_copy(args1, argptr);
        va_list args2;
        va_copy(args2, argptr);

        // The return value is desired char count without NULL terminal.
        // Minus number means error.
        int count = std::vsnprintf(nullptr, 0, reinterpret_cast<const char*>(format), args1);
        // Check expected size.
        if (count < 0) {
            // Invalid length returned by vsnprintf.
            return std::unexpected(FormatError::NoDesiredSize);
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
            return std::unexpected(FormatError::BadWrittenSize);
        }

        // Return value
        return rv;
    }

    FormatResult<std::u8string> printf(const char8_t* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        auto rv = vprintf(format, argptr);
        va_end(argptr);
        return rv;
    }

    FormatResult<std::u8string> vprintf(const char8_t* format, va_list argptr) {
        return generic_printf(format, argptr);
    }

    FormatResult<std::string> printf(const char* format, ...) {
        va_list argptr;
        va_start(argptr, format);
        auto rv = vprintf(format, argptr);
        va_end(argptr);
        return rv;
    }

    FormatResult<std::string> vprintf(const char* format, va_list argptr) {
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

    std::vector<std::u8string_view> split(const std::u8string_view& strl, const std::u8string_view& _delimiter) {
        // Reference:
        // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

        // prepare return value
        std::vector<std::u8string_view> elems;

        // if string need to be splitted is empty, return original string (empty string).
        // if delimiter is empty, return original string.
        std::u8string delimiter(_delimiter);
        if (strl.empty() || delimiter.empty()) {
            elems.emplace_back(strl);
            return elems;
        }

        // start spliting
        std::size_t previous = 0, current;
        while ((current = strl.find(delimiter.c_str(), previous)) != std::u8string::npos) {
            elems.emplace_back(strl.substr(previous, current - previous));
            previous = current + delimiter.size();
        }
        // try insert last part but prevent possible out of range exception
        if (previous <= strl.size()) {
            elems.emplace_back(strl.substr(previous));
        }
        return elems;
    }

    std::vector<std::u8string> split_owned(const std::u8string_view& strl, const std::u8string_view& _delimiter) {
        // call split view
        auto view_result = split(strl, _delimiter);

        // copy string view result to string
        std::vector<std::u8string> elems;
        elems.reserve(view_result.size());
        for (const auto& strl_view : view_result) {
            elems.emplace_back(std::u8string(strl_view));
        }
        // return copied result
        return elems;
    }

#pragma endregion

} // namespace yycc::string::op
