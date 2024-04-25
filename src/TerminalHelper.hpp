#pragma once
#include "YYCCInternal.hpp"
#include <cstdio>

namespace YYCC::TerminalHelper {
	
#define UNVIRT_REMOVE_PARENS_IMPL(...) __VA_ARGS__
#define UNVIRT_REMOVE_PARENS(T) UNVIRT_REMOVE_PARENS_IMPL T


#define UNVIRT_TERMCOLHDR_BLACK "\033[30m"
#define UNVIRT_TERMCOLHDR_RED "\033[31m"
#define UNVIRT_TERMCOLHDR_GREEN "\033[32m"
#define UNVIRT_TERMCOLHDR_YELLOW "\033[33m"
#define UNVIRT_TERMCOLHDR_BLUE "\033[34m"
#define UNVIRT_TERMCOLHDR_MAGENTA "\033[35m"
#define UNVIRT_TERMCOLHDR_CYAN "\033[36m"
#define UNVIRT_TERMCOLHDR_WHITE "\033[37m"

#define UNVIRT_TERMCOLHDR_LIGHT_BLACK "\033[90m"
#define UNVIRT_TERMCOLHDR_LIGHT_RED "\033[91m"
#define UNVIRT_TERMCOLHDR_LIGHT_GREEN "\033[92m"
#define UNVIRT_TERMCOLHDR_LIGHT_YELLOW "\033[93m"
#define UNVIRT_TERMCOLHDR_LIGHT_BLUE "\033[94m"
#define UNVIRT_TERMCOLHDR_LIGHT_MAGENTA "\033[95m"
#define UNVIRT_TERMCOLHDR_LIGHT_CYAN "\033[96m"
#define UNVIRT_TERMCOLHDR_LIGHT_WHITE "\033[97m"

#define UNVIRT_TERMCOLTAIL "\033[0m"


#define UNVIRT_TERMCOL_BLACK(T) "\033[30m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_RED(T) "\033[31m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_GREEN(T) "\033[32m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_YELLOW(T) "\033[33m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_BLUE(T) "\033[34m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_MAGENTA(T) "\033[35m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_CYAN(T) "\033[36m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_WHITE(T) "\033[37m" UNVIRT_REMOVE_PARENS(T) "\033[0m"

#define UNVIRT_TERMCOL_LIGHT_BLACK(T) "\033[90m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_RED(T) "\033[91m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_GREEN(T) "\033[92m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_YELLOW(T) "\033[93m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_BLUE(T) "\033[94m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_MAGENTA(T) "\033[95m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_CYAN(T) "\033[96m" UNVIRT_REMOVE_PARENS(T) "\033[0m"
#define UNVIRT_TERMCOL_LIGHT_WHITE(T) "\033[97m" UNVIRT_REMOVE_PARENS(T) "\033[0m"

#if YYCC_OS == YYCC_OS_WINDOWS

	bool ColorfulTerminal(FILE* fs);
	bool UTF8Terminal(FILE* fs);
	
#endif

}
