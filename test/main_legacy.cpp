#include <YYCCommonplace.hpp>
#include <cstdio>
#include <set>
#include <map>

namespace Console = YYCC::ConsoleHelper;

namespace YYCCTestbench {

	static void Assert(bool condition, const YYCC::yycc_char8_t* description) {
		if (condition) {
			Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_GREEN("OK: %s")), description);
		} else {
			Console::FormatLine(YYCC_U8(YYCC_COLOR_LIGHT_RED("Failed: %s\n")), description);
			std::abort();
		}
	}

	static void ConsoleTestbench() {
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

	static void ExceptionTestbench() {
#if defined(YYCC_OS_WINDOWS)

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
