#include "op.hpp"
#include <map>
#include <memory>
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

    std::u8string to_replace(const std::u8string_view& _strl, const std::u8string_view& _from_strl, const std::u8string_view& _to_strl) {
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

        // fetch element
        while (auto item = fct_data()) {
            // append delimiter if it is not first
            if (is_first) is_first = false;
            else ret.append(delimiter);

            // append element
            ret.append(item.value());
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

#pragma region Strip

#pragma region Code Point Iterator

    class CodePointIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::u8string_view;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::u8string_view*;
        using reference = const std::u8string_view&;

    private:
        std::u8string_view current_str;
        std::u8string_view next_str;

    public:
        CodePointIterator() : CodePointIterator(std::u8string_view()) {}
        CodePointIterator(const std::u8string_view& strl) : current_str(), next_str(strl) { ++(*this); }
        YYCC_DEFAULT_COPY_MOVE(CodePointIterator)

        reference operator*() const { return this->current_str; }
        pointer operator->() const { return &this->current_str; }
        CodePointIterator& operator++() {
            // move next string to current string and analyse it
            current_str = next_str;
            next_str = std::u8string_view();

            // we only process it if there is some chars
            if (!current_str.empty()) {
                // extract the string to be checked
                std::u8string_view strl = current_str;

                // get how many bytes this code point occupied.
                size_t bytes_to_skip = evaluate_utf8_byte_count(strl.front());
                // if evaluate skip size is overflow the whole size of string, throw exception
                if (bytes_to_skip > strl.size()) throw std::runtime_error("bad utf8 sequence. no sufficient continuation bytes.");
                // check following bytes are starts with 0b10
                for (size_t i = 1; i < bytes_to_skip; ++i) {
                    check_continuation_byte(strl[i]);
                }

                // Everything is okey, set current string and next string
                current_str = strl.substr(0, bytes_to_skip);
                next_str = strl.substr(bytes_to_skip);
            }

            // return self
            return *this;
        }
        CodePointIterator operator++(int) {
            CodePointIterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const CodePointIterator& other) const {
            return this->current_str == other.current_str && this->next_str == other.next_str;
        }
        bool operator!=(const CodePointIterator& other) const { return !(*this == other); }

    private:
        /**
         * @brief Calulate how many bytes following code point occupied according to first byte of sequence.
         * @param[in] byte First sequence for checking.
         * @return The size of following code point occupied ranging from 1 to 4 (inclusive).
         */
        size_t evaluate_utf8_byte_count(char8_t c) const {
            auto byte = static_cast<uint8_t>(c);
            if ((byte & 0x80) == 0x00) return 1; // 0xxxxxxx
            if ((byte & 0xE0) == 0xC0) return 2; // 110xxxxx
            if ((byte & 0xF0) == 0xE0) return 3; // 1110xxxx
            if ((byte & 0xF8) == 0xF0) return 4; // 11110xxx
            throw std::runtime_error("invalid utf8 sequence. bad start byte");
        }
        /**
         * @brief Check whether given byte is a valid continuation byte in UTF8.
         * @param[in] c Byte for checking.
         */
        void check_continuation_byte(char8_t c) const {
            auto byte = static_cast<uint8_t>(c);
            if ((byte & 0xC0) != 0x80) {
                throw std::runtime_error("bad utf8 sequence. no sufficient continuation bytes.");
            }
        }
    };

#pragma endregion

#pragma region Code Point

    class CodePoint {
    private:
        std::u8string_view u8str;

    public:
        explicit CodePoint(std::u8string_view u8str) : u8str(u8str) {}
        YYCC_DEFAULT_COPY_MOVE(CodePoint)

        CodePointIterator begin() const { return CodePointIterator(u8str); }
        CodePointIterator end() const {
            // Pass empty string view indicate end.
            return CodePointIterator(std::u8string_view());
        }
    };

#pragma endregion

#pragma region Trie Tree Node

    struct TrieTreeNode {
        TrieTreeNode() : is_end(false), children() {}
        bool is_end;                                               ///< Whether this node is a viable end.
        std::map<char8_t, std::unique_ptr<TrieTreeNode>> children; ///< The children node.
    };

#pragma endregion

#pragma region Trie Tree

    class TrieTree {
    private:
        std::unique_ptr<TrieTreeNode> root;

    public:
        TrieTree() : root(std::make_unique<TrieTreeNode>()) {
            // Do not accept root element always (no empty string).
            root->is_end = false;
        }

        /**
         * @brief Insert new words in trie tree.
         * @details
         * The reason why use iterator, rather than string view, is that in strip function, we may need insert a string backwardly,
         * so using string view reverse iterator and iterator argument can easily achieve this goal without any more burden.
         * @tparam InputIt The iterator following input iterator name convention.
         * @param[in] first The head of iterator.
         * @param[in] last The tail of iterator.
         */
        template<std::input_iterator InputIt>
            requires std::is_same_v<std::iter_value_t<InputIt>, char8_t>
        void insert(InputIt first, InputIt last) {
            // prevent empty string
            if (first == last) return;

            // insert item
            TrieTreeNode* node = root.get();
            for (auto it = first; it != last; ++it) {
                char8_t c = *it;
                if (node->children.find(c) == node->children.end()) {
                    node->children[c] = std::make_unique<TrieTreeNode>();
                }
                node = node->children[c].get();
            }
            node->is_end = true;
        }

        /**
         * @brief Check whether given words has prefix stored in this trie tree.
         * @details Same reason for using iterator as function argument.
         * @tparam InputIt The iterator following input iterator name convention.
         * @param[in] first The head of iterator.
         * @param[in] last The tail of iterator.
         * @return \c std::nullopt if there is no match, otherwise the length of matched prefix.
         */
        template<std::input_iterator InputIt>
            requires std::is_same_v<std::iter_value_t<InputIt>, char8_t>
        std::optional<size_t> search(InputIt first, InputIt last) {
            TrieTreeNode* node = root.get();
            size_t cnt = 0;

            for (auto it = first; it != last; ++it) {
                char8_t c = *it;
                auto finder = node->children.find(c);
                if (finder == node->children.end()) {
                    // There is no more matching, break the while.
                    break;
                } else {
                    // There are more matching item, find next one.
                    node = finder->second.get();
                    ++cnt;
                }
            }

            // YYC MARK:
            // There is a fatal bug for Trie Tree, but it doesn't matter with our usage scenario.
            //
            // Assume there is two string "ab" and "abcd". If user give "abc",
            // we should match it with "ab" prefix, but this function will return there is no match.
            // However, this is impossible for UTF8 sequence.
            // There is no possibility that two UTF8 sequence, indicating two different Unicode code point respectively,
            // has the same prefix and different length. Because their first byte must be different,
            // the first byte indicate the length of sequence.
            //
            // This result also can be proven for suffix,
            // because first byte must not be equal to any other continuation bytes.
            // It is impossible that they have same "ab".
            //
            // So it is safe for our usage scenario although this bug is presented.

            // check whether current is valid end.
            // if it is, return the count of prefix, otherwise return nothing.
            if (node->is_end) {
                return cnt;
            } else {
                return std::nullopt;
            }
        }
    };

#pragma endregion

    template<bool bDoLeft, bool bDoRight>
    static std::u8string_view internal_strip(const std::u8string_view& strl, const std::u8string_view& words) {
        std::optional<TrieTree> prefix, suffix;
        if constexpr (bDoLeft) prefix = TrieTree();
        if constexpr (bDoRight) suffix = TrieTree();

        CodePoint code_point(words);
        for (const auto& seq : code_point) {
            if (prefix.has_value()) prefix.value().insert(seq.begin(), seq.end());
            if (suffix.has_value()) suffix.value().insert(seq.rbegin(), seq.rend());
        }

        std::u8string_view striped = strl;
        if constexpr (bDoLeft) {
            while (auto cnt = prefix.value().search(striped.begin(), striped.end())) {
                striped = striped.substr(cnt.value());
            }
        }
        if constexpr (bDoRight) {
            while (auto cnt = suffix.value().search(striped.rbegin(), striped.rend())) {
                striped = striped.substr(0, striped.size() - cnt.value());
            }
        }

        return striped;
    }

    std::u8string_view strip(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_strip<true, true>(strl, words);
    }

    std::u8string_view lstrip(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_strip<true, false>(strl, words);
    }

    std::u8string_view rstrip(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_strip<false, true>(strl, words);
    }

#pragma endregion

#pragma region Trim

    template<bool bDoLeft, bool bDoRight>
    std::u8string_view internal_trim(const std::u8string_view& strl, const std::u8string_view& words) {
        // check words
        if (!std::ranges::none_of(words, [](auto c) { return static_cast<uint8_t>(c) & 0x80; })) {
            throw std::invalid_argument("given words are not all ASCII (<= 0x7F) only");
        }

        // prepare return value
        std::u8string_view rv = strl;

        // remove left first
        if constexpr (bDoLeft) {
            auto finder = rv.find_first_not_of(words);
            if (finder == std::u8string_view::npos) {
                // all string are in given words
                rv = std::u8string_view();
            } else {
                // remove by offset
                rv = rv.substr(finder);
            }
        }

        // remove right
        if constexpr (bDoRight) {
            auto finder = rv.find_last_not_of(words);
            if (finder == std::u8string_view::npos) {
                // all string are in given words
                rv = std::u8string_view();
            } else {
                // remove by offset
                rv = rv.substr(0, finder + 1);
            }
        }

        // return value
        return rv;
    }

    std::u8string_view trim(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_trim<true, true>(strl, words);
    }

    std::u8string_view ltrim(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_trim<true, false>(strl, words);
    }

    std::u8string_view rtrim(const std::u8string_view& strl, const std::u8string_view& words) {
        return internal_trim<false, true>(strl, words);
    }

#pragma endregion

#pragma region Split

    // Reference:
    // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

#pragma region Lazy Split Iterator

    LazySplitIterator::LazySplitIterator() : LazySplitIterator(std::nullopt, std::u8string_view()) {}

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
        // YYC MARK: do not compare the delimiter
        return (this->m_current_str == other.m_current_str) && (this->m_next_str == other.m_next_str);
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
