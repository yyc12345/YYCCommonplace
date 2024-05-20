#pragma once
#include "YYCCInternal.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include <cstdio>
#include <string>

namespace YYCC::TerminalHelper {
	
#define YYCC_TERMCOLHDR_BLACK "\033[30m"
#define YYCC_TERMCOLHDR_RED "\033[31m"
#define YYCC_TERMCOLHDR_GREEN "\033[32m"
#define YYCC_TERMCOLHDR_YELLOW "\033[33m"
#define YYCC_TERMCOLHDR_BLUE "\033[34m"
#define YYCC_TERMCOLHDR_MAGENTA "\033[35m"
#define YYCC_TERMCOLHDR_CYAN "\033[36m"
#define YYCC_TERMCOLHDR_WHITE "\033[37m"

#define YYCC_TERMCOLHDR_LIGHT_BLACK "\033[90m"
#define YYCC_TERMCOLHDR_LIGHT_RED "\033[91m"
#define YYCC_TERMCOLHDR_LIGHT_GREEN "\033[92m"
#define YYCC_TERMCOLHDR_LIGHT_YELLOW "\033[93m"
#define YYCC_TERMCOLHDR_LIGHT_BLUE "\033[94m"
#define YYCC_TERMCOLHDR_LIGHT_MAGENTA "\033[95m"
#define YYCC_TERMCOLHDR_LIGHT_CYAN "\033[96m"
#define YYCC_TERMCOLHDR_LIGHT_WHITE "\033[97m"

#define YYCC_TERMCOLTAIL "\033[0m"


#define YYCC_TERMCOL_BLACK(T) "\033[30m" T "\033[0m"
#define YYCC_TERMCOL_RED(T) "\033[31m" T "\033[0m"
#define YYCC_TERMCOL_GREEN(T) "\033[32m" T "\033[0m"
#define YYCC_TERMCOL_YELLOW(T) "\033[33m" T "\033[0m"
#define YYCC_TERMCOL_BLUE(T) "\033[34m" T "\033[0m"
#define YYCC_TERMCOL_MAGENTA(T) "\033[35m" T "\033[0m"
#define YYCC_TERMCOL_CYAN(T) "\033[36m" T "\033[0m"
#define YYCC_TERMCOL_WHITE(T) "\033[37m" T "\033[0m"

#define YYCC_TERMCOL_LIGHT_BLACK(T) "\033[90m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_RED(T) "\033[91m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_GREEN(T) "\033[92m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_YELLOW(T) "\033[93m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_BLUE(T) "\033[94m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_MAGENTA(T) "\033[95m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_CYAN(T) "\033[96m" T "\033[0m"
#define YYCC_TERMCOL_LIGHT_WHITE(T) "\033[97m" T "\033[0m"

	/**
	 * @brief Try letting terminal support ASCII color schema.
	 * @param fs[in] The stream to be set.
	 * @return true if success, otherwise false.
	*/
	bool EnsureTerminalColor(FILE* fs);
	/**
	 * @brief Try setting terminal to UTF8 encoding.
	 * @param fs[in] The stream to be set.
	 * @return true if success, otherwise false.
	 * @remarks If you enable UTF8 for a stream, you must use stream functions provided in this namespace to operate that stream,
	 * because after UTF8 modification, some old standard functions are not work and it may takes a little bit performance reduction.
	*/
	bool EnsureTerminalUTF8(FILE* fs);
	
	bool FGets(std::string& u8_buf, FILE* stream);
	void FPuts(const char* u8_buf, FILE* stream);
	void FPrintf(FILE* stream, const char* u8_fmt, ...);
}

#endif