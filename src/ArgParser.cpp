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
		for (int i = 1; i < argc; ++i) { // starts with 1 to remove first part (executable self)
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
			for (int i = 1; i < argc; ++i) { // starts with 1 to remove first part (executable self)
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

	const yycc_u8string& ArgumentList::Current() const {
		if (IsEOF()) throw std::runtime_error("attempt to get data on the tail of iterator.");
		return *m_ArgumentsIterator;
	}

	bool ArgumentList::IsSwitch(bool* is_long_name, yycc_u8string* long_name, yycc_char8_t* short_name) const {
		// check eof first
		if (IsEOF()) throw std::runtime_error("attempt to fetch data on the tail of iterator.");
		// check long name first, then check short name
		if (IsLongNameSwitch(long_name)) {
			if (is_long_name != nullptr) *is_long_name = true;
			return true;
		}
		if (IsShortNameSwitch(short_name)) {
			if (is_long_name != nullptr) *is_long_name = false;
			return true;
		}
		// not matched
		return false;
	}
	bool ArgumentList::IsLongNameSwitch(yycc_u8string* name_part) const {
		// fetch current parameter
		if (IsEOF()) throw std::runtime_error("attempt to fetch data on the tail of iterator.");
		const yycc_u8string& param = *m_ArgumentsIterator;
		// find double slash
		if (param.find(AbstractArgument::DOUBLE_DASH) != 0u) return false;
		// check gotten long name
		yycc_u8string_view long_name = yycc_u8string_view(param).substr(2u);
		if (!AbstractArgument::IsLegalLongName(long_name)) return false;
		// set checked long name if possible and return
		if (name_part != nullptr)
			*name_part = long_name;
		return true;
	}
	bool ArgumentList::IsShortNameSwitch(yycc_char8_t* name_part) const {
		// fetch current parameter
		if (IsEOF()) throw std::runtime_error("attempt to fetch data on the tail of iterator.");
		const yycc_u8string& param = *m_ArgumentsIterator;
		// if the length is not exactly equal to 2, 
		// or it not starts with dash,
		// it is impossible a short name
		if (param.size() != 2u || param[0] != AbstractArgument::DASH) return false;
		// check gotten short name
		yycc_char8_t short_name = param[1];
		if (!AbstractArgument::IsLegalShortName(short_name)) return false;
		// set checked short name if possible and return
		if (name_part != nullptr)
			*name_part = short_name;
		return true;
	}

	bool ArgumentList::IsValue(yycc_u8string* val) const { 
		bool is_value = !IsSwitch();
		if (is_value && val != nullptr)
			*val = *m_ArgumentsIterator;
		return is_value;
	}

	bool ArgumentList::IsEOF() const { return m_ArgumentsIterator == m_Arguments.end(); }

	void ArgumentList::Reset() { m_ArgumentsIterator = m_Arguments.begin(); }

#pragma endregion

#pragma region Abstract Argument

	const yycc_u8string AbstractArgument::DOUBLE_DASH = YYCC_U8("--");
	const yycc_char8_t AbstractArgument::DASH = YYCC_U8_CHAR('-');
	const yycc_char8_t AbstractArgument::NO_SHORT_NAME = YYCC_U8_CHAR(0);
	const yycc_char8_t AbstractArgument::MIN_SHORT_NAME = YYCC_U8_CHAR('!');
	const yycc_char8_t AbstractArgument::MAX_SHORT_NAME = YYCC_U8_CHAR('~');

	bool AbstractArgument::IsLegalShortName(yycc_char8_t short_name) {
		if (short_name == AbstractArgument::DASH || // dash is not allowed
			short_name < AbstractArgument::MIN_SHORT_NAME || short_name > AbstractArgument::MAX_SHORT_NAME) { // non-display ASCII chars are not allowed
			return false;
		}
		// okey
		return true;
	}
	bool AbstractArgument::IsLegalLongName(const yycc_u8string_view& long_name) {
		// empty is not allowed
		if (long_name.empty()) return false;
		// non-display ASCII chars are not allowed
		for (const auto& val : long_name) {
			if (val < AbstractArgument::MIN_SHORT_NAME || val > AbstractArgument::MAX_SHORT_NAME)
				return false;
		}
		// okey
		return true;
	}

	AbstractArgument::AbstractArgument(
		const yycc_char8_t* long_name, yycc_char8_t short_name,
		const yycc_char8_t* description, const yycc_char8_t* argument_example,
		bool is_optional) :
		m_LongName(), m_ShortName(AbstractArgument::NO_SHORT_NAME), m_Description(), m_ArgumentExample(),
		m_IsOptional(is_optional), m_IsCaptured(false) {

		// try to assign long name and check it
		if (long_name != nullptr) {
			m_LongName = long_name;
			if (!AbstractArgument::IsLegalLongName(m_LongName))
				throw std::invalid_argument("Given long name is invalid.");
		}
		// try to assign short name and check it
		if (short_name != AbstractArgument::NO_SHORT_NAME) {
			m_ShortName = short_name;
			if (!AbstractArgument::IsLegalShortName(m_ShortName))
				throw std::invalid_argument("Given short name is invalid.");
		}
		// check short name and long name existence
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

	bool OptionContext::Parse(ArgumentList& al) {
		// reset argument list first
		al.Reset();

		// prepare variables and start loop
		yycc_u8string long_name;
		yycc_char8_t short_name;
		bool is_long_name;
		while (!al.IsEOF()) {
			// if we can not find any switches, return with error
			if (!al.IsSwitch(&is_long_name, &long_name, &short_name)) return false;
			
			// find corresponding argument by long name or short name.
			// if we can not find it, return with error.
			AbstractArgument* arg;
			if (is_long_name) {
				auto finder = m_LongNameMap.find(long_name);
				if (finder == m_LongNameMap.end()) return false;
				arg = finder->second;
			} else {
				auto finder = m_ShortNameMap.find(short_name);
				if (finder == m_ShortNameMap.end()) return false;
				arg = finder->second;
			}

			// if this argument has been captured, raise error
			if (arg->IsCaptured()) return false;
			// call user parse function of found argument
			if (arg->Parse(al)) {
				// success. mark it is captured
				arg->SetCaptured(true);
			} else {
				// failed, return error
				return false;
			}

			// move to next argument
			al.Next();
		}

		// after processing all argument,
		// we should check whether all non-optional argument are captured.
		for (const auto* arg : m_Arguments) {
			if (!arg->IsOptional() && !arg->IsCaptured())
				return false;
		}

		// okey
		return true;
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
