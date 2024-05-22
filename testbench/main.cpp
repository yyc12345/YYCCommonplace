#include <YYCCommonplace.hpp>
#include <cstdio>

namespace Testbench {

	static void Assert(bool condition, const char* description) {
		if (condition) {
			YYCC::TerminalHelper::FPrintf(stdout, YYCC_TERMCOL_LIGHT_GREEN(u8"OK: %s\n"), description);
		} else {
			YYCC::TerminalHelper::FPrintf(stdout, YYCC_TERMCOL_LIGHT_RED(u8"Failed: %s\n"), description);
			std::abort();
		}
	}

	static void TerminalTestbench() {
		YYCC::TerminalHelper::EnsureTerminalUTF8(stdout);
		YYCC::TerminalHelper::FPuts(u8"你好世界\n", stdout);
		YYCC::TerminalHelper::EnsureTerminalColor(stdout);
		YYCC::TerminalHelper::FPuts(YYCC_TERMCOL_LIGHT_CYAN(u8"Colorful Terminal\n"), stdout);
	}

	static void StringTestbench() {
		auto test_printf = YYCC::StringHelper::Printf(u8"%s == %s", u8"Hello World", u8"你好世界");
		Assert(test_printf == u8"Hello World == 你好世界", "YYCC::StringHelper::Printf");
			
		auto test_lower = YYCC::StringHelper::Lower("LOWER");
		Assert(test_lower == "lower", "YYCC::StringHelper::Lower");
		auto test_upper = YYCC::StringHelper::Upper("upper");
		Assert(test_upper == "UPPER", "YYCC::StringHelper::Upper");


		std::vector<std::string> test_join_container {
			"", "1", "2", ""
		};
		auto test_join = YYCC::StringHelper::Join(test_join_container, ", ");
		Assert(test_join == ", 1, 2, ", "YYCC::StringHelper::Join");
		test_join = YYCC::StringHelper::Join(test_join_container, ", ", true);
		Assert(test_join == ", 2, 1, ", "YYCC::StringHelper::Join");

		auto test_split = YYCC::StringHelper::Split(", 1, 2, ", ", ");
		Assert(test_split.size() == 4u, "YYCC::StringHelper::Split");
		Assert(test_split[0] == "", "YYCC::StringHelper::Split");
		Assert(test_split[1] == "1", "YYCC::StringHelper::Split");
		Assert(test_split[2] == "2", "YYCC::StringHelper::Split");
		Assert(test_split[3] == "", "YYCC::StringHelper::Split");
		test_split = YYCC::StringHelper::Split("test", "-");
		Assert(test_split.size() == 1u, "YYCC::StringHelper::Split");
		Assert(test_split[0] == "test", "YYCC::StringHelper::Split");
		test_split = YYCC::StringHelper::Split("test", "");
		Assert(test_split.size() == 1u, "YYCC::StringHelper::Split");
		Assert(test_split[0] == "test", "YYCC::StringHelper::Split");

	}

	static void DialogTestbench() {
		YYCC::DialogHelper::FileFilters test;
		test.Add("Microsoft Word (*.docx; *.doc)", {"*.docx", "*.doc"});
		test.Add("Microsoft Excel (*.xlsx; *.xls)", {"*.xlsx", "*.xls"});
		test.Add("Microsoft PowerPoint (*.pptx; *.ppt)", {"*.pptx", "*.ppt"});
		test.Add("Text File (*.*)", {"*.txt"});
		test.Add("All Files (*.*)", {"*.*"});

		UINT count;
		COMDLG_FILTERSPEC* specs;
		bool ret = test.Generate(count, specs);
	}

}

int main(int argc, char** args) {
	Testbench::TerminalTestbench();
	Testbench::StringTestbench();
	Testbench::DialogTestbench();
}
