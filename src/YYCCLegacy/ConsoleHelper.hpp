#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <string>

/**
 * @brief The helper providing universal C\# style console function and other console related stuff
 * @details
 * For how to utilize this functions provided by this namespace, please view \ref console_helper.
*/
namespace YYCC::ConsoleHelper {

	/// @brief The head of ASCII escape code of black color.
#define YYCC_COLORHDR_BLACK "\033[30m"
	/// @brief The head of ASCII escape code of red color.
#define YYCC_COLORHDR_RED "\033[31m"
	/// @brief The head of ASCII escape code of green color.
#define YYCC_COLORHDR_GREEN "\033[32m"
	/// @brief The head of ASCII escape code of yellow color.
#define YYCC_COLORHDR_YELLOW "\033[33m"
	/// @brief The head of ASCII escape code of blue color.
#define YYCC_COLORHDR_BLUE "\033[34m"
	/// @brief The head of ASCII escape code of magenta color.
#define YYCC_COLORHDR_MAGENTA "\033[35m"
	/// @brief The head of ASCII escape code of cyan color.
#define YYCC_COLORHDR_CYAN "\033[36m"
	/// @brief The head of ASCII escape code of white color.
#define YYCC_COLORHDR_WHITE "\033[37m"
	
	/// @brief The head of ASCII escape code of light black color.
#define YYCC_COLORHDR_LIGHT_BLACK "\033[90m"
	/// @brief The head of ASCII escape code of light red color.
#define YYCC_COLORHDR_LIGHT_RED "\033[91m"
	/// @brief The head of ASCII escape code of light green color.
#define YYCC_COLORHDR_LIGHT_GREEN "\033[92m"
	/// @brief The head of ASCII escape code of light yellow color.
#define YYCC_COLORHDR_LIGHT_YELLOW "\033[93m"
	/// @brief The head of ASCII escape code of light blue color.
#define YYCC_COLORHDR_LIGHT_BLUE "\033[94m"
	/// @brief The head of ASCII escape code of light magenta color.
#define YYCC_COLORHDR_LIGHT_MAGENTA "\033[95m"
	/// @brief The head of ASCII escape code of light cyan color.
#define YYCC_COLORHDR_LIGHT_CYAN "\033[96m"
	/// @brief The head of ASCII escape code of light white color.
#define YYCC_COLORHDR_LIGHT_WHITE "\033[97m"
	
	/// @brief The tail of ASCII escape code of every color.
#define YYCC_COLORTAIL "\033[0m"
	
	/// @brief The ASCII escape code pair of black color.
#define YYCC_COLOR_BLACK(T) "\033[30m" T "\033[0m"
	/// @brief The ASCII escape code pair of red color.
#define YYCC_COLOR_RED(T) "\033[31m" T "\033[0m"
	/// @brief The ASCII escape code pair of green color.
#define YYCC_COLOR_GREEN(T) "\033[32m" T "\033[0m"
	/// @brief The ASCII escape code pair of yellow color.
#define YYCC_COLOR_YELLOW(T) "\033[33m" T "\033[0m"
	/// @brief The ASCII escape code pair of blue color.
#define YYCC_COLOR_BLUE(T) "\033[34m" T "\033[0m"
	/// @brief The ASCII escape code pair of magenta color.
#define YYCC_COLOR_MAGENTA(T) "\033[35m" T "\033[0m"
	/// @brief The ASCII escape code pair of cyan color.
#define YYCC_COLOR_CYAN(T) "\033[36m" T "\033[0m"
	/// @brief The ASCII escape code pair of white color.
#define YYCC_COLOR_WHITE(T) "\033[37m" T "\033[0m"
	
	/// @brief The ASCII escape code pair of light black color.
#define YYCC_COLOR_LIGHT_BLACK(T) "\033[90m" T "\033[0m"
	/// @brief The ASCII escape code pair of light red color.
#define YYCC_COLOR_LIGHT_RED(T) "\033[91m" T "\033[0m"
	/// @brief The ASCII escape code pair of light green color.
#define YYCC_COLOR_LIGHT_GREEN(T) "\033[92m" T "\033[0m"
	/// @brief The ASCII escape code pair of light yellow color.
#define YYCC_COLOR_LIGHT_YELLOW(T) "\033[93m" T "\033[0m"
	/// @brief The ASCII escape code pair of light blue color.
#define YYCC_COLOR_LIGHT_BLUE(T) "\033[94m" T "\033[0m"
	/// @brief The ASCII escape code pair of light magenta color.
#define YYCC_COLOR_LIGHT_MAGENTA(T) "\033[95m" T "\033[0m"
	/// @brief The ASCII escape code pair of light cyan color.
#define YYCC_COLOR_LIGHT_CYAN(T) "\033[96m" T "\033[0m"
	/// @brief The ASCII escape code pair of light white color.
#define YYCC_COLOR_LIGHT_WHITE(T) "\033[97m" T "\033[0m"

	/**
	 * @brief Enable console color support for Windows.
	 * @details This actually is enable virtual console feature for \c stdout and \c stderr.
	 * @return True if success, otherwise false.
	 * @remarks 
	 * This function only works on Windows and do nothing on other platforms such as Linux,
	 * because we assume all terminals existing on other platform support color feature as default.
	*/
	bool EnableColorfulConsole();

	/**
	 * @brief Reads the next line of UTF8 characters from the standard input stream.
	 * @return
	 * The next line of UTF8 characters from the input stream.
	 * Empty string if user just press Enter key or function failed.
	*/
	yycc_u8string ReadLine();

	/**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void Format(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard output stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void FormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Writes the specified string value to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
	void Write(const yycc_char8_t* u8_strl);
	/**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard output stream.
	 * @param[in] u8_strl The value to write.
	*/
	void WriteLine(const yycc_char8_t* u8_strl);

	/**
	 * @brief 
	 * Writes the text representation of the specified object 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void ErrFormat(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief 
	 * Writes the text representation of the specified object, 
	 * followed by the current line terminator, 
	 * to the standard error stream using the specified format information.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments of format string.
	*/
	void ErrFormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Writes the specified string value to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
	void ErrWrite(const yycc_char8_t* u8_strl);
	/**
	 * @brief 
	 * Writes the specified string value, followed by the current line terminator, 
	 * to the standard error stream.
	 * @param[in] u8_strl The value to write.
	*/
	void ErrWriteLine(const yycc_char8_t* u8_strl);

}
