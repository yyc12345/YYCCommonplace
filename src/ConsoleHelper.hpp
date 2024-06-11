#pragma once
#include "YYCCInternal.hpp"

#include <cstdio>
#include <string>

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
	 * @details This function is more like C# Console.ReadLine().
	 * It read user input with UTF8 encoding until reaching EOL.
	 * 
	 * This function provide an universal, platform-independent way to read UTF8 string from console, 
	 * no matter whether it is redirected.
	 * @return The UTF8 encoded string this function read. EOL is excluded.
	 * @remarks In Windows, this function will try use native Win32 function for reading,
	 * because standard C/C++ function can not handle UTF8 input on Windows normally.
	 * In other platforms, this function will redirect request to std::readline with std::cin,
	 * which is all programmer commonly used method.
	 * It also mean that we assume stdin is encoded by UTF8 on these platforms.
	 * 
	 * Please note that EOL will automatically converted into LF on Windows platform, not CRLF.
	 * This action is actually remove all CR chars in result string.
	 * 
	 * This function also can be used as ordering user press Enter key by
	 * simply calling this function and ignoring its return value.
	*/
	std::string ReadLine();
	/**
	 * @brief Universal console write function
	 * @details This function is more like C# Console.Write().
	 * It write user given UTF8 string into console.
	 * 
	 * This function provide an universal, platform-independent way to write UTF8 string into console, 
	 * no matter whether it is redirected.
	 * @param u8_fmt[in] The format string.
	 * If you just want to write a pure string, you should escape formatter chars (%) in this string,
	 * because this function always take this parameter as a format string.
	 * @param ...[in] The arguments to be formatted.
	 * @remarks In Windows, this funcion will use native Win32 function for writing,
	 * because standard C/C++ function can not handle UTF8 output on Windows normally.
	 * In other platforms, this function will redirect request to std::fprintf with stdout,
	 * which is all programmer commonly used method.
	 * It also mean that we assume stdout is encoded by UTF8 on these platforms.
	*/
	void Write(const char* u8_fmt, ...);
	/**
	 * @brief Universal console write function with automatic EOL
	 * @details This function is same as Write(const char*, ...), 
	 * but it will automatically add EOL in output to break line.
	 * This is commonly used.
	 * @param u8_fmt[in] The format string.
	 * If you just want to write a pure string, you should escape formatter chars (%) in this string,
	 * because this function always take this parameter as a format string.
	 * If you want to output plain string, you need escape format 
	 * @param ...[in] The arguments to be formatted.
	*/
	void WriteLine(const char* u8_fmt, ...);

}
