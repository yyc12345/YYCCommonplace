#pragma once
#include "YYCCInternal.hpp"

#include "Constraints.hpp"
#include "EncodingHelper.hpp"
#include <functional>
#include <vector>
#include <map>
#include <stdexcept>

namespace YYCC::ArgParser {

	class ArgumentList {
	public:
		static ArgumentList CreateFromStd(int argc, char* argv[]);
#if YYCC_OS == YYCC_OS_WINDOWS
		static ArgumentList CreateFromWin32();
#endif
	private:
		ArgumentList(std::vector<yycc_u8string>&& arguments);

	public:
		void Prev();
		void Next();
		bool IsSwitch(
			bool* is_long_name = nullptr,
			yycc_u8string_view* long_name = nullptr,
			yycc_char8_t* short_name = nullptr) const;
		bool IsValue() const;
		bool IsEOF() const;
		void Reset();

	private:
		std::vector<yycc_u8string> m_Arguments;
		std::vector<yycc_u8string>::const_iterator m_ArgumentsIterator;
	};

	class AbstractArgument {
		friend class OptionContext;
	public:
		static const yycc_u8string DOUBLE_DASH;
		static const yycc_char8_t DASH;
		static const yycc_char8_t NO_SHORT_NAME;
		static const yycc_char8_t MIN_SHORT_NAME;
		static const yycc_char8_t MAX_SHORT_NAME;
	public:
		AbstractArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name = AbstractArgument::NO_SHORT_NAME,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false);
		virtual ~AbstractArgument();

	protected:
		virtual bool Parse(ArgumentList& al) = 0;
		virtual void Reset() = 0;

	public:
		bool HasLongName() const;
		const yycc_u8string& GetLongName() const;
		bool HasShortName() const;
		yycc_char8_t GetShortName() const;
		bool HasDescription() const;
		const yycc_u8string& GetDescription() const;
		bool HasArgumentExample() const;
		const yycc_u8string& GetArgumentExample() const;
		bool IsOptional() const;
	private:
		yycc_u8string m_LongName;
		yycc_char8_t m_ShortName;
		yycc_u8string m_Description;
		yycc_u8string m_ArgumentExample;
		bool m_IsOptional;

	public:
		bool IsCaptured() const;
	private:
		void SetCaptured(bool is_captured);
		bool m_IsCaptured;
	};

	class OptionContext {
	public:
		OptionContext(
			const yycc_char8_t* summary, const yycc_char8_t* description,
			std::initializer_list<AbstractArgument*> arguments);
		~OptionContext();

	public:
		bool Parse(ArgumentList* al);
		void Reset();

	private:
		yycc_u8string m_Summary;
		yycc_u8string m_Description;

		std::vector<AbstractArgument*> m_Arguments;
		std::map<yycc_u8string, AbstractArgument*> m_LongNameMap;
		std::map<yycc_char8_t, AbstractArgument*> m_ShortNameMap;
	};

#pragma region Argument Presets

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	class NumberArgument : public AbstractArgument {
	public:
		NumberArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false,
			Constraints::Constraint<_Ty> constraint = Constraints::Constraint<_Ty> {}) :
			AbstractArgument(long_name, short_name, description, argument_example, is_optional), m_Data(), m_Constraint(constraint) {}
		virtual ~NumberArgument() {}

	public:
		_Ty Get() const {
			if (!IsCaptured()) throw std::runtime_error("try fetching data from a not captured argument.");
			return m_Data;
		}

	protected:
		virtual bool Parse(ArgumentList& al) override {} // todo
		virtual void Reset() override {}// todo

	protected:
		_Ty m_Data;
		Constraints::Constraint<_Ty> m_Constraint;
	};

	class SwitchArgument : public AbstractArgument {
	public:
		SwitchArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr) :
			AbstractArgument(long_name, short_name, description, argument_example, true), m_Data(false) {} // bool switch must be optional, because it is false if no given switch.
		virtual ~SwitchArgument() {}

	public:
		bool Get() const { return m_Data; }

	protected:
		virtual bool Parse(ArgumentList& al) override { m_Data = true; }
		virtual void Reset() override { m_Data = false; }

	protected:
		bool m_Data;
	};

	class StringArgument : public AbstractArgument {
	public:
		StringArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false,
			Constraints::Constraint<yycc_u8string_view> constraint = Constraints::Constraint<yycc_u8string_view> {}) :
			AbstractArgument(long_name, short_name, description, argument_example, is_optional), m_Data(), m_Constraint(constraint) {}
		virtual ~StringArgument() {}

	public:
		const yycc_u8string& Get() const {
			if (!IsCaptured()) throw std::runtime_error("try fetching data from a not captured argument.");
			return m_Data;
		}

	protected:
		virtual bool Parse(ArgumentList& al) override {} // todo
		virtual void Reset() override {}// todo

	protected:
		yycc_u8string m_Data;
		Constraints::Constraint<yycc_u8string_view> m_Constraint;
	};

#pragma endregion


}
