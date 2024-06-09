#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

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
	 * @brief Try letting terminal support ASCII color schema.
	 * @param fs[in] The stream to be set.
	 * @return true if success, otherwise false.
	*/
	bool EnableColorfulConsole(FILE* fs);
	
	/*
	Reference:
	* https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
	* https://stackoverflow.com/questions/69830460/reading-utf-8-input

	There is 3 way to make Windows console enable UTF8 mode.

	First one is calling SetConsoleCP and SetConsoleOutputCP.
	The side effect of this is std::cin and std::cout is broken,
	however there is a patch for this issue.

	Second one is calling _set_mode with _O_U8TEXT or _O_U16TEXT to enable Unicode mode for Windows console.
	This also have side effect which is stronger than first one.
	All puts family functions (ASCII-based output functions) will throw assertion exception.
	You only can use putws family functions (wide-char-based output functions).
	However these functions can not be used without calling _set_mode in Windows design.

	There still is another method, using WriteConsoleW directly visiting console.
	This function family can output correct string without calling any extra functions!
	This method is what we adopted.
	*/

	/**
	 * @brief 
	 * @return 
	*/
	std::string ReadLine();
	/**
	 * @brief 
	 * @param u8_fmt The format, or a simple string (format related chars still need escape).
	 * @param ...[in] The parameter for formatter.
	*/
	void Write(const char* u8_fmt, ...);
	/**
	 * @brief 
	 * @param u8_fmt 
	 * @param  
	*/
	void WriteLine(const char* u8_fmt, ...);

}

#endif