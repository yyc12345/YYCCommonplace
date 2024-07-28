#include "ArgParser.hpp"

#include "EncodingHelper.hpp"

#if YYCC_OS == YYCC_OS_WINDOWS
#include "WinImportPrefix.hpp"
#include <Windows.h>
#include <shellapi.h>
#include <processenv.h>
#include "WinImportSuffix.hpp"
#endif

namespace YYCC::ArgParser {

#pragma region Arguments List

	ArgumentList ArgumentList::CreateFromStd(int argc, char* argv[]) {
		std::vector<yycc_u8string> args;
		for (int i = 0; i < argc; ++i) {
			if (argv[i] != nullptr)
				args.emplace_back(yycc_u8string(YYCC::EncodingHelper::ToUTF8(argv[i])));
		}
		return ArgumentList(std::move(args));
	}

#if YYCC_OS == YYCC_OS_WINDOWS
	ArgumentList ArgumentList::CreateFromWin32() {
		// Reference: https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-commandlinetoargvw

		// prepare list
		std::vector<yycc_u8string> args;

		// try fetching from Win32 functions
		int argc;
		LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (argv != NULL) {
			for (int i = 0; i < argc; ++i) {
				if (argv[i] != nullptr) {
					yycc_u8string u8_argv;
					if (YYCC::EncodingHelper::WcharToUTF8(argv[i], u8_argv))
						args.emplace_back(std::move(u8_argv));
				}
			}
		}
		LocalFree(argv);

		// return result
		return ArgumentList(std::move(args));
	}
#endif

	ArgumentList::ArgumentList(std::vector<yycc_u8string>&& arguments) :
		m_Arguments(arguments), m_ArgumentsIterator(m_Arguments.begin()) {}

	void ArgumentList::Prev() {
		if (m_ArgumentsIterator == m_Arguments.begin())
			throw std::runtime_error("attempt to move on the head of iterator.");
		--m_ArgumentsIterator;
	}

	void ArgumentList::Next() {
		if (IsEOF()) throw std::runtime_error("attempt to move on the tail of iterator.");
		++m_ArgumentsIterator;
	}

	static bool IsLongName(const yycc_u8string& param, yycc_u8string_view* name_part) {
		if (param.find(AbstractArgument::DOUBLE_DASH) != 0u) return false;
		if (name_part != nullptr)
			*name_part = yycc_u8string_view(param).substr(2u);
		return true;
	}
	static bool IsShortName(const yycc_u8string& param, yycc_char8_t* name_part) {
		if (param.size() != 2u ||
			param[0] != AbstractArgument::DASH ||
			param[1] == AbstractArgument::DASH ||
			param[1] < AbstractArgument::MIN_SHORT_NAME || param[1] > AbstractArgument::MAX_SHORT_NAME) {
			return false;
		}
		if (name_part != nullptr)
			*name_part = param[1];
		return true;
	}
	bool ArgumentList::IsSwitch(
		bool* is_long_name = nullptr,
		yycc_u8string_view* long_name = nullptr,
		yycc_char8_t* short_name = nullptr) const {
		// get argument first
		if (IsEOF()) throw std::runtime_error("attempt to fetch data on the tail of iterator.");
		const auto& param = *m_ArgumentsIterator;
		// check long name first, then check short name
		if (IsLongName(param, long_name)) {
			if (is_long_name != nullptr) *is_long_name = true;
			return true;
		}
		if (IsShortName(param, short_name)) {
			if (is_long_name != nullptr) *is_long_name = false;
			return true;
		}
		// not matched
		return false;
	}

	bool ArgumentList::IsValue() const { return !IsSwitch(); }

	bool ArgumentList::IsEOF() const { return m_ArgumentsIterator == m_Arguments.end(); }

	void ArgumentList::Reset() { m_ArgumentsIterator = m_Arguments.begin(); }

#pragma endregion

#pragma region Abstract Argument

	const yycc_u8string AbstractArgument::DOUBLE_DASH = YYCC_U8("--");
	const yycc_char8_t AbstractArgument::DASH = YYCC_U8_CHAR('-');
	const yycc_char8_t AbstractArgument::NO_SHORT_NAME = YYCC_U8_CHAR(0);
	const yycc_char8_t AbstractArgument::MIN_SHORT_NAME = YYCC_U8_CHAR('!');
	const yycc_char8_t AbstractArgument::MAX_SHORT_NAME = YYCC_U8_CHAR('~');

	AbstractArgument::AbstractArgument(
		const yycc_char8_t* long_name, yycc_char8_t short_name,
		const yycc_char8_t* description, const yycc_char8_t* argument_example,
		bool is_optional) :
		m_LongName(), m_ShortName(NO_SHORT_NAME), m_Description(), m_ArgumentExample(),
		m_IsOptional(is_optional), m_IsCaptured(false) {

		// try to assign long name
		if (long_name != nullptr) m_LongName = long_name;
		// try to assign short name
		if (short_name == AbstractArgument::DASH ||
			short_name < AbstractArgument::MIN_SHORT_NAME ||
			short_name > AbstractArgument::MAX_SHORT_NAME) {
			throw std::invalid_argument("given short name character is invalid.");
		}
		m_ShortName = short_name;
		// check short name and long name
		if (!HasShortName() && !HasLongName())
			throw std::invalid_argument("you must specify an one of long name or short name.");

		// try to assign other string values
		if (description != nullptr) m_Description = description;
		if (argument_example != nullptr) m_ArgumentExample = argument_example;
	}

	AbstractArgument::~AbstractArgument() {}

	bool AbstractArgument::HasLongName() const { return !m_LongName.empty(); }
	const yycc_u8string& AbstractArgument::GetLongName() const { return m_LongName; }
	bool AbstractArgument::HasShortName() const { return m_ShortName != NO_SHORT_NAME; }
	yycc_char8_t AbstractArgument::GetShortName() const { return m_ShortName; }
	bool AbstractArgument::HasDescription() const { return !m_Description.empty(); }
	const yycc_u8string& AbstractArgument::GetDescription() const { return m_Description; }
	bool AbstractArgument::HasArgumentExample() const { return !m_ArgumentExample.empty(); }
	const yycc_u8string& AbstractArgument::GetArgumentExample() const { return m_ArgumentExample; }
	bool AbstractArgument::IsOptional() const { return m_IsOptional; }

	bool AbstractArgument::IsCaptured() const { return m_IsCaptured; }
	void AbstractArgument::SetCaptured(bool is_captured) { m_IsCaptured = is_captured; }

#pragma endregion

#pragma region Option Context

	OptionContext::OptionContext(
		const yycc_char8_t* summary, const yycc_char8_t* description,
		std::initializer_list<AbstractArgument*> arguments) :
		m_Summary(), m_Description() {
		// assign summary and description
		if (summary != nullptr) m_Summary = summary;
		if (description != nullptr) m_Description = description;

		// insert argument list and check them
		for (auto* arg : arguments) {
			// insert into long name map if necessary
			if (arg->HasLongName()) {
				auto result = m_LongNameMap.try_emplace(arg->GetLongName(), arg);
				if (!result.second) throw std::invalid_argument("duplicated long name!");
			}
			// insert into short name map if necessary
			if (arg->HasShortName()) {
				auto result = m_ShortNameMap.try_emplace(arg->GetShortName(), arg);
				if (!result.second) throw std::invalid_argument("duplicated short name!");
			}
			// insert into argument list
			m_Arguments.emplace_back(arg);
		}
	}

	OptionContext::~OptionContext() {}

	bool OptionContext::Parse(ArgumentList* al) {
		return false; //todo
	}

	void OptionContext::Reset() {
		for (auto* arg : m_Arguments) {
			// clear user data and unset captured
			arg->Reset();
			arg->SetCaptured(false);
		}
	}

#pragma endregion

}
