#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <string>

/**
 * @brief The namespace providing universal Console visiting functions like C-Sharp Console class.
 * @details
 * \ref console_helper
*/
namespace YYCC::ConsoleHelper {

#define YYCC_COLORHDR_BLACK "\033[30m"
#define YYCC_COLORHDR_RED "\033[31m"
#define YYCC_COLORHDR_GREEN "\033[32m"
#define YYCC_COLORHDR_YELLOW "\033[33m"
#define YYCC_COLORHDR_BLUE "\033[34m"
#define YYCC_COLORHDR_MAGENTA "\033[35m"
#define YYCC_COLORHDR_CYAN "\033[36m"
#define YYCC_COLORHDR_WHITE "\033[37m"

#define YYCC_COLORHDR_LIGHT_BLACK "\033[90m"
#define YYCC_COLORHDR_LIGHT_RED "\033[91m"
#define YYCC_COLORHDR_LIGHT_GREEN "\033[92m"
#define YYCC_COLORHDR_LIGHT_YELLOW "\033[93m"
#define YYCC_COLORHDR_LIGHT_BLUE "\033[94m"
#define YYCC_COLORHDR_LIGHT_MAGENTA "\033[95m"
#define YYCC_COLORHDR_LIGHT_CYAN "\033[96m"
#define YYCC_COLORHDR_LIGHT_WHITE "\033[97m"

#define YYCC_COLORTAIL "\033[0m"


#define YYCC_COLOR_BLACK(T) "\033[30m" T "\033[0m"
#define YYCC_COLOR_RED(T) "\033[31m" T "\033[0m"
#define YYCC_COLOR_GREEN(T) "\033[32m" T "\033[0m"
#define YYCC_COLOR_YELLOW(T) "\033[33m" T "\033[0m"
#define YYCC_COLOR_BLUE(T) "\033[34m" T "\033[0m"
#define YYCC_COLOR_MAGENTA(T) "\033[35m" T "\033[0m"
#define YYCC_COLOR_CYAN(T) "\033[36m" T "\033[0m"
#define YYCC_COLOR_WHITE(T) "\033[37m" T "\033[0m"

#define YYCC_COLOR_LIGHT_BLACK(T) "\033[90m" T "\033[0m"
#define YYCC_COLOR_LIGHT_RED(T) "\033[91m" T "\033[0m"
#define YYCC_COLOR_LIGHT_GREEN(T) "\033[92m" T "\033[0m"
#define YYCC_COLOR_LIGHT_YELLOW(T) "\033[93m" T "\033[0m"
#define YYCC_COLOR_LIGHT_BLUE(T) "\033[94m" T "\033[0m"
#define YYCC_COLOR_LIGHT_MAGENTA(T) "\033[95m" T "\033[0m"
#define YYCC_COLOR_LIGHT_CYAN(T) "\033[96m" T "\033[0m"
#define YYCC_COLOR_LIGHT_WHITE(T) "\033[97m" T "\033[0m"

	/**
	 * @brief Enable Windows console color support.
	 * @details This actually is enable virtual console feature for stdout and stderr.
	 * @return True if success, otherwise false.
	 * @remarks This function only works on Windows and do nothing on other platforms such as Linux,
	 * because we assume all terminals existing on other platform support color feature as default.
	*/
	bool EnableColorfulConsole();

	/**
	 * @brief Universal console read function
	 * @return
	 * The UTF8 encoded string this function read. EOL is excluded.
	 * Empty string if user just press Enter key or function failed.
	 * @remarks
	 * This function is more like C# Console.ReadLine().
	 * It read user input with UTF8 encoding until reaching EOL.
	 * \par
	 * This function also can be used as ordering user press Enter key by
	 * simply calling this function and ignoring its return value.
	*/
	yycc_u8string ReadLine();

	/**
	 * @brief Universal console write function with format feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void Format(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Universal console write function with format and auto EOL feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void FormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Universal console write function.
	 * @param[in] u8_strl The string to be written.
	*/
	void Write(const yycc_char8_t* u8_strl);
	/**
	 * @brief Universal console write function with auto EOL feature.
	 * @param[in] u8_strl The string to be written.
	*/
	void WriteLine(const yycc_char8_t* u8_strl);

	/**
	 * @brief Universal console error write function with format and feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void ErrFormat(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Universal console error write function with format and auto EOL feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void ErrFormatLine(const yycc_char8_t* u8_fmt, ...);
	/**
	 * @brief Universal console error write function.
	 * @param[in] u8_strl The string to be written.
	*/
	void ErrWrite(const yycc_char8_t* u8_strl);
	/**
	 * @brief Universal console error write function with auto EOL feature.
	 * @param[in] u8_strl The string to be written.
	*/
	void ErrWriteLine(const yycc_char8_t* u8_strl);

}
