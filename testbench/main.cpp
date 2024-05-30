#include <YYCCommonplace.hpp>
#include <cstdio>

namespace Testbench {

	static void Assert(bool condition, const char* description) {
		if (condition) {
			YYCC::TerminalHelper::FPrintf(stdout, YYCC_TERMCOL_LIGHT_GREEN("OK: %s\n"), description);
		} else {
			YYCC::TerminalHelper::FPrintf(stdout, YYCC_TERMCOL_LIGHT_RED("Failed: %s\n"), description);
			std::abort();
		}
	}

	static void TerminalTestbench() {
		YYCC::TerminalHelper::EnsureTerminalUTF8(stdout);
		YYCC::TerminalHelper::FPuts("你好世界\n", stdout);
		YYCC::TerminalHelper::EnsureTerminalColor(stdout);
		YYCC::TerminalHelper::FPuts(YYCC_TERMCOL_LIGHT_CYAN("Colorful Terminal\n"), stdout);
	}

	static void StringTestbench() {
		auto test_printf = YYCC::StringHelper::Printf("%s == %s", "Hello World", "你好世界");
		Assert(test_printf == "Hello World == 你好世界", "YYCC::StringHelper::Printf");
			
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

	static void ParserTestbench() {

		// Test success TryParse
#define TEST_MACRO(type_t, value, string_value) { \
	std::string cache_string(string_value); \
	type_t cache; \
	Assert(YYCC::ParserHelper::TryParse<type_t>(cache_string, cache) && cache == value, "YYCC::StringHelper::TryParse<" #type_t ">"); \
}
		
		TEST_MACRO(int8_t, INT8_C(-61), "-61");
		TEST_MACRO(uint8_t, UINT8_C(200), "200");
		TEST_MACRO(int16_t, INT16_C(6161), "6161");
		TEST_MACRO(uint16_t, UINT16_C(32800), "32800");
		TEST_MACRO(int32_t, INT32_C(61616161), "61616161");
		TEST_MACRO(uint32_t, UINT32_C(4294967293), "4294967293");
		TEST_MACRO(int64_t, INT64_C(616161616161), "616161616161");
		TEST_MACRO(uint64_t, UINT64_C(9223372036854775807), "9223372036854775807");
		TEST_MACRO(bool, true, "true");
		
#undef TEST_MACRO

		// Test failed TryParse
#define TEST_MACRO(type_t, value, string_value) { \
	std::string cache_string(string_value); \
	type_t cache; \
	Assert(!YYCC::ParserHelper::TryParse<type_t>(cache_string, cache), "YYCC::StringHelper::TryParse<" #type_t ">"); \
}
		
		TEST_MACRO(int8_t, INT8_C(-61), "6161");
		TEST_MACRO(uint8_t, UINT8_C(200), "32800");
		TEST_MACRO(int16_t, INT16_C(6161), "61616161");
		TEST_MACRO(uint16_t, UINT16_C(32800), "4294967293");
		TEST_MACRO(int32_t, INT32_C(61616161), "616161616161");
		TEST_MACRO(uint32_t, UINT32_C(4294967293), "9223372036854775807");
		TEST_MACRO(int64_t, INT64_C(616161616161), "616161616161616161616161");
		TEST_MACRO(uint64_t, UINT64_C(9223372036854775807), "92233720368547758079223372036854775807");
		TEST_MACRO(bool, true, "hello, world!");
		
#undef TEST_MACRO

		// Test ToString
#define TEST_MACRO(type_t, value, string_value) { \
	type_t cache = value; \
	std::string ret(YYCC::ParserHelper::ToString<type_t>(cache)); \
	Assert(ret == string_value, "YYCC::StringHelper::ToString<" #type_t ">"); \
}

		TEST_MACRO(int8_t, INT8_C(-61), "-61");
		TEST_MACRO(uint8_t, UINT8_C(200), "200");
		TEST_MACRO(int16_t, INT16_C(6161), "6161");
		TEST_MACRO(uint16_t, UINT16_C(32800), "32800");
		TEST_MACRO(int32_t, INT32_C(61616161), "61616161");
		TEST_MACRO(uint32_t, UINT32_C(4294967293), "4294967293");
		TEST_MACRO(int64_t, INT64_C(616161616161), "616161616161");
		TEST_MACRO(uint64_t, UINT64_C(9223372036854775807), "9223372036854775807");
		TEST_MACRO(bool, true, "true");
		//{
		//	bool cache = true;
		//	std::string ret(YYCC::ParserHelper::ToString<bool>(cache));
		//	Assert(ret == "true", "YYCC::StringHelper::ToString<bool>");
		//}

#undef TEST_MACRO
	}

	static void DialogTestbench() {
		std::string ret;
		std::vector<std::string> rets;

		YYCC::DialogHelper::FileDialog params;
		auto& filters = params.ConfigreFileTypes();
		filters.Add("Microsoft Word (*.docx; *.doc)", {"*.docx", "*.doc"});
		filters.Add("Microsoft Excel (*.xlsx; *.xls)", {"*.xlsx", "*.xls"});
		filters.Add("Microsoft PowerPoint (*.pptx; *.ppt)", {"*.pptx", "*.ppt"});
		filters.Add("Text File (*.txt)", {"*.txt"});
		filters.Add("All Files (*.*)", {"*.*"});
		params.SetDefaultFileTypeIndex(0u);
		if (YYCC::DialogHelper::OpenFileDialog(params, ret)) {
			YYCC::TerminalHelper::FPrintf(stdout, "Open File: %s\n", ret.c_str());
		}
		if (YYCC::DialogHelper::OpenMultipleFileDialog(params, rets)) {
			YYCC::TerminalHelper::FPuts("Open Multiple Files:\n", stdout);
			for (const auto& item : rets) {
				YYCC::TerminalHelper::FPrintf(stdout, "\t%s\n", item.c_str());
			}
		}
		if (YYCC::DialogHelper::SaveFileDialog(params, ret)) {
			YYCC::TerminalHelper::FPrintf(stdout, "Save File: %s\n", ret.c_str());
		}
		params.Clear();
		if (YYCC::DialogHelper::OpenFolderDialog(params, ret)) {
			YYCC::TerminalHelper::FPrintf(stdout, "Open Folder: %s\n", ret.c_str());
		}
	}

}

int main(int argc, char** args) {
	Testbench::TerminalTestbench();
	Testbench::StringTestbench();
	Testbench::ParserTestbench();
	//Testbench::DialogTestbench();
}
