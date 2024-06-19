#include <YYCCommonplace.hpp>
#include <cstdio>

namespace Console = YYCC::ConsoleHelper;

namespace YYCCTestbench {

#pragma region Unicode Test Data

	// UNICODE Test Strings
	// Ref: https://stackoverflow.com/questions/478201/how-to-test-an-application-for-correct-encoding-e-g-utf-8
#define TEST_UNICODE_STR_JAPAN "\u30E6\u30FC\u30B6\u30FC\u5225\u30B5\u30A4\u30C8"
#define TEST_UNICODE_STR_CHINA "\u7B80\u4F53\u4E2D\u6587"
#define TEST_UNICODE_STR_KOREA "\uD06C\uB85C\uC2A4 \uD50C\uB7AB\uD3FC\uC73C\uB85C"
#define TEST_UNICODE_STR_ISRAEL "\u05DE\u05D3\u05D5\u05E8\u05D9\u05DD \u05DE\u05D1\u05D5\u05E7\u05E9\u05D9\u05DD"
#define TEST_UNICODE_STR_EGYPT "\u0623\u0641\u0636\u0644 \u0627\u0644\u0628\u062D\u0648\u062B"
#define TEST_UNICODE_STR_GREECE "\u03A3\u1F72 \u03B3\u03BD\u03C9\u03C1\u03AF\u03B6\u03C9 \u1F00\u03C0\u1F78"
#define TEST_UNICODE_STR_RUSSIA "\u0414\u0435\u0441\u044F\u0442\u0443\u044E \u041C\u0435\u0436\u0434\u0443\u043D\u0430\u0440\u043E\u0434\u043D\u0443\u044E"
#define TEST_UNICODE_STR_THAILAND "\u0E41\u0E1C\u0E48\u0E19\u0E14\u0E34\u0E19\u0E2E\u0E31\u0E48\u0E19\u0E40\u0E2A\u0E37\u0E48\u0E2D\u0E21\u0E42\u0E17\u0E23\u0E21\u0E41\u0E2A\u0E19\u0E2A\u0E31\u0E07\u0E40\u0E27\u0E0A"
#define TEST_UNICODE_STR_FRANCE "fran\u00E7ais langue \u00E9trang\u00E8re"
#define TEST_UNICODE_STR_SPAIN "ma\u00F1ana ol\u00E9"
#define TEST_UNICODE_STR_MATHMATICS "\u222E E\u22C5da = Q,  n \u2192 \u221E, \u2211 f(i) = \u220F g(i)"
#define TEST_UNICODE_STR_EMOJI "\U0001F363 \u2716 \U0001F37A" // sushi x beer mug

#define CONCAT(prefix, strl) prefix ## strl
#define CPP_U8_LITERAL(strl) strl
#define CPP_U16_LITERAL(strl) CONCAT(u, strl)
#define CPP_U32_LITERAL(strl) CONCAT(U, strl)
#define CPP_WSTR_LITERAL(strl) CONCAT(L, strl)

	static std::vector<std::string> c_UTF8TestStrTable {
		CPP_U8_LITERAL(TEST_UNICODE_STR_JAPAN),
		CPP_U8_LITERAL(TEST_UNICODE_STR_CHINA),
		CPP_U8_LITERAL(TEST_UNICODE_STR_KOREA),
		CPP_U8_LITERAL(TEST_UNICODE_STR_ISRAEL),
		CPP_U8_LITERAL(TEST_UNICODE_STR_EGYPT),
		CPP_U8_LITERAL(TEST_UNICODE_STR_GREECE),
		CPP_U8_LITERAL(TEST_UNICODE_STR_RUSSIA),
		CPP_U8_LITERAL(TEST_UNICODE_STR_THAILAND),
		CPP_U8_LITERAL(TEST_UNICODE_STR_FRANCE),
		CPP_U8_LITERAL(TEST_UNICODE_STR_SPAIN),
		CPP_U8_LITERAL(TEST_UNICODE_STR_MATHMATICS),
		CPP_U8_LITERAL(TEST_UNICODE_STR_EMOJI),
	};
	static std::vector<std::wstring> c_WStrTestStrTable {
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_JAPAN),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_CHINA),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_KOREA),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_ISRAEL),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_EGYPT),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_GREECE),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_RUSSIA),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_THAILAND),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_FRANCE),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_SPAIN),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_MATHMATICS),
		CPP_WSTR_LITERAL(TEST_UNICODE_STR_EMOJI),
	};
	static std::vector<std::u16string> c_UTF16TestStrTable {
		CPP_U16_LITERAL(TEST_UNICODE_STR_JAPAN),
		CPP_U16_LITERAL(TEST_UNICODE_STR_CHINA),
		CPP_U16_LITERAL(TEST_UNICODE_STR_KOREA),
		CPP_U16_LITERAL(TEST_UNICODE_STR_ISRAEL),
		CPP_U16_LITERAL(TEST_UNICODE_STR_EGYPT),
		CPP_U16_LITERAL(TEST_UNICODE_STR_GREECE),
		CPP_U16_LITERAL(TEST_UNICODE_STR_RUSSIA),
		CPP_U16_LITERAL(TEST_UNICODE_STR_THAILAND),
		CPP_U16_LITERAL(TEST_UNICODE_STR_FRANCE),
		CPP_U16_LITERAL(TEST_UNICODE_STR_SPAIN),
		CPP_U16_LITERAL(TEST_UNICODE_STR_MATHMATICS),
		CPP_U16_LITERAL(TEST_UNICODE_STR_EMOJI),
	};
	static std::vector<std::u32string> c_UTF32TestStrTable {
		CPP_U32_LITERAL(TEST_UNICODE_STR_JAPAN),
		CPP_U32_LITERAL(TEST_UNICODE_STR_CHINA),
		CPP_U32_LITERAL(TEST_UNICODE_STR_KOREA),
		CPP_U32_LITERAL(TEST_UNICODE_STR_ISRAEL),
		CPP_U32_LITERAL(TEST_UNICODE_STR_EGYPT),
		CPP_U32_LITERAL(TEST_UNICODE_STR_GREECE),
		CPP_U32_LITERAL(TEST_UNICODE_STR_RUSSIA),
		CPP_U32_LITERAL(TEST_UNICODE_STR_THAILAND),
		CPP_U32_LITERAL(TEST_UNICODE_STR_FRANCE),
		CPP_U32_LITERAL(TEST_UNICODE_STR_SPAIN),
		CPP_U32_LITERAL(TEST_UNICODE_STR_MATHMATICS),
		CPP_U32_LITERAL(TEST_UNICODE_STR_EMOJI),
	};

#undef CPP_WSTR_LITERAL
#undef CPP_U32_LITERAL
#undef CPP_U16_LITERAL
#undef CPP_U8_LITERAL
#undef CONCAT

#pragma endregion

	static void Assert(bool condition, const char* description) {
		if (condition) {
			Console::FormatLine(YYCC_COLOR_LIGHT_GREEN("OK: %s"), description);
		} else {
			Console::FormatLine(YYCC_COLOR_LIGHT_RED("Failed: %s\n"), description);
			std::abort();
		}
	}

	static void ConsoleTestbench() {
		// Color Test
		Console::EnableColorfulConsole();
		Console::WriteLine("Color Test:");

#define TEST_MACRO(col) Console::WriteLine("\t" YYCC_COLOR_ ## col ("\u2588\u2588") YYCC_COLOR_LIGHT_ ## col("\u2588\u2588") " " #col " / LIGHT " #col );
		// U+2588 is full block

		TEST_MACRO(BLACK);
		TEST_MACRO(RED);
		TEST_MACRO(GREEN);
		TEST_MACRO(YELLOW);
		TEST_MACRO(BLUE);
		TEST_MACRO(MAGENTA);
		TEST_MACRO(CYAN);
		TEST_MACRO(WHITE);

#undef TEST_MACRO

		// UTF8 Output Test
		Console::WriteLine("UTF8 Output Test:");
		for (const auto& strl : c_UTF8TestStrTable) {
			Console::FormatLine("\t%s", strl.c_str());
		}

		// UTF8 Input Test
		Console::WriteLine("UTF8 Input Test:");
		for (const auto& strl : c_UTF8TestStrTable) {
			Console::FormatLine("\tPlease type: %s", strl.c_str());
			Console::Write("\t> ");

			std::string gotten(Console::ReadLine());
			Assert(gotten == strl, YYCC::StringHelper::Printf("Got: %s", gotten.c_str()).c_str());
		}

	}

	static void EncodingTestbench() {
		// get test tuple size
		size_t count = c_UTF8TestStrTable.size();

		// check the convertion between given string
		for (size_t i = 0u; i < count; ++i) {
			// get item
			const auto& u8str = c_UTF8TestStrTable[i];
			const auto& u16str = c_UTF16TestStrTable[i];
			const auto& u32str = c_UTF32TestStrTable[i];

			// create cache variables
			std::string u8cache;
			std::u16string u16cache;
			std::u32string u32cache;

			// do convertion check
			Assert(YYCC::EncodingHelper::UTF8ToUTF16(u8str.c_str(), u16cache) && u16cache == u16str, "YYCC::EncodingHelper::UTF8ToUTF16");
			Assert(YYCC::EncodingHelper::UTF8ToUTF32(u8str.c_str(), u32cache) && u32cache == u32str, "YYCC::EncodingHelper::UTF8ToUTF32");

			Assert(YYCC::EncodingHelper::UTF16ToUTF8(u16str.c_str(), u8cache) && u8cache == u8str, "YYCC::EncodingHelper::UTF16ToUTF8");
			Assert(YYCC::EncodingHelper::UTF32ToUTF8(u32str.c_str(), u8cache) && u8cache == u8str, "YYCC::EncodingHelper::UTF32ToUTF8");
		}

		// check wstring convertion on windows
#if YYCC_OS == YYCC_OS_WINDOWS
		for (size_t i = 0u; i < count; ++i) {
			// get item
			const auto& u8str = c_UTF8TestStrTable[i];
			const auto& wstr = c_WStrTestStrTable[i];

			// create cache variables
			std::string u8cache;
			std::wstring wcache;

			// do convertion check
			Assert(YYCC::EncodingHelper::UTF8ToWchar(u8str.c_str(), wcache) && wcache == wstr, "YYCC::EncodingHelper::UTF8ToWchar");
			Assert(YYCC::EncodingHelper::WcharToUTF8(wstr.c_str(), u8cache) && u8cache == u8str, "YYCC::EncodingHelper::WcharToUTF8");
		}
#endif

	}

	static void StringTestbench() {
		// Test Printf
		auto test_printf = YYCC::StringHelper::Printf("%s == %s", "Hello World", "Hello, world");
		Assert(test_printf == "Hello World == Hello, world", "YYCC::StringHelper::Printf");

		// Test Replace
		auto test_replace = YYCC::StringHelper::Replace("aabbcc", "bb", "dd"); // normal case
		Assert(test_replace == "aaddcc", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("aabbcc", "zz", "yy"); // no replace
		Assert(test_replace == "aabbcc", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("aabbcc", "", "zz"); // empty finding
		Assert(test_replace == "aabbcc", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("aabbcc", nullptr, "zz"); // nullptr finding
		Assert(test_replace == "aabbcc", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("aaaabbaa", "aa", ""); // no replaced string
		Assert(test_replace == "bb", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("aaxcc", "x", "yx"); // nested replacing
		Assert(test_replace == "aayxcc", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace("", "", "xy"); // empty source string
		Assert(test_replace == "", "YYCC::StringHelper::Replace");
		test_replace = YYCC::StringHelper::Replace(nullptr, "", "xy"); // nullptr source string
		Assert(test_replace == "", "YYCC::StringHelper::Replace");

		// Test Upper / Lower
		auto test_lower = YYCC::StringHelper::Lower("LOWER");
		Assert(test_lower == "lower", "YYCC::StringHelper::Lower");
		auto test_upper = YYCC::StringHelper::Upper("upper");
		Assert(test_upper == "UPPER", "YYCC::StringHelper::Upper");

		// Test Join
		std::vector<std::string> test_join_container {
			"", "1", "2", ""
		};
		auto test_join = YYCC::StringHelper::Join(test_join_container, ", ");
		Assert(test_join == ", 1, 2, ", "YYCC::StringHelper::Join");
		test_join = YYCC::StringHelper::Join(test_join_container, ", ", true);
		Assert(test_join == ", 2, 1, ", "YYCC::StringHelper::Join");

		// Test Split
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
#if YYCC_OS == YYCC_OS_WINDOWS

		std::string ret;
		std::vector<std::string> rets;

		YYCC::DialogHelper::FileDialog params;
		auto& filters = params.ConfigreFileTypes();
		filters.Add("Microsoft Word (*.docx; *.doc)", { "*.docx", "*.doc" });
		filters.Add("Microsoft Excel (*.xlsx; *.xls)", { "*.xlsx", "*.xls" });
		filters.Add("Microsoft PowerPoint (*.pptx; *.ppt)", { "*.pptx", "*.ppt" });
		filters.Add("Text File (*.txt)", { "*.txt" });
		filters.Add("All Files (*.*)", { "*.*" });
		params.SetDefaultFileTypeIndex(0u);
		if (YYCC::DialogHelper::OpenFileDialog(params, ret)) {
			Console::FormatLine("Open File: %s", ret.c_str());
		}
		if (YYCC::DialogHelper::OpenMultipleFileDialog(params, rets)) {
			Console::WriteLine("Open Multiple Files:");
			for (const auto& item : rets) {
				Console::FormatLine("\t%s", item.c_str());
			}
		}
		if (YYCC::DialogHelper::SaveFileDialog(params, ret)) {
			Console::FormatLine("Save File: %s", ret.c_str());
		}
		params.Clear();
		if (YYCC::DialogHelper::OpenFolderDialog(params, ret)) {
			Console::FormatLine("Open Folder: %s", ret.c_str());
		}

#endif
	}

	static void ExceptionTestbench() {
#if YYCC_OS == YYCC_OS_WINDOWS

		YYCC::ExceptionHelper::Register();

		// Perform a div zero exception.
		int i = 1, j = 0;
		int k = i / j;

		YYCC::ExceptionHelper::Unregister();

#endif
	}

	static void WinFctTestbench() {
#if YYCC_OS == YYCC_OS_WINDOWS

		Console::FormatLine("Current Module HANDLE: 0x%" PRI_XPTR_LEFT_PADDING PRIXPTR, YYCC::WinFctHelper::GetCurrentModule());

		std::string test_temp;
		Assert(YYCC::WinFctHelper::GetTempDirectory(test_temp), "YYCC::WinFctHelper::GetTempDirectory");
		Console::FormatLine("Temp Directory: %s", test_temp.c_str());

		std::string test_module_name;
		Assert(YYCC::WinFctHelper::GetModuleFileName(YYCC::WinFctHelper::GetCurrentModule(), test_module_name), "YYCC::WinFctHelper::GetModuleFileName");
		Console::FormatLine("Current Module File Name: %s", test_module_name.c_str());

		std::string test_localappdata_path;
		Assert(YYCC::WinFctHelper::GetLocalAppData(test_localappdata_path), "YYCC::WinFctHelper::GetLocalAppData");
		Console::FormatLine("Local AppData: %s", test_localappdata_path.c_str());

#endif
	}

	static void FsPathPatch() {

		std::filesystem::path test_path;
		for (const auto& strl : c_UTF8TestStrTable) {
			test_path /= YYCC::FsPathPatch::FromUTF8Path(strl.c_str());
		}
		std::string test_slashed_path(YYCC::FsPathPatch::ToUTF8Path(test_path));

#if YYCC_OS == YYCC_OS_WINDOWS
		std::wstring wdecilmer(1u, std::filesystem::path::preferred_separator);
		std::string decilmer(YYCC::EncodingHelper::WcharToUTF8(wdecilmer.c_str()));
#else
		std::string decilmer(1u, std::filesystem::path::preferred_separator);
#endif
		std::string test_joined_path(YYCC::StringHelper::Join(c_UTF8TestStrTable, decilmer.c_str()));

		Assert(test_slashed_path == test_joined_path, "YYCC::FsPathPatch");

	}

}

int main(int argc, char** args) {
	//YYCCTestbench::ConsoleTestbench();
	YYCCTestbench::EncodingTestbench();
	//YYCCTestbench::StringTestbench();
	//YYCCTestbench::ParserTestbench();
	//YYCCTestbench::DialogTestbench();
	//YYCCTestbench::ExceptionTestbench();
	//YYCCTestbench::WinFctTestbench();
	//YYCCTestbench::FsPathPatch();
}
