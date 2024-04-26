#include "TerminalHelper.hpp"
#if YYCC_OS == YYCC_OS_WINDOWS

#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "WinImportSuffix.hpp"

namespace YYCC::TerminalHelper {

	bool ColorfulTerminal(FILE* fs) {
		if (!_isatty(_fileno(fs))) return false;

		HANDLE h_output;
		DWORD dw_mode;

		h_output = (HANDLE)_get_osfhandle(_fileno(fs));
		if (!GetConsoleMode(h_output, &dw_mode)) return false;
		if (!SetConsoleMode(h_output, dw_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return false;

		return true;
	}

	bool UTF8Terminal(FILE* fs) {
		if (!SetConsoleCP(CP_UTF8)) return false;
		if (!SetConsoleOutputCP(CP_UTF8)) return false;

		/*_setmode(_fileno(stdout), _O_U8TEXT);*/
		_setmode(_fileno(fs), _O_U16TEXT);
		return true;
	}

}

#endif
