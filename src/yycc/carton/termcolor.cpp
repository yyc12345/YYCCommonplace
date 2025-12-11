#include "termcolor.hpp"
#include "../flag_enum.hpp"
#include "../patch/stream.hpp"
#include <stdexcept>
#include <bit>

#define FLAG_ENUM ::yycc::flag_enum

using namespace std::literals::string_view_literals;
using namespace yycc::patch::stream;

namespace yycc::carton::termcolor {

#pragma region Lowlevel Functions

    const std::u8string_view foreground(Color color) {
        switch (color) {
            case Color::Default:
                return u8""sv;
            case Color::Black:
                return u8"\033[30m"sv;
            case Color::Red:
                return u8"\033[31m"sv;
            case Color::Green:
                return u8"\033[32m"sv;
            case Color::Yellow:
                return u8"\033[33m"sv;
            case Color::Blue:
                return u8"\033[34m"sv;
            case Color::Magenta:
                return u8"\033[35m"sv;
            case Color::Cyan:
                return u8"\033[36m"sv;
            case Color::White:
                return u8"\033[37m"sv;
            case Color::LightBlack:
                return u8"\033[90m"sv;
            case Color::LightRed:
                return u8"\033[91m"sv;
            case Color::LightGreen:
                return u8"\033[92m"sv;
            case Color::LightYellow:
                return u8"\033[93m"sv;
            case Color::LightBlue:
                return u8"\033[94m"sv;
            case Color::LightMagenta:
                return u8"\033[95m"sv;
            case Color::LightCyan:
                return u8"\033[96m"sv;
            case Color::LightWhite:
                return u8"\033[97m"sv;
            default:
                throw std::invalid_argument("invalid color kind");
        }
    }

    const std::u8string_view background(Color color) {
        switch (color) {
            case Color::Default:
                return u8""sv;
            case Color::Black:
                return u8"\033[40m"sv;
            case Color::Red:
                return u8"\033[41m"sv;
            case Color::Green:
                return u8"\033[42m"sv;
            case Color::Yellow:
                return u8"\033[43m"sv;
            case Color::Blue:
                return u8"\033[44m"sv;
            case Color::Magenta:
                return u8"\033[45m"sv;
            case Color::Cyan:
                return u8"\033[46m"sv;
            case Color::White:
                return u8"\033[47m"sv;
            case Color::LightBlack:
                return u8"\033[100m"sv;
            case Color::LightRed:
                return u8"\033[101m"sv;
            case Color::LightGreen:
                return u8"\033[102m"sv;
            case Color::LightYellow:
                return u8"\033[103m"sv;
            case Color::LightBlue:
                return u8"\033[104m"sv;
            case Color::LightMagenta:
                return u8"\033[105m"sv;
            case Color::LightCyan:
                return u8"\033[106m"sv;
            case Color::LightWhite:
                return u8"\033[107m"sv;
            default:
                throw std::invalid_argument("invalid color kind");
        }
    }

    const std::u8string_view style(Attribute attr) {
        // Return for Default first because it can not pass following test
        if (attr == Attribute::Default) {
            return u8""sv;
        }

        // Check whether it only has one flag
        if (!std::has_single_bit(FLAG_ENUM::integer(attr))) {
            throw std::invalid_argument("style() only accept single flag attribute");
        }

        // Return result
        switch (attr) {
            case Attribute::Bold:
                return u8"\033[1m"sv;
            case Attribute::Dark:
                return u8"\033[2m"sv;
            case Attribute::Italic:
                return u8"\033[3m"sv;
            case Attribute::Underline:
                return u8"\033[4m"sv;
            case Attribute::Blink:
                return u8"\033[5m"sv;
            case Attribute::Reverse:
                return u8"\033[6m"sv;
            case Attribute::Concealed:
                return u8"\033[7m"sv;
            default:
                throw std::invalid_argument("invalid attribute kind");
        }
    }

    /**
     * @private
     * @brief The possible maximum length of ANSI Escape Sequence used in this module.
     * @details This const value is used for computing reserved size of final built string.
     */
    static constexpr size_t ANSI_ESC_LEN = sizeof(u8"\033[000m") - 1;

    /**
     * @private
     * @brief Count how many single flags combine given attributes.
     * @details
     * For function styles() involving multiple font style ANSI Escape Sequence,
     * this function may be useful for computing desired size of final result,
     * to reduce useless memory re-allocation.
     * @param[in] attrs Attributes for counting.
     * @return The count of single flag.
     */
    static size_t count_attribute_flags(Attribute attrs) {
        return static_cast<size_t>(std::popcount(FLAG_ENUM::integer(attrs)));
    }

    /**
     * @private
     * @brief Append multiple font styles into given string.
     * @details
     * This function will decompose given font styles into single flag.
     * And append its components one by one into given string.
     * If there is enough reserved space in given string,
     * there is no memory re-allocation happened.
     * @remarks
     * This function is served for styles() and colored().
     * @param[in] s The string to be appended.
     * @param[in] attrs The attributes for writting.
     */
    static void append_styles(std::u8string& s, Attribute attrs) {
#define CHECK_ATTR(probe) \
    if (FLAG_ENUM::has(attrs, probe)) s.append(termcolor::style(probe));

        if (attrs != Attribute::Default) {
            CHECK_ATTR(Attribute::Bold);
            CHECK_ATTR(Attribute::Dark);
            CHECK_ATTR(Attribute::Italic);
            CHECK_ATTR(Attribute::Blink);
            CHECK_ATTR(Attribute::Reverse);
            CHECK_ATTR(Attribute::Concealed);
        }

#undef CHECK_ATTR
    }

    std::u8string styles(Attribute attrs) {
        // Prepare the result string
        std::u8string rv;
        rv.reserve(count_attribute_flags(attrs) * ANSI_ESC_LEN);
        // Append styles and return
        append_styles(rv, attrs);
        return rv;
    }

    const std::u8string_view reset() {
        return u8"\033[0m"sv;
    }

#pragma endregion

#pragma region Highlevel Functions

    std::u8string colored(const std::u8string_view& words, Color foreground, Color background, Attribute styles) {
        // Calculate the expected size of result string.
        // final count = styles count + 1 (foreground) + 1 (background) + 1 (reset)
        std::u8string rv;
        size_t ansi_esc_count = count_attribute_flags(styles) + 3;
        rv.reserve(ansi_esc_count * ANSI_ESC_LEN + words.size());

        // Append data one by one
        rv.append(termcolor::foreground(foreground));
        rv.append(termcolor::background(background));
        append_styles(rv, styles);
        rv.append(words);
        rv.append(termcolor::reset());

        // Return result
        return rv;
    }

    void cprint(const std::u8string_view& words, Color foreground, Color background, Attribute styles, std::ostream& dst) {
        dst << colored(words, foreground, background, styles);
    }

    void ceprint(const std::u8string_view& words, Color foreground, Color background, Attribute styles) {
        cprint(words, foreground, background, styles, std::cerr);
    }

    void cprintln(const std::u8string_view& words, Color foreground, Color background, Attribute styles, std::ostream& dst) {
        cprint(words, foreground, background, styles, dst);
        dst << std::endl;
    }

    void ceprintln(const std::u8string_view& words, Color foreground, Color background, Attribute styles) {
        cprintln(words, foreground, background, styles, std::cerr);
    }

#pragma endregion

} // namespace yycc::carton::termcolor
