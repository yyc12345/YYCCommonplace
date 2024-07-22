#pragma once
#include "YYCCInternal.hpp"

#include <memory>
#include <vector>
#include <map>
#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cstring>

namespace YYCC::ConfigManager {

	template<typename _Ty>
	struct Constraint {
		using CheckFct_t = std::function<bool(const _Ty&)>;
		//using CorrectFct_t = std::function<_Ty(const _Ty&)>;
		CheckFct_t m_CheckFct;
		//CorrectFct_t m_CorrectFct;

		bool IsValid() const {
			return m_CheckFct != nullptr/* && m_CorrectFct != nullptr*/;
		}
	};

	namespace ConstraintPresets {

		template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> && !std::is_enum_v<_Ty> && !std::is_same_v<_Ty, bool>, int> = 0>
		Constraint<_Ty> GetNumberRangeConstraint(_Ty min_value, _Ty max_value) {
			if (min_value > max_value)
				throw std::invalid_argument("invalid min max value for NumberRangeConstraint");
			return Constraint<_Ty> {
				[min_value, max_value](const _Ty& val) -> bool { return (val <= max_value) && (val >= min_value); }
					/*[min_value, max_value](const _Ty& val) -> _Ty { return std::clamp(val, min_value, max_value); }*/
			};
		}

	}

	class AbstractSetting {
		friend class CoreManager;
	public:
		AbstractSetting(const yycc_char8_t* name) : m_Name(), m_RawData() {
			if (name != nullptr) m_Name = name;
		}
		virtual ~AbstractSetting() {}

		// Name interface
	public:
		const yycc_u8string& GetName() const { return m_Name; }
	private:
		yycc_u8string m_Name;

		// User Implementations
	protected:
		virtual bool UserLoad() = 0;
		virtual bool UserSave() = 0;
		virtual void UserReset() = 0;

		// Buffer related functions
	protected:
		void ResizeData(size_t new_size) { m_RawData.resize(new_size); }
		const void* GetDataPtr() const { return m_RawData.data(); }
		void* GetDataPtr() { return m_RawData.data(); }
		size_t GetDataSize() const { return m_RawData.size(); }
	private:
		std::vector<uint8_t> m_RawData;
	};

	class CoreManager {
	public:
		CoreManager(
			const yycc_char8_t* cfg_file_path,
			uint64_t version_identifier,
			std::initializer_list<AbstractSetting*> settings);
		~CoreManager() {}

		// Core functions
	public:
		bool Load();
		bool Save();
		void Reset();

	private:
		using FileHandleGuard_t = std::unique_ptr<FILE, std::function<void(FILE*)>>;
		FileHandleGuard_t GetFileHandle(const yycc_char8_t* mode) const;

		yycc_u8string m_CfgFilePath;
		uint64_t m_VersionIdentifier;
		std::map<yycc_u8string, AbstractSetting*> m_Settings;
	};

#pragma region Setting Presets

	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> || std::is_enum_v<_Ty>, int> = 0>
	class NumberSetting : public AbstractSetting {
	public:
		NumberSetting(const yycc_char8_t* name, _Ty default_value, Constraint<_Ty> constraint = Constraint<_Ty> {}) :
			AbstractSetting(name), m_Data(default_value), m_DefaultData(default_value), m_Constraint(constraint) {}
		virtual ~NumberSetting() {}

		_Ty Get() const { return m_Data; }
		bool Set(_Ty new_data) {
			// validate data
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(new_data))
				return false;
			// assign data
			m_Data = new_data;
			return true;
		}

	protected:
		virtual bool UserLoad() override {
			// read data
			if (sizeof(m_Data) != GetDataSize())
				return false;
			m_Data = *reinterpret_cast<const _Ty*>(GetDataPtr());
			// check data
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(m_Data))
				return false;
			return true;
		}
		virtual bool UserSave() override {
			// write data
			ResizeData(sizeof(m_Data));
			*reinterpret_cast<_Ty*>(GetDataPtr()) = m_Data;
			return true;
		}
		virtual void UserReset() override {
			m_Data = m_DefaultData;
		}

		_Ty m_Data, m_DefaultData;
		Constraint<_Ty> m_Constraint;
	};

	class StringSetting : public AbstractSetting {
	public:
		StringSetting(const yycc_char8_t* name, const yycc_u8string_view& default_value, Constraint<yycc_u8string_view> constraint = Constraint<yycc_u8string_view> {}) :
			AbstractSetting(name), m_Data(), m_DefaultData(), m_Constraint(constraint) {
			m_Data = default_value;
			m_DefaultData = default_value;
		}
		virtual ~StringSetting() {}

		const yycc_u8string& Get() const { return m_Data; }
		bool Set(const yycc_u8string_view& new_data) {
			// check data validation
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(new_data))
				return false;
			// assign data
			m_Data = new_data;
			return true;
		}

	protected:
		virtual bool UserLoad() override {
			// read string length
			size_t string_length;
			if (GetDataSize() < sizeof(string_length))
				return false;
			string_length = *reinterpret_cast<const size_t*>(GetDataPtr());
			// read string body
			if (GetDataSize() != sizeof(string_length) + string_length)
				return false;
			m_Data.assign(
				reinterpret_cast<const yycc_char8_t*>(static_cast<const uint8_t*>(GetDataPtr()) + sizeof(string_length)),
				string_length
			);
			// check data
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(m_Data))
				return false;
			return true;
		}
		virtual bool UserSave() override {
			// allocate result buffer
			size_t string_length = m_Data.size();
			ResizeData(sizeof(string_length) + string_length);
			// get pointer
			uint8_t* ptr = static_cast<uint8_t*>(GetDataPtr());
			// assign string length
			*reinterpret_cast<size_t*>(ptr) = string_length;
			// assign string body
			std::memcpy(ptr + sizeof(string_length), m_Data.data(), string_length);
			return true;
		}
		virtual void UserReset() override {
			m_Data = m_DefaultData;
		}

		yycc_u8string m_Data, m_DefaultData;
		Constraint<yycc_u8string_view> m_Constraint;
	};

#pragma endregion


}
