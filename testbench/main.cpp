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

		std::string test_string(u8"UPPER -> lower\n");
		YYCC::TerminalHelper::FPuts(test_string.c_str(), stdout);
		YYCC::TerminalHelper::FPuts(YYCC::StringHelper::Lower(test_string.c_str()).c_str(), stdout);
		YYCC::TerminalHelper::FPuts(YYCC::StringHelper::Upper(test_string.c_str()).c_str(), stdout);
		
		std::vector<std::string> test_container {
			"test1", "test2", "test3", "test4"
		};
		YYCC::TerminalHelper::FPuts(YYCC::StringHelper::Join(test_container, ", ").c_str(), stdout);
		std::string test_split(", 1, 3, 5, 7, 9, ");
		YYCC::TerminalHelper::FPuts(YYCC::StringHelper::Join(YYCC::StringHelper::Split(test_split.c_str(), ", "), " -> ").c_str(), stdout);
	}

}

int main(int argc, char** args) {
	Testbench::TerminalTestbench();
	Testbench::StringTestbench();
}
