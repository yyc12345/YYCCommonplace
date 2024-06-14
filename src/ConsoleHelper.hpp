#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <string>

/**
 * @brief The namespace providing universal Console visiting functions like C-Sharp Console class.
 * @details
 * \par Why this Namespace
 * Windows console doesn't support UTF8 very well.
 * The standard input output functions can not work properly on Windows with UTF8.
 * So we create this namespace and provide various console-related functions
 * to patch Windows console and let it more like the console in other platforms.
 * \par
 * The function provided in this function can be called in any platforms.
 * In Windows, the implementation will use Windows native function,
 * and in other platform, the implementation will redirect request to standard C function
 * like std::fputs and etc.
 * So the programmer do not need to be worried about which function should they use,
 * and don't need to use macro to use different IO function in different platforms.
 * It is just enough that fully use the functions provided in this namespace.
 * \par
 * All IO functions this namespace provided are UTF8-based.
 * It also means that input output string should always be UTF8 encoded.
 * 
 * \par Input Functions
 * Please note that EOL will automatically converted into LF on Windows platform, not CRLF.
 * This action actually is removing all CR chars in result string.
 * This behavior affect nothing in most cases but it still is possible break something in some special case.
 * \par
 * Due to implementation, if you decide to use this function,
 * you should give up using any other function to read stdin stream,
 * such as std::gets() and std::cin.
 * Because this function may read chars which is more than needed.
 * These extra chars will be stored in this function and can be used next calling.
 * But these chars can not be visited by stdin again.
 * This behavior may cause bug.
 * So if you decide using this function, stick on it and do not change.
 * \par
 * Due to implementation, this function do not support hot switch of stdin.
 * It means that stdin can be redirected before first calling of this function,
 * but it should not be redirected during program running.
 * The reason is the same one introduced above.
 * 
 * \par Output Functions
 * In current implementation, EOL will not be converted automatically to CRLF.
 * This is different with other stream read functions provided in this namespace.
 * \par
 * Comparing with other stream read functions provided in this namespace,
 * stream write function support hot switch of stdout and stderr.
 * Because they do not have internal buffer storing something.
 * \par
 * In this namespace, there are various stream write function.
 * There is a list telling you how to choose one from them for using:
 * \li Functions with leading "Err" will write data into stderr,
 * otherwise they will write data into stdout.
 * \li Functions with embedded "Format" are output functions with format feature
 * like std::fprintf(), otherwise the functions with embedded "Write" will
 * only write plain string like std::fputs().
 * \li Functions with trailing "Line" will write extra EOL to break current line.
 * This is commonly used, otherwise functions will only write the text provided by arguments,
 * without adding something.
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
	std::string ReadLine();

	/**
	 * @brief Universal console write function with format feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void Format(const char* u8_fmt, ...);
	/**
	 * @brief Universal console write function with format and auto EOL feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void FormatLine(const char* u8_fmt, ...);
	/**
	 * @brief Universal console write function.
	 * @param[in] u8_strl The string to be written.
	*/
	void Write(const char* u8_strl);
	/**
	 * @brief Universal console write function with auto EOL feature.
	 * @param[in] u8_strl The string to be written.
	*/
	void WriteLine(const char* u8_strl);

	/**
	 * @brief Universal console error write function with format and feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void ErrFormat(const char* u8_fmt, ...);
	/**
	 * @brief Universal console error write function with format and auto EOL feature.
	 * @param[in] u8_fmt The format string.
	 * @param[in] ... The arguments to be formatted.
	*/
	void ErrFormatLine(const char* u8_fmt, ...);
	/**
	 * @brief Universal console error write function.
	 * @param[in] u8_strl The string to be written.
	*/
	void ErrWrite(const char* u8_strl);
	/**
	 * @brief Universal console error write function with auto EOL feature.
	 * @param[in] u8_strl The string to be written.
	*/
	void ErrWriteLine(const char* u8_strl);

}
