#pragma once
#include <string>

/**
 * @brief The helper providing universal C\# style console function and other console related stuff
 * @details
 * The origin of this namespace is coming from the requirement of UTF8 console in Windows.
 * There are 3 ways to make Windows console enable UTF8 mode.
 * 
 * First one is calling \c SetConsoleCP and \c SetConsoleOutputCP.
 * The side effect of this is \c std::cin and \c std::cout is broken,
 * however there is a patch for this issue.
 * 
 * Second one is calling \c _set_mode with \c _O_U8TEXT or \c _O_U16TEXT to enable Unicode mode for Windows console.
 * This also have side effect which is stronger than first one.
 * All "puts" family functions (ASCII-based output functions) will throw assertion exception.
 * You only can use "putws" family functions (wide-char-based output functions).
 * However these functions can not be used without calling \c _set_mode in Windows design.
 * 
 * There still is another method, using \c WriteConsoleW directly visiting console.
 * This function family can output correct string without calling any extra functions!
 * This method is what we adopted and finally become this namespace.
 * 
 * Reference:
 * \li https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
 * \li https://stackoverflow.com/questions/69830460/reading-utf-8-input
 * 
 * For how to utilize this functions provided by this namespace, please view \ref csconsole.
 * 
 * @warning
 * All functions provided by this namespace are too aggressive.
 * Once you use it, you should not use any other input output functions.
 * 
 * @deprecated
 * This namespace provided functions are too aggressive and can not cover all use scenario.
 * So I start to give this up when migrating this namespace during developing YYCC 2.x version.
 * I just do a simple type fix and rename when migrating this namespace to make it "just works".
 * There is no suggestion for using this namespace. And there is no update for this namespace.
 * Programmer should treat Windows UTF8 issue on their own.
*/
namespace yycc::carton::csconsole {

    /**
	 * @brief Reads the next line of UTF8 characters from the standard input stream.
	 * @return
	 * The next line of UTF8 characters from the input stream.
	 * Empty string if user just press Enter key or function failed.
	*/
    std::u8string read_line();

    /**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
    void format(const char8_t* u8_fmt, ...);
    /**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
    void format_line(const char8_t* u8_fmt, ...);
    /**
	 * @brief Writes the specified string value to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
    void write(const char8_t* u8_strl);
    /**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
    void write_line(const char8_t* u8_strl);

    /**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
    void eformat(const char8_t* u8_fmt, ...);
    /**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
    void eformat_line(const char8_t* u8_fmt, ...);
    /**
	 * @brief Writes the specified string value to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
    void ewrite(const char8_t* u8_strl);
    /**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
    void ewrite_line(const char8_t* u8_strl);

}
