#include <YYCCommonplace.hpp>
#include <cstdio>

namespace Testbench {

	static void TerminalTestbench() {
		YYCC::TerminalHelper::EnsureTerminalUTF8(stdout);
		YYCC::TerminalHelper::FPuts(u8"你好世界\n", stdout);
		YYCC::TerminalHelper::EnsureTerminalColor(stdout);
		YYCC::TerminalHelper::FPuts(YYCC_TERMCOL_LIGHT_CYAN(u8"Colorful Terminal\n"), stdout);
	}

	static void StringTestbench() {
		YYCC::TerminalHelper::FPuts(
			YYCC::StringHelper::Printf(u8"Translation: %s == %s\n", u8"Hello World", u8"你好世界").c_str(),
			stdout
		);
	}

}

int main(int argc, char** args) {
	Testbench::TerminalTestbench();
	Testbench::StringTestbench();
}
