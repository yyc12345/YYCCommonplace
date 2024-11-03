#include <YYCCommonplace.hpp>
#include <cstdio>
#include <set>
#include <map>

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
#define CPP_U8_LITERAL(strl) YYCC_U8(strl)
#define CPP_U16_LITERAL(strl) CONCAT(u, strl)
#define CPP_U32_LITERAL(strl) CONCAT(U, strl)
#define CPP_WSTR_LITERAL(strl) CONCAT(L, strl)

	static std::vector<YYCC::yycc_u8string> c_UTF8TestStrTable {
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

	static void Assert(bool condition, const YYCC::yycc_char8_t* description) {
		if (condition) {
			Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_GREEN("OK: %s")), description);
		} else {
			Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_RED("Failed: %s\n")), description);
			std::abort();
		}
	}

	static void ConsoleTestbench() {
		// Color Test
		Console::EnableColorfulConsole();
		Console::WriteLine(YYCC_U8("Color Test:"));

#define TEST_MACRO(col) Console::WriteLine(YYCC_U8("\t" YYCC_COLOR_ ## col ("\u2588\u2588") YYCC_COLOR_LIGHT_ ## col("\u2588\u2588") " " #col " / LIGHT " #col ));
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
		Console::WriteLine(YYCC_U8("UTF8 Output Test:"));
		for (const auto& strl : c_UTF8TestStrTable) {
			Console::FormatLine(YYCC_U8("\t%s"), strl.c_str());
		}

		// UTF8 Input Test
		Console::WriteLine(YYCC_U8("UTF8 Input Test:"));
		for (const auto& strl : c_UTF8TestStrTable) {
			Console::FormatLine(YYCC_U8("\tPlease type: %s"), strl.c_str());
			Console::Write(YYCC_U8("\t> "));

			YYCC::yycc_u8string gotten(Console::ReadLine());
			if (gotten == strl) Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_GREEN("\tMatched! Got: %s")), gotten.c_str());
			else Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_RED("\tNOT Matched! Got: %s")), gotten.c_str());
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
			YYCC::yycc_u8string u8cache;
			std::u16string u16cache;
			std::u32string u32cache;

			// do convertion check
			Assert(YYCC::EncodingHelper::UTF8ToUTF16(u8str, u16cache) && u16cache == u16str, YYCC_U8("YYCC::EncodingHelper::UTF8ToUTF16"));
			Assert(YYCC::EncodingHelper::UTF8ToUTF32(u8str, u32cache) && u32cache == u32str, YYCC_U8("YYCC::EncodingHelper::UTF8ToUTF32"));

			Assert(YYCC::EncodingHelper::UTF16ToUTF8(u16str, u8cache) && u8cache == u8str, YYCC_U8("YYCC::EncodingHelper::UTF16ToUTF8"));
			Assert(YYCC::EncodingHelper::UTF32ToUTF8(u32str, u8cache) && u8cache == u8str, YYCC_U8("YYCC::EncodingHelper::UTF32ToUTF8"));
		}

		// check wstring convertion on windows
#if YYCC_OS == YYCC_OS_WINDOWS
		for (size_t i = 0u; i < count; ++i) {
			// get item
			const auto& u8str = c_UTF8TestStrTable[i];
			const auto& wstr = c_WStrTestStrTable[i];

			// create cache variables
			YYCC::yycc_u8string u8cache;
			std::wstring wcache;

			// do convertion check
			Assert(YYCC::EncodingHelper::UTF8ToWchar(u8str.c_str(), wcache) && wcache == wstr, YYCC_U8("YYCC::EncodingHelper::UTF8ToWchar"));
			Assert(YYCC::EncodingHelper::WcharToUTF8(wstr.c_str(), u8cache) && u8cache == u8str, YYCC_U8("YYCC::EncodingHelper::WcharToUTF8"));
		}
#endif

	}

	static void StringTestbench() {
		// Test Printf
		auto test_printf = YYCC::StringHelper::Printf(YYCC_U8("%s == %s"), YYCC_U8("Hello World"), YYCC_U8("Hello, world"));
		Assert(test_printf == YYCC_U8("Hello World == Hello, world"), YYCC_U8("YYCC::StringHelper::Printf"));

		// Test Replace
		auto test_replace = YYCC::StringHelper::Replace(YYCC_U8("aabbcc"), YYCC_U8("bb"), YYCC_U8("dd")); // normal case
		Assert(test_replace == YYCC_U8("aaddcc"), YYCC_U8("YYCC::StringHelper::Replace"));
		test_replace = YYCC::StringHelper::Replace(YYCC_U8("aabbcc"), YYCC_U8("zz"), YYCC_U8("yy")); // no replace
		Assert(test_replace == YYCC_U8("aabbcc"), YYCC_U8("YYCC::StringHelper::Replace"));
		test_replace = YYCC::StringHelper::Replace(YYCC_U8("aabbcc"), YYCC::yycc_u8string_view(), YYCC_U8("zz")); // empty finding
		Assert(test_replace == YYCC_U8("aabbcc"), YYCC_U8("YYCC::StringHelper::Replace"));
		test_replace = YYCC::StringHelper::Replace(YYCC_U8("aaaabbaa"), YYCC_U8("aa"), YYCC_U8("")); // no replaced string
		Assert(test_replace == YYCC_U8("bb"), YYCC_U8("YYCC::StringHelper::Replace"));
		test_replace = YYCC::StringHelper::Replace(YYCC_U8("aaxcc"), YYCC_U8("x"), YYCC_U8("yx")); // nested replacing
		Assert(test_replace == YYCC_U8("aayxcc"), YYCC_U8("YYCC::StringHelper::Replace"));
		test_replace = YYCC::StringHelper::Replace(YYCC::yycc_u8string_view(), YYCC_U8(""), YYCC_U8("xy")); // empty source string
		Assert(test_replace == YYCC_U8(""), YYCC_U8("YYCC::StringHelper::Replace"));

		// Test Upper / Lower
		auto test_lower = YYCC::StringHelper::Lower(YYCC_U8("LOWER"));
		Assert(test_lower == YYCC_U8("lower"), YYCC_U8("YYCC::StringHelper::Lower"));
		auto test_upper = YYCC::StringHelper::Upper(YYCC_U8("upper"));
		Assert(test_upper == YYCC_U8("UPPER"), YYCC_U8("YYCC::StringHelper::Upper"));

		// Test Join
		std::vector<YYCC::yycc_u8string> test_join_container {
			YYCC_U8(""), YYCC_U8("1"), YYCC_U8("2"), YYCC_U8("")
		};
		auto test_join = YYCC::StringHelper::Join(test_join_container.begin(), test_join_container.end(), YYCC_U8(", "));
		Assert(test_join == YYCC_U8(", 1, 2, "), YYCC_U8("YYCC::StringHelper::Join"));

		// Test Split
		auto test_split = YYCC::StringHelper::Split(YYCC_U8(", 1, 2, "), YYCC_U8(", ")); // normal
		Assert(test_split.size() == 4u, YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[0] == YYCC_U8(""), YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[1] == YYCC_U8("1"), YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[2] == YYCC_U8("2"), YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[3] == YYCC_U8(""), YYCC_U8("YYCC::StringHelper::Split"));
		test_split = YYCC::StringHelper::Split(YYCC_U8("test"), YYCC_U8("-")); // no matched decilmer
		Assert(test_split.size() == 1u, YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[0] == YYCC_U8("test"), YYCC_U8("YYCC::StringHelper::Split"));
		test_split = YYCC::StringHelper::Split(YYCC_U8("test"), YYCC::yycc_u8string_view()); // empty decilmer
		Assert(test_split.size() == 1u, YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[0] == YYCC_U8("test"), YYCC_U8("YYCC::StringHelper::Split"));
		test_split = YYCC::StringHelper::Split(YYCC::yycc_u8string_view(), YYCC_U8("")); // empty source string
		Assert(test_split.size() == 1u, YYCC_U8("YYCC::StringHelper::Split"));
		Assert(test_split[0].empty(), YYCC_U8("YYCC::StringHelper::Split"));

	}

	static void ParserTestbench() {

		// Test success TryParse
#define TEST_MACRO(type_t, value, string_value, ...) { \
	YYCC::yycc_u8string cache_string(YYCC_U8(string_value)); \
	type_t cache; \
	Assert(YYCC::ParserHelper::TryParse<type_t>(cache_string, cache, ##__VA_ARGS__) && cache == value, YYCC_U8("YYCC::StringHelper::TryParse<" #type_t ">")); \
}
		
		TEST_MACRO(int8_t, INT8_C(-61), "-61");
		TEST_MACRO(uint8_t, UINT8_C(200), "200");
		TEST_MACRO(int16_t, INT16_C(6161), "6161");
		TEST_MACRO(uint16_t, UINT16_C(32800), "32800");
		TEST_MACRO(int32_t, INT32_C(61616161), "61616161");
		TEST_MACRO(uint32_t, UINT32_C(4294967293), "4294967293");
		TEST_MACRO(int64_t, INT64_C(616161616161), "616161616161");
		TEST_MACRO(uint64_t, UINT64_C(9223372036854775807), "9223372036854775807");
		TEST_MACRO(uint32_t, UINT32_C(0xffff), "ffff", 16);
		TEST_MACRO(float, 1.0f, "1.0");
		TEST_MACRO(double, 1.0, "1.0");
		TEST_MACRO(bool, true, "true");

#undef TEST_MACRO

		// Test failed TryParse
#define TEST_MACRO(type_t, string_value, ...) { \
	YYCC::yycc_u8string cache_string(YYCC_U8(string_value)); \
	type_t cache; \
	Assert(!YYCC::ParserHelper::TryParse<type_t>(cache_string, cache, ##__VA_ARGS__), YYCC_U8("YYCC::StringHelper::TryParse<" #type_t ">")); \
}

		TEST_MACRO(int8_t, "6161");
		TEST_MACRO(uint8_t, "32800");
		TEST_MACRO(int16_t, "61616161");
		TEST_MACRO(uint16_t, "4294967293");
		TEST_MACRO(int32_t, "616161616161");
		TEST_MACRO(uint32_t, "9223372036854775807");
		TEST_MACRO(int64_t, "616161616161616161616161");
		TEST_MACRO(uint64_t, "92233720368547758079223372036854775807");
		TEST_MACRO(float, "1e40");
		TEST_MACRO(double, "1e114514");
		TEST_MACRO(bool, "hello, world!");

#undef TEST_MACRO

		// Test ToString
#define TEST_MACRO(type_t, value, string_value, ...) { \
	type_t cache = value; \
	YYCC::yycc_u8string ret(YYCC::ParserHelper::ToString<type_t>(cache, ##__VA_ARGS__)); \
	Assert(ret == YYCC_U8(string_value), YYCC_U8("YYCC::StringHelper::ToString<" #type_t ">")); \
}

		TEST_MACRO(int8_t, INT8_C(-61), "-61");
		TEST_MACRO(uint8_t, UINT8_C(200), "200");
		TEST_MACRO(int16_t, INT16_C(6161), "6161");
		TEST_MACRO(uint16_t, UINT16_C(32800), "32800");
		TEST_MACRO(int32_t, INT32_C(61616161), "61616161");
		TEST_MACRO(uint32_t, UINT32_C(4294967293), "4294967293");
		TEST_MACRO(int64_t, INT64_C(616161616161), "616161616161");
		TEST_MACRO(uint64_t, UINT64_C(9223372036854775807), "9223372036854775807");
		TEST_MACRO(uint32_t, UINT32_C(0xffff), "ffff", 16);
		TEST_MACRO(float, 1.0f, "1.0", std::chars_format::fixed, 1);
		TEST_MACRO(double, 1.0, "1.0", std::chars_format::fixed, 1);
		TEST_MACRO(bool, true, "true");

#undef TEST_MACRO

	}

	static void DialogTestbench() {
#if YYCC_OS == YYCC_OS_WINDOWS

		YYCC::yycc_u8string ret;
		std::vector<YYCC::yycc_u8string> rets;

		YYCC::DialogHelper::FileDialog params;
		auto& filters = params.ConfigreFileTypes();
		filters.Add(YYCC_U8("Microsoft Word (*.docx; *.doc)"), { YYCC_U8("*.docx"), YYCC_U8("*.doc") });
		filters.Add(YYCC_U8("Microsoft Excel (*.xlsx; *.xls)"), { YYCC_U8("*.xlsx"), YYCC_U8("*.xls") });
		filters.Add(YYCC_U8("Microsoft PowerPoint (*.pptx; *.ppt)"), { YYCC_U8("*.pptx"), YYCC_U8("*.ppt") });
		filters.Add(YYCC_U8("Text File (*.txt)"), { YYCC_U8("*.txt") });
		filters.Add(YYCC_U8("All Files (*.*)"), { YYCC_U8("*.*") });
		params.SetDefaultFileTypeIndex(0u);
		if (YYCC::DialogHelper::OpenFileDialog(params, ret)) {
			Console::FormatLine(YYCC_U8("Open File: %s"), ret.c_str());
		}
		if (YYCC::DialogHelper::OpenMultipleFileDialog(params, rets)) {
			Console::WriteLine(YYCC_U8("Open Multiple Files:"));
			for (const auto& item : rets) {
				Console::FormatLine(YYCC_U8("\t%s"), item.c_str());
			}
		}
		if (YYCC::DialogHelper::SaveFileDialog(params, ret)) {
			Console::FormatLine(YYCC_U8("Save File: %s"), ret.c_str());
		}
		params.Clear();
		if (YYCC::DialogHelper::OpenFolderDialog(params, ret)) {
			Console::FormatLine(YYCC_U8("Open Folder: %s"), ret.c_str());
		}

#endif
	}

	static void ExceptionTestbench() {
#if YYCC_OS == YYCC_OS_WINDOWS

		YYCC::ExceptionHelper::Register([](const YYCC::yycc_u8string& log_path, const YYCC::yycc_u8string& coredump_path) -> void {
			MessageBoxW(
				NULL,
				YYCC::EncodingHelper::UTF8ToWchar(
				YYCC::StringHelper::Printf(YYCC_U8("Log generated:\nLog path: %s\nCore dump path: %s"), log_path.c_str(), coredump_path.c_str())
			).c_str(),
				L"Fatal Error", MB_OK + MB_ICONERROR
			);
			}
		);

		// Perform a div zero exception.
#if defined (YYCC_DEBUG_UE_FILTER)
		// Reference: https://stackoverflow.com/questions/20981982/is-it-possible-to-debug-unhandledexceptionfilters-with-a-debugger
		__try {
			// all of code normally inside of main or WinMain here...
			int i = 1, j = 0;
			int k = i / j;
		} __except (YYCC::ExceptionHelper::DebugCallUExceptionImpl(GetExceptionInformation())) {
			OutputDebugStringW(L"executed filter function\n");
		}
#else
		int i = 1, j = 0;
		int k = i / j;
#endif

		YYCC::ExceptionHelper::Unregister();

#endif
	}

	static void WinFctTestbench() {
#if YYCC_OS == YYCC_OS_WINDOWS

		HMODULE test_current_module;
		Assert((test_current_module = YYCC::WinFctHelper::GetCurrentModule()) != nullptr, YYCC_U8("YYCC::WinFctHelper::GetCurrentModule"));
		Console::FormatLine(YYCC_U8("Current Module HANDLE: 0x%" PRI_XPTR_LEFT_PADDING PRIXPTR), test_current_module);

		YYCC::yycc_u8string test_temp;
		Assert(YYCC::WinFctHelper::GetTempDirectory(test_temp), YYCC_U8("YYCC::WinFctHelper::GetTempDirectory"));
		Console::FormatLine(YYCC_U8("Temp Directory: %s"), test_temp.c_str());

		YYCC::yycc_u8string test_module_name;
		Assert(YYCC::WinFctHelper::GetModuleFileName(YYCC::WinFctHelper::GetCurrentModule(), test_module_name), YYCC_U8("YYCC::WinFctHelper::GetModuleFileName"));
		Console::FormatLine(YYCC_U8("Current Module File Name: %s"), test_module_name.c_str());

		YYCC::yycc_u8string test_localappdata_path;
		Assert(YYCC::WinFctHelper::GetLocalAppData(test_localappdata_path), YYCC_U8("YYCC::WinFctHelper::GetLocalAppData"));
		Console::FormatLine(YYCC_U8("Local AppData: %s"), test_localappdata_path.c_str());

		Assert(YYCC::WinFctHelper::IsValidCodePage(static_cast<UINT>(1252)), YYCC_U8("YYCC::WinFctHelper::IsValidCodePage"));
		Assert(!YYCC::WinFctHelper::IsValidCodePage(static_cast<UINT>(114514)), YYCC_U8("YYCC::WinFctHelper::IsValidCodePage"));

		// MARK: There is no testbench for MoveFile, CopyFile DeleteFile.
		// Because they can operate file system files.
		// And may cause test environment entering unstable status.

#endif
	}

	static void StdPatchTestbench() {

		// Std Path

		std::filesystem::path test_path;
		for (const auto& strl : c_UTF8TestStrTable) {
			test_path /= YYCC::StdPatch::ToStdPath(strl);
		}
		YYCC::yycc_u8string test_slashed_path(YYCC::StdPatch::ToUTF8Path(test_path));

#if YYCC_OS == YYCC_OS_WINDOWS
		std::wstring wdecilmer(1u, std::filesystem::path::preferred_separator);
		YYCC::yycc_u8string decilmer(YYCC::EncodingHelper::WcharToUTF8(wdecilmer));
#else
		YYCC::yycc_u8string decilmer(1u, std::filesystem::path::preferred_separator);
#endif
		YYCC::yycc_u8string test_joined_path(YYCC::StringHelper::Join(c_UTF8TestStrTable.begin(), c_UTF8TestStrTable.end(), decilmer));

		Assert(test_slashed_path == test_joined_path, YYCC_U8("YYCC::StdPatch::ToStdPath, YYCC::StdPatch::ToUTF8Path"));

		// StartsWith, EndsWith
		YYCC::yycc_u8string test_starts_ends_with(YYCC_U8("aaabbbccc"));
		Assert(YYCC::StdPatch::StartsWith(test_starts_ends_with, YYCC_U8("aaa")), YYCC_U8("YYCC::StdPatch::StartsWith"));
		Assert(!YYCC::StdPatch::StartsWith(test_starts_ends_with, YYCC_U8("ccc")), YYCC_U8("YYCC::StdPatch::StartsWith"));
		Assert(!YYCC::StdPatch::EndsWith(test_starts_ends_with, YYCC_U8("aaa")), YYCC_U8("YYCC::StdPatch::EndsWith"));
		Assert(YYCC::StdPatch::EndsWith(test_starts_ends_with, YYCC_U8("ccc")), YYCC_U8("YYCC::StdPatch::EndsWith"));

		// Contains
		std::set<int> test_set { 1, 2, 3, 4, 6, 7 };
		Assert(YYCC::StdPatch::Contains(test_set, static_cast<int>(1)), YYCC_U8("YYCC::StdPatch::Contains"));
		Assert(!YYCC::StdPatch::Contains(test_set, static_cast<int>(5)), YYCC_U8("YYCC::StdPatch::Contains"));
		std::map<int, float> test_map { { 1, 1.0f }, { 4, 4.0f } };
		Assert(YYCC::StdPatch::Contains(test_map, static_cast<int>(1)), YYCC_U8("YYCC::StdPatch::Contains"));
		Assert(!YYCC::StdPatch::Contains(test_map, static_cast<int>(5)), YYCC_U8("YYCC::StdPatch::Contains"));

	}

	enum class TestFlagEnum : uint8_t {
		Test1 = 0b00000000,
		Test2 = 0b00000001,
		Test3 = 0b00000010,
		Test4 = 0b00000100,
		Test5 = 0b00001000,
		Test6 = 0b00010000,
		Test7 = 0b00100000,
		Test8 = 0b01000000,
		Test9 = 0b10000000,
		Inverted = 0b01111111,
		Merged = Test3 + Test5,
	};

	static void EnumHelperTestbench() {
		TestFlagEnum val;

		Assert(YYCC::EnumHelper::Merge(TestFlagEnum::Test3, TestFlagEnum::Test5) == TestFlagEnum::Merged, YYCC_U8("YYCC::EnumHelper::Merge"));

		Assert(YYCC::EnumHelper::Invert(TestFlagEnum::Test9) == TestFlagEnum::Inverted, YYCC_U8("YYCC::EnumHelper::Invert"));

		val = YYCC::EnumHelper::Merge(TestFlagEnum::Test3, TestFlagEnum::Test5);
		YYCC::EnumHelper::Mask(val, TestFlagEnum::Test3);
		Assert(YYCC::EnumHelper::Bool(val), YYCC_U8("YYCC::EnumHelper::Mask"));
		val = YYCC::EnumHelper::Merge(TestFlagEnum::Test3, TestFlagEnum::Test5);
		YYCC::EnumHelper::Mask(val, TestFlagEnum::Test4);
		Assert(!YYCC::EnumHelper::Bool(val), YYCC_U8("YYCC::EnumHelper::Mask"));
		
		val = TestFlagEnum::Test3;
		YYCC::EnumHelper::Add(val, TestFlagEnum::Test5);
		Assert(val == TestFlagEnum::Merged, YYCC_U8("YYCC::EnumHelper::Add"));
		
		val = TestFlagEnum::Merged;
		YYCC::EnumHelper::Remove(val, TestFlagEnum::Test5);
		Assert(val == TestFlagEnum::Test3, YYCC_U8("YYCC::EnumHelper::Remove"));
		
		val = YYCC::EnumHelper::Merge(TestFlagEnum::Test3, TestFlagEnum::Test5);
		Assert(YYCC::EnumHelper::Has(val, TestFlagEnum::Test3), YYCC_U8("YYCC::EnumHelper::Has"));
		Assert(!YYCC::EnumHelper::Has(val, TestFlagEnum::Test4), YYCC_U8("YYCC::EnumHelper::Has"));
		
		Assert(!YYCC::EnumHelper::Bool(TestFlagEnum::Test1), YYCC_U8("YYCC::EnumHelper::Bool"));
		Assert(YYCC::EnumHelper::Bool(TestFlagEnum::Test2), YYCC_U8("YYCC::EnumHelper::Bool"));

	}

	static void VersionMacroTestbench() {
		Assert(YYCC_VERCMP_E(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_E"));
		Assert(!YYCC_VERCMP_NE(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_NE"));
		Assert(YYCC_VERCMP_G(1, 2, 3, 0, 2, 5), YYCC_U8("YYCC_VERCMP_G"));
		Assert(YYCC_VERCMP_GE(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_GE"));
		Assert(YYCC_VERCMP_NL(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_NL"));
		Assert(YYCC_VERCMP_L(0, 2, 5, 1, 2, 3), YYCC_U8("YYCC_VERCMP_L"));
		Assert(YYCC_VERCMP_LE(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_LE"));
		Assert(YYCC_VERCMP_NG(1, 2, 3, 1, 2, 3), YYCC_U8("YYCC_VERCMP_NG"));
	}

	enum class TestEnum : int8_t {
		Test1, Test2, Test3
	};

	class TestConfigManager {
	public:
		TestConfigManager() :
			m_IntSetting(YYCC_U8("int-setting"), INT32_C(0)),
			m_FloatSetting(YYCC_U8("float-setting"), 0.0f),
			m_StringSetting(YYCC_U8("string-setting"), YYCC_U8("")),
			m_BoolSetting(YYCC_U8("bool-setting"), false),
			m_ClampedFloatSetting(YYCC_U8("clamped-float-setting"), 0.0f, YYCC::Constraints::GetMinMaxRangeConstraint<float>(-1.0f, 1.0f)),
			m_EnumSetting(YYCC_U8("enum-setting"), TestEnum::Test1),
			m_CoreManager(YYCC_U8("test.cfg"), UINT64_C(0), {
				&m_IntSetting, &m_FloatSetting, &m_StringSetting, &m_BoolSetting, &m_ClampedFloatSetting, &m_EnumSetting
			}) {}
		~TestConfigManager() {}

		void PrintSettings() {
			Console::WriteLine(YYCC_U8("Config Manager Settings:"));

			Console::FormatLine(YYCC_U8("\tint-setting: %" PRIi32), m_IntSetting.Get());
			Console::FormatLine(YYCC_U8("\tfloat-setting: %f"), m_FloatSetting.Get());
			Console::FormatLine(YYCC_U8("\tstring-setting: %s"), m_StringSetting.Get().c_str());

			Console::FormatLine(YYCC_U8("\tbool-setting: %s"), m_BoolSetting.Get() ? YYCC_U8("true") : YYCC_U8("false"));
			Console::FormatLine(YYCC_U8("\tfloat-setting: %f"), m_ClampedFloatSetting.Get());
			Console::FormatLine(YYCC_U8("\tenum-setting: %" PRIi8), static_cast<std::underlying_type_t<TestEnum>>(m_EnumSetting.Get()));
		}

		YYCC::ConfigManager::NumberSetting<int32_t> m_IntSetting;
		YYCC::ConfigManager::NumberSetting<float> m_FloatSetting;
		YYCC::ConfigManager::StringSetting m_StringSetting;

		YYCC::ConfigManager::NumberSetting<bool> m_BoolSetting;
		YYCC::ConfigManager::NumberSetting<float> m_ClampedFloatSetting;
		YYCC::ConfigManager::NumberSetting<TestEnum> m_EnumSetting;

		YYCC::ConfigManager::CoreManager m_CoreManager;
	};

	static void ConfigManagerTestbench() {
		// init cfg manager
		TestConfigManager test;

		// test constraint works
		Assert(!test.m_ClampedFloatSetting.Set(2.0f), YYCC_U8("YYCC::Constraints::Constraint"));
		Assert(test.m_ClampedFloatSetting.Get() == 0.0f, YYCC_U8("YYCC::Constraints::Constraint"));

		// test modify settings
#define TEST_MACRO(member_name, set_val) { \
	Assert(test.member_name.Set(set_val), YYCC_U8("YYCC::ConfigManager::AbstractSetting::Set")); \
	Assert(test.member_name.Get() == set_val, YYCC_U8("YYCC::ConfigManager::AbstractSetting::Set")); \
}

		TEST_MACRO(m_IntSetting, INT32_C(114));
		TEST_MACRO(m_FloatSetting, 2.0f);
		TEST_MACRO(m_StringSetting, YYCC_U8("fuck"));
		TEST_MACRO(m_BoolSetting, true);
		TEST_MACRO(m_ClampedFloatSetting, 0.5f);
		TEST_MACRO(m_EnumSetting, TestEnum::Test2);

#undef TEST_MACRO

		// test save
		test.PrintSettings();
		Assert(test.m_CoreManager.Save(), YYCC_U8("YYCC::ConfigManager::CoreManager::Save"));

		// test reset
		test.m_CoreManager.Reset();
		test.PrintSettings();
		Assert(test.m_IntSetting.Get() == INT32_C(0), YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));
		Assert(test.m_FloatSetting.Get() == 0.0f, YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));
		Assert(test.m_StringSetting.Get() == YYCC_U8(""), YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));
		Assert(test.m_BoolSetting.Get() == false, YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));
		Assert(test.m_ClampedFloatSetting.Get() == 0.0f, YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));
		Assert(test.m_EnumSetting.Get() == TestEnum::Test1, YYCC_U8("YYCC::ConfigManager::CoreManager::Reset"));

		// test load
		YYCC::ConfigManager::ConfigLoadResult wrong_result = YYCC::EnumHelper::Merge(
			YYCC::ConfigManager::ConfigLoadResult::ItemError,
			YYCC::ConfigManager::ConfigLoadResult::BrokenFile
		);
		Assert(!YYCC::EnumHelper::Has(test.m_CoreManager.Load(), wrong_result), YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		test.PrintSettings();
		Assert(test.m_IntSetting.Get() == INT32_C(114), YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		Assert(test.m_FloatSetting.Get() == 2.0f, YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		Assert(test.m_StringSetting.Get() == YYCC_U8("fuck"), YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		Assert(test.m_BoolSetting.Get() == true, YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		Assert(test.m_ClampedFloatSetting.Get() == 0.5f, YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));
		Assert(test.m_EnumSetting.Get() == TestEnum::Test2, YYCC_U8("YYCC::ConfigManager::CoreManager::Load"));

	}

	class TestArgParser {
	public:
		TestArgParser() :
			m_IntArgument(YYCC_U8("int"), YYCC_U8_CHAR('i'), YYCC_U8("integral argument"), YYCC_U8("114514")),
			m_FloatArgument(nullptr, YYCC_U8_CHAR('f'), nullptr, nullptr, true),
			m_StringArgument(YYCC_U8("string"), YYCC::ArgParser::AbstractArgument::NO_SHORT_NAME, nullptr, nullptr, true),
			m_BoolArgument(nullptr, YYCC_U8_CHAR('b'), nullptr),
			m_ClampedFloatArgument(YYCC_U8("clamped-float"), YYCC::ArgParser::AbstractArgument::NO_SHORT_NAME, nullptr, nullptr, true, YYCC::Constraints::GetMinMaxRangeConstraint<float>(-1.0f, 1.0f)),
			m_OptionContext(YYCC_U8("TestArgParser"), YYCC_U8("This is the testbench of argument parser."), {
				&m_IntArgument, &m_FloatArgument, &m_StringArgument,
				&m_BoolArgument, &m_ClampedFloatArgument
			}) {}
		~TestArgParser() {}

		YYCC::ArgParser::NumberArgument<int32_t> m_IntArgument;
		YYCC::ArgParser::NumberArgument<float> m_FloatArgument;
		YYCC::ArgParser::StringArgument m_StringArgument;

		YYCC::ArgParser::SwitchArgument m_BoolArgument;
		YYCC::ArgParser::NumberArgument<float> m_ClampedFloatArgument;

		YYCC::ArgParser::OptionContext m_OptionContext;
	};

	static void ArgParserTestbench(int argc, char* argv[]) {
		// test command line getter
		{
			YYCC::ConsoleHelper::WriteLine(YYCC_U8("YYCC::ArgParser::ArgumentList::CreateFromStd"));
			auto result = YYCC::ArgParser::ArgumentList::CreateFromStd(argc, argv);
			for (result.Reset(); !result.IsEOF(); result.Next()) {
				YYCC::ConsoleHelper::FormatLine(YYCC_U8("\t%s"), result.Argument().c_str());
			}
		}
#if YYCC_OS == YYCC_OS_WINDOWS
		{
			YYCC::ConsoleHelper::WriteLine(YYCC_U8("YYCC::ArgParser::ArgumentList::CreateFromWin32"));
			auto result = YYCC::ArgParser::ArgumentList::CreateFromWin32();
			for (result.Reset(); !result.IsEOF(); result.Next()) {
				YYCC::ConsoleHelper::FormatLine(YYCC_U8("\t%s"), result.Argument().c_str());
			}
		}
#endif

		// test option context
		// init option context
		TestArgParser test;

#define PREPARE_DATA(...) const char* test_argv[] = { __VA_ARGS__ }; \
auto al = YYCC::ArgParser::ArgumentList::CreateFromStd(sizeof(test_argv) / sizeof(char*), const_cast<char**>(test_argv));

		// normal test
		{
			PREPARE_DATA("exec", "-i", "114514");
			Assert(test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_IntArgument.IsCaptured() && test.m_IntArgument.Get() == UINT32_C(114514), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(!test.m_BoolArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(!test.m_FloatArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(!test.m_StringArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(!test.m_BoolArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(!test.m_ClampedFloatArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// no argument
		{
			PREPARE_DATA("exec");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// error argument
		{
			PREPARE_DATA("exec", "-?", "114514");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// lost argument
		{
			PREPARE_DATA("exec", "-i");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// dplicated assign
		{
			PREPARE_DATA("exec", "-i", "114514" "--int", "114514");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// extra useless argument
		{
			PREPARE_DATA("exec", "-i", "114514" "1919810");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// invalid clamp argument
		{
			PREPARE_DATA("exec", "-i", "114514", "--clamped-float", "114.0");
			Assert(!test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}
		// full argument
		{
			PREPARE_DATA("exec", "-i", "114514", "-f", "2.0", "--string", "fuck", "-b", "--clamped-float", "0.5");
			Assert(test.m_OptionContext.Parse(al), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_IntArgument.IsCaptured() && test.m_IntArgument.Get() == UINT32_C(114514), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_FloatArgument.IsCaptured() && test.m_FloatArgument.Get() == 2.0f, YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_StringArgument.IsCaptured() && test.m_StringArgument.Get() == YYCC_U8("fuck"), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_BoolArgument.IsCaptured(), YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			Assert(test.m_ClampedFloatArgument.IsCaptured() && test.m_ClampedFloatArgument.Get() == 0.5f, YYCC_U8("YYCC::ArgParser::OptionContext::Parse"));
			test.m_OptionContext.Reset();
		}

		// Help text
		test.m_OptionContext.Help();

#undef PREPARE_DATA

	}

}

int main(int argc, char* argv[]) {

#if YYCC_VERCMP_NE(YYCC_VER_MAJOR, YYCC_VER_MINOR, YYCC_VER_PATCH, 1, 3 ,0)
#error "The YYCC library used when compiling is not match code expected, this may cause build error."
#error "If you trust it, please annotate these preprocessor statement, otherwise please contact developer."
#endif

	// common testbench
	// normal
	YYCCTestbench::EncodingTestbench();
	YYCCTestbench::StringTestbench();
	YYCCTestbench::ParserTestbench();
	YYCCTestbench::WinFctTestbench();
	YYCCTestbench::StdPatchTestbench();
	YYCCTestbench::EnumHelperTestbench();
	YYCCTestbench::VersionMacroTestbench();
	// advanced
	YYCCTestbench::ConfigManagerTestbench();
	YYCCTestbench::ArgParserTestbench(argc, argv);

	// testbench which may terminal app or ordering input
	YYCCTestbench::ConsoleTestbench();
	YYCCTestbench::DialogTestbench();
	YYCCTestbench::ExceptionTestbench();
}
