#pragma once
#include "YYCCInternal.hpp"

#include "Constraints.hpp"
#include "EncodingHelper.hpp"
#include "ParserHelper.hpp"
#include <functional>
#include <vector>
#include <map>
#include <stdexcept>

/**
 * @brief Universal argument parser.
 * @details
 * For how to use this namespace, please see \ref arg_parser.
*/
namespace YYCC::ArgParser {

	/**
	 * @brief The advanced wrapper of the list containing command line arguments.
	 * @details
	 * This class is used by OptionContext and argument class internally for convenience.
	 * It should not be constrcuted directly.
	 * Programmer should choose proper static creation function to create instance of this class.
	*/
	class ArgumentList {
	public:
		/**
		 * @brief Create argument list from the parameters of standard C main function.
		 * @param[in] argc The argument count passed to standard C main function.
		 * @param[in] argv The argument value passed to standard C main function.
		 * @return Extracted argument list instance.
		 * @remarks
		 * First item in command line will be stripped,
		 * because in most cases it points to executable self
		 * and should not be seen as a part of arguments.
		*/
		static ArgumentList CreateFromStd(int argc, char* argv[]);
#if YYCC_OS == YYCC_OS_WINDOWS
		/**
		 * @brief Create argument list from Win32 function.
		 * @details
		 * @return Extracted argument list instance.
		 * @remarks
		 * First item in command line will be stripped,
		 * because in most cases it points to executable self
		 * and should not be seen as a part of arguments.
		 * \par
		 * Programmer should use this function instead of CreateFromStd(),
		 * because that function involve encoding issue on Windows, especially command line including non-ASCII chars.
		 * Only this function guaranteen that return correct argument list on Windows.
		*/
		static ArgumentList CreateFromWin32();
#endif
	private:
		/**
		 * @brief Constructor of ArgumentList used internally.
		 * @param[in] arguments 
		 * Underlying argument list.
		 * This argument list should remove first executable name before passing it to there.
		*/
		ArgumentList(std::vector<yycc_u8string>&& arguments);
	public:
		/// @brief Default copy constructor
		ArgumentList(const ArgumentList&) = default;
		/// @brief Default copy assigner
		ArgumentList& operator=(const ArgumentList&) = default;
		/// @brief Default move constructor
		ArgumentList(ArgumentList&&) = default;
		/// @brief Default move assigner
		ArgumentList& operator=(ArgumentList&&) = default;

	public:
		/**
		 * @brief Move to previous argument.
		 * @exception std::runtime_error Try moving at the head of argument list.
		*/
		void Prev();
		/**
		 * @brief Move to next argument.
		 * @exception std::runtime_error Try moving at the tail of argument list.
		*/
		void Next();
		/**
		 * @brief Get the string of current argument.
		 * @exception std::runtime_error Try fetching data at the tail of argument list.
		 * @return The constant reference to the string of current argument.
		*/
		const yycc_u8string& Argument() const;
		/**
		 * @brief Check whether current argument is a option / switch.
		 * @param[out] is_long_name 
		 * It will be set true if this argument is long name, otherwise short name.
		 * nullptr if you don't want to receive this infomation.
		 * @param[out] long_name 
		 * The container holding matched long name if it is (double dash stripped).
		 * nullptr if you don't want to receive this infomation.
		 * @param[out] short_name 
		 * The variable holding matched short name if it is (dash stripped).
		 * nullptr if you don't want to receive this infomation.
		 * @exception std::runtime_error Try fetching data at the tail of argument list.
		 * @return 
		 * True if it is, otherwise false.
		 * If this function return false, all given parameters are in undefined status.
		*/
		bool IsSwitch(
			bool* is_long_name = nullptr,
			yycc_u8string* long_name = nullptr,
			yycc_char8_t* short_name = nullptr) const;
		/**
		 * @brief Check whether current argument is a value.
		 * @param[out] val 
		 * The variable holding value if it is.
		 * nullptr if you don't want to receive this infomation.
		 * @exception std::runtime_error Try fetching data at the tail of argument list.
		 * @return True if it is, otherwise false.
		*/
		bool IsValue(yycc_u8string* val = nullptr) const;
		/**
		 * @brief Check whether we are at the tail of argument list.
		 * @details
		 * Please note EOF is a special state that you can not fetch data from it.
		 * EOF is the next element of the last element of argument list.
		 * It more like \c end() in most C++ container.
		 * @return True if it is, otherwise false.
		*/
		bool IsEOF() const;
		/**
		 * @brief Reset cursor to the head of argument list for reuse.
		*/
		void Reset();
	private:
		/**
		 * @brief Check whether current argument is long name option / switch.
		 * @details This function is used by IsSwitch() internally.
		 * @param[out] name_part 
		 * The container holding matched long name if it is (double dash stripped).
		 * nullptr if you don't want to receive this infomation.
		 * @return True if it is, otherwise false.
		*/
		bool IsLongNameSwitch(yycc_u8string* name_part = nullptr) const;
		/**
		 * @brief Check whether current argument is short name option / switch.
		 * @details This function is used by IsSwitch() internally.
		 * @param[out] name_part 
		 * The variable holding matched short name if it is (dash stripped).
		 * nullptr if you don't want to receive this infomation.
		 * @return True if it is, otherwise false.
		*/
		bool IsShortNameSwitch(yycc_char8_t* name_part = nullptr) const;

	private:
		std::vector<yycc_u8string> m_Arguments;
		std::vector<yycc_u8string>::const_iterator m_ArgumentsIterator;
	};

	/**
	 * @brief The base class of every argument.
	 * @details Programmer can inherit this class and implement essential functions to create custom argument.
	*/
	class AbstractArgument {
		friend class OptionContext;

		// Long name and short name constants and checker.
	public:
		static const yycc_u8string DOUBLE_DASH; ///< The constant value representing double dash (\c --)
		static const yycc_char8_t DASH; ///< The constant value representing dash (\c -)
		static const yycc_char8_t NO_SHORT_NAME; ///< The constant value representing that there is not short value.
		static const yycc_char8_t MIN_SHORT_NAME; ///< The constant value representing the minimum value of valid ASCII chars in short and long name.
		static const yycc_char8_t MAX_SHORT_NAME; ///< The constant value representing the maximum value of valid ASCII chars in short and long name.
		/**
		 * @brief Check whether given short name is valid.
		 * @details
		 * An ASCII code of valid short name 
		 * should not lower than #MIN_SHORT_NAME or higher than #MAX_SHORT_NAME.
		 * It also can not be #DASH.
		 * @param[in] short_name Short name for checking.
		 * @return True if it is valid, otherwise false.
		*/
		static bool IsLegalShortName(yycc_char8_t short_name);
		/**
		 * @brief Check whether given long name is valid.
		 * @details
		 * An ASCII code of every item in valid long name 
		 * should not lower than #MIN_SHORT_NAME or higher than #MAX_SHORT_NAME.
		 * However it can be #DASH. This is different with short name.
		 * @param[in] long_name Long name for checking.
		 * @return True if it is valid, otherwise false.
		*/
		static bool IsLegalLongName(const yycc_u8string_view& long_name);

		// Constructor & destructor
	public:
		/**
		 * @brief Constructor an argument
		 * @param[in] long_name The long name of this argument. nullptr if no long name.
		 * @param[in] short_name The short name of this argument. #NO_SHORT_NAME if no short name.
		 * @param[in] description The description of this argument to indroduce what this argument does. nullptr if no description.
		 * @param[in] argument_example The example string of this argument's value. nullptr if no example.
		 * @param[in] is_optional
		 * True if this argument is optional argument.
		 * Optional argument can be absent in argument list.
		 * Non-optional argument must be presented in argument list,
		 * otherwise parser will fail.
		 * @exception std::invalid_argument Given short name or long name are invalid.
		*/
		AbstractArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name = AbstractArgument::NO_SHORT_NAME,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false);
		virtual ~AbstractArgument();

		// ===== User Implementation =====
	protected:
		/**
		 * @brief User implemented custom parse function
		 * @param[in] al  The argument list for parsing.
		 * @return True if parse is success, otherwise false.
		 * @remarks
		 * When enter this function, argument list points to switch self.
		 * After success parsing, you should point it to the argument this function last accepted.
		 * For exmaple, for command line "-i 114514",
		 * when enter this function, this argument list point to "-i",
		 * and you should set it to "114514" when exiting this function.
		*/
		virtual bool Parse(ArgumentList& al) = 0;
		/**
		 * @brief User implemented custom reset function
		 * @remarks 
		 * In this function, user should claer its stored value if is has.
		 * You don't need clar capture state. That is done by library self.
		*/
		virtual void Reset() = 0;
		
		// ===== Basic Infos =====
	public:
		/// @brief Check whether this argument specify long name.
		/// @return True if it is, otherwise false.
		bool HasLongName() const;
		/// @brief Get specified long name.
		/// @return Specified long name.
		const yycc_u8string& GetLongName() const;
		/// @brief Check whether this argument specify short name.
		/// @return True if it is, otherwise false.
		bool HasShortName() const;
		/// @brief Get specified short name.
		/// @return Specified short name.
		yycc_char8_t GetShortName() const;
		/// @brief Check whether this argument specify description.
		/// @return True if it is, otherwise false.
		bool HasDescription() const;
		/// @brief Get specified description.
		/// @return Specified description.
		const yycc_u8string& GetDescription() const;
		/// @brief Check whether this argument specify example.
		/// @return True if it is, otherwise false.
		bool HasArgumentExample() const;
		/// @brief Get specified example.
		/// @return Specified example.
		const yycc_u8string& GetArgumentExample() const;
		/// @brief Check whether this argument is optional.
		/// @return True if it is, otherwise false.
		bool IsOptional() const;
	private:
		yycc_u8string m_LongName;
		yycc_char8_t m_ShortName;
		yycc_u8string m_Description;
		yycc_u8string m_ArgumentExample;
		bool m_IsOptional;
		
		// ===== Capture State =====
	public:
		/// @brief Check whether this argument has been captured.
		/// @return True if it is, otherwise false.
		bool IsCaptured() const;
	private:
		/**
		 * @brief Set capture state of this argument.
		 * @details This function is used internally by OptionContext.
		 * @param[in] is_captured New states of captured.
		*/
		void SetCaptured(bool is_captured);
		bool m_IsCaptured;
	};

	/// @brief The core of argument parser, also manage all arguments.
	class OptionContext {
	public:
		/**
		 * @brief Construct option context.
		 * @param[in] summary The summary of this application which will be printed in help text.
		 * @param[in] description The description of this application which will be printed in help text.
		 * @param[in] arguments The initializer list including pointers to all arguments.
		*/
		OptionContext(
			const yycc_char8_t* summary, const yycc_char8_t* description,
			std::initializer_list<AbstractArgument*> arguments);
		~OptionContext();

	public:
		/**
		 * @brief Start a parse.
		 * @param[in] al The reference to ArgumentList for parsing.
		 * @return 
		 * True if success, otherwise false.
		 * If this function return false, you should not visit any arguments it managed.
		*/
		bool Parse(ArgumentList& al);
		/**
		 * @brief Reset all managed argument to default state thus you can start another parsing.
		*/
		void Reset();
		/**
		 * @brief Print help text in \c stdout.
		*/
		void Help() const;

	private:
		yycc_u8string m_Summary;
		yycc_u8string m_Description;

		std::vector<AbstractArgument*> m_Arguments;
		std::map<yycc_u8string, AbstractArgument*> m_LongNameMap;
		std::map<yycc_char8_t, AbstractArgument*> m_ShortNameMap;
	};

#pragma region Argument Presets
	
	/**
	 * @brief Arithmetic (integral, floating point. except bool) type argument
	 * @tparam _Ty The internal stored type belongs to arithmetic type.
	*/
	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
	class NumberArgument : public AbstractArgument {
	public:
		/**
		 * @brief Constructor an arithmetic argument
		 * @param[in] long_name The long name of this argument. nullptr if no long name.
		 * @param[in] short_name The short name of this argument. #NO_SHORT_NAME if no short name.
		 * @param[in] description The description of this argument to indroduce what this argument does. nullptr if no description.
		 * @param[in] argument_example The example string of this argument's value. nullptr if no example.
		 * @param[in] is_optional True if this argument is optional argument.
		 * @param[in] constraint The constraint applied to this argument.
		 * @exception std::invalid_argument Given short name or long name are invalid.
		*/
		NumberArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false,
			Constraints::Constraint<_Ty> constraint = Constraints::Constraint<_Ty> {}) :
			AbstractArgument(long_name, short_name, description, argument_example, is_optional), m_Data(), m_Constraint(constraint) {}
		virtual ~NumberArgument() {}

	public:
		/// @brief Get stored data in argument.
		_Ty Get() const {
			if (!IsCaptured()) throw std::runtime_error("try fetching data from a not captured argument.");
			return m_Data;
		}

	protected:
		virtual bool Parse(ArgumentList& al) override {
			// try get corresponding value
			yycc_u8string strval;
			al.Next();
			if (al.IsEOF() || !al.IsValue(&strval)) {
				al.Prev();
				return false;
			}
			// try parsing value
			if (!YYCC::ParserHelper::TryParse<_Ty>(strval, m_Data)) return false;
			// check constraint
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(m_Data))
				return false;
			// okey
			return true;
		}
		virtual void Reset() override {
			std::memset(&m_Data, 0, sizeof(m_Data));
		}

	protected:
		_Ty m_Data;
		Constraints::Constraint<_Ty> m_Constraint;
	};

	/**
	 * @brief A simple switch type argument which do not store any value.
	*/
	class SwitchArgument : public AbstractArgument {
	public:
		/**
		 * @brief Constructor an switch argument
		 * @param[in] long_name The long name of this argument. nullptr if no long name.
		 * @param[in] short_name The short name of this argument. #NO_SHORT_NAME if no short name.
		 * @param[in] description The description of this argument to indroduce what this argument does. nullptr if no description.
		 * @exception std::invalid_argument Given short name or long name are invalid.
		*/
		SwitchArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr) :
			// bool switch must be optional, because it is false if no given switch.
			// bool switch doesn't have argument, so it doesn't have example property.
			AbstractArgument(long_name, short_name, description, nullptr, true) {} 
		virtual ~SwitchArgument() {}

	protected:
		virtual bool Parse(ArgumentList& al) override { return true; } // simply return true because no value to store.
		virtual void Reset() override {} // nothing need to be reset.
	};
	
	/// @brief String type argument
	class StringArgument : public AbstractArgument {
	public:
		/**
		 * @brief Constructor a string argument
		 * @param[in] long_name The long name of this argument. nullptr if no long name.
		 * @param[in] short_name The short name of this argument. #NO_SHORT_NAME if no short name.
		 * @param[in] description The description of this argument to indroduce what this argument does. nullptr if no description.
		 * @param[in] argument_example The example string of this argument's value. nullptr if no example.
		 * @param[in] is_optional True if this argument is optional argument.
		 * @param[in] constraint The constraint applied to this argument.
		 * @exception std::invalid_argument Given short name or long name are invalid.
		*/
		StringArgument(
			const yycc_char8_t* long_name, yycc_char8_t short_name,
			const yycc_char8_t* description = nullptr, const yycc_char8_t* argument_example = nullptr,
			bool is_optional = false,
			Constraints::Constraint<yycc_u8string> constraint = Constraints::Constraint<yycc_u8string> {}) :
			AbstractArgument(long_name, short_name, description, argument_example, is_optional), m_Data(), m_Constraint(constraint) {}
		virtual ~StringArgument() {}

	public:
		/// @brief Get stored data in argument.
		const yycc_u8string& Get() const {
			if (!IsCaptured()) throw std::runtime_error("try fetching data from a not captured argument.");
			return m_Data;
		}

	protected:
		virtual bool Parse(ArgumentList& al) override {
			// try get corresponding value
			al.Next();
			if (al.IsEOF() || !al.IsValue(&m_Data)) {
				al.Prev();
				return false;
			}
			// check constraint
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(m_Data))
				return false;
			// okey
			return true;
		}
		virtual void Reset() override {
			m_Data.clear();
		}

	protected:
		yycc_u8string m_Data;
		Constraints::Constraint<yycc_u8string> m_Constraint;
	};

#pragma endregion


}
