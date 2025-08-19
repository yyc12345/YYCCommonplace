#pragma once
#include <string>
#include <string_view>
#include <iostream>

/**
 * @brief The namespace for terminal font color and style.
 * @details
 * This namespace provides functions to generate ANSI escape sequence for terminal font color and style.
 * It also provides functions to add color and style for given string with ANSI Escape Sequence.
 * 
 * This namespace is basically the immitation of the Python package with same name.
 */
namespace yycc::carton::termcolor {

#pragma region Lowlevel Functions

    /**
     * @brief The color of font.
     */
    enum class Color {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        LightBlack,
        LightRed,
        LightGreen,
        LightYellow,
        LightBlue,
        LightMagenta,
        LightCyan,
        LightWhite,
        Default
    };

    /**
    * @brief Get ANSI escape sequence for foreground color
    * @param[in] color The color to generate sequence for
    * @return Gotten ANSI escape sequence
    */
    const std::u8string_view foreground(Color color);
    /**
    * @brief Get ANSI escape sequence for background color
    * @param[in] color The color to generate sequence for
    * @return Gotten ANSI escape sequence
    */
    const std::u8string_view background(Color color);

    /**
     * @brief The attribute of font
     * @remarks We define this enum as unsigned integral, so that we can use \c std::has_single_bit.
     */
    enum class Attribute : uint32_t {
        Default = 0,
        Bold = 1 << 0,
        Dark = 1 << 1,
        Italic = 1 << 2,
        Underline = 1 << 3,
        Blink = 1 << 4,
        Reverse = 1 << 5,
        Concealed = 1 << 6
    };

    /**
     * @brief Get ANSI escape sequence for text style
     * @details
     * Please note that this function only support single attribute flag.
     * If you want to use multiple attributes, please use styles() instead.
     * 
     * However, the difference between this function and styles() is that 
     * there is no memory allocation in this function.
     * It may have better performance that styles().
     * @param[in] attr Single attribute to generate sequence for
     * @return Gotten ANSI escape sequence
     * @throws std::invalid_argument if attribute is not a single flag
     */
    const std::u8string_view style(Attribute attr);
    /**
     * @brief Generates ANSI escape sequence for multiple text styles
     * @param[in] attrs Combination of attributes to generate sequences for
     * @return Generated ANSI escape sequence
     */
    std::u8string styles(Attribute attrs);
    /**
     * @brief Get ANSI escape sequence for reset style
     * @return Gotten ANSI reset sequence
     */
    const std::u8string_view reset();

#pragma endregion

#pragma region Highlevel Functions

    /**
     * @brief Add color and style for given string with ANSI Escape Sequence.
     * @param[in] words The words to be decorated.
     * @param[in] foreground The foreground of words.
     * @param[in] background The background of words.
     * @param[in] styles The font style of words.
     * @return Decorated words.
     */
    std::u8string colored(const std::u8string_view& words,
                          Color foreground = Color::Default,
                          Color background = Color::Default,
                          Attribute styles = Attribute::Default);

    /**
     * @brief Print words into stream with given styles.
     * @param[in] words The words to be printed.
     * @param[in] foreground The foreground of words.
     * @param[in] background The background of words.
     * @param[in] styles The font style of words.
     * @param[in] dst The stream written into. \c stdout in default.
     */
    void cprint(const std::u8string_view& words = std::u8string_view(u8""),
                Color foreground = Color::Default,
                Color background = Color::Default,
                Attribute styles = Attribute::Default,
                std::ostream& dst = std::cout);

    /**
     * @brief Print words into \c stderr with given styles.
     * @param[in] words The words to be printed.
     * @param[in] foreground The foreground of words.
     * @param[in] background The background of words.
     * @param[in] styles The font style of words.
     */
    void ceprint(const std::u8string_view& words = std::u8string_view(u8""),
                 Color foreground = Color::Default,
                 Color background = Color::Default,
                 Attribute styles = Attribute::Default);

    /**
     * @brief Print words into stream with given styles and break line.
     * @param[in] words The words to be printed.
     * @param[in] foreground The foreground of words.
     * @param[in] background The background of words.
     * @param[in] styles The font style of words.
     * @param[in] dst The stream written into. \c stdout in default.
     */
    void cprintln(const std::u8string_view& words = std::u8string_view(u8""),
                  Color foreground = Color::Default,
                  Color background = Color::Default,
                  Attribute styles = Attribute::Default,
                  std::ostream& dst = std::cout);

    /**
     * @brief Print words into \c stderr with given styles and break line.
     * @param[in] words The words to be printed.
     * @param[in] foreground The foreground of words.
     * @param[in] background The background of words.
     * @param[in] styles The font style of words.
     */
    void ceprintln(const std::u8string_view& words = std::u8string_view(u8""),
                   Color foreground = Color::Default,
                   Color background = Color::Default,
                   Attribute styles = Attribute::Default);

#pragma endregion

} // namespace yycc::carton::termcolor
