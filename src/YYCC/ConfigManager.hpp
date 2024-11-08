#pragma once
#include "YYCCInternal.hpp"

#include "Constraints.hpp"
#include <memory>
#include <vector>
#include <map>
#include <initializer_list>
#include <type_traits>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cstring>

/**
 * @brief Universal configuration manager
 * @details For how to use this namespace, please see \ref config_manager.
*/
namespace YYCC::ConfigManager {
	
	/**
	 * @brief The load result of loading config.
	*/
	enum class ConfigLoadResult {
		OK = 0, ///< Success load configs.
		Created = 1 << 0, ///< Given file is not existing, we create all configs in default values.
		ForwardNew = 1 << 1,	///< Detect the config file created by higher version. We create all configs in default values.
		Migrated = 1 << 2, ///< Detect the config file created by lower version. We try migrate configs written in it.
		BrokenFile = 1 << 3, ///< Given file has bad format. Thus some configs are kept as its default values.
		ItemError = 1 << 4 ///< Some config can not be recognized from the data read from file so they are reset to default value.
	};
	using UnderlyingConfigLoadResult_t = std::underlying_type_t<ConfigLoadResult>;

	/// @brief The base class of every setting.
	/// @details Programmer can inherit this class and implement essential functions to create custom setting.
	class AbstractSetting {
		friend class CoreManager;
	public:
		/**
		 * @brief Construct a setting
		 * @param[in] name The name of this setting.
		 * @exception std::invalid_argument Name of setting is empty.
		*/
		AbstractSetting(const yycc_u8string_view& name);
		virtual ~AbstractSetting();
		YYCC_DEL_CLS_COPY_MOVE(AbstractSetting);

		// Name interface
	public:
		/// @brief Get name of this setting.
		/// @details Name was used in storing setting in file.
		const yycc_u8string& GetName() const;
	private:
		yycc_u8string m_Name;

		// User Implementations
	protected:
		/// @brief User implemented custom load function
		/// @remarks 
		/// In this function, programmer should read data from internal buffer 
		/// and store it to its own another internal variables.
		/// @return True if success, otherwise false.
		virtual bool UserLoad() = 0;
		/// @brief User implemented custom save function
		/// @remarks 
		/// In this function, programmer should write data, 
		/// which is stored in another variavle by it own, to internal buffer.
		/// @return True if success, otherwise false.
		virtual bool UserSave() = 0;
		/// @brief User implemented custom reset function
		/// @remarks In this function, programmer should reset its internal variable to default value.
		virtual void UserReset() = 0;

		// Buffer related functions
	protected:
		/// @brief Resize internal buffer to given size.
		/// @remarks It is usually used in UserSave.
		/// @param[in] new_size The new size of internal buffer.
		void ResizeData(size_t new_size);
		/// @brief Get data pointer to internal buffer.
		/// @remarks It is usually used in UserLoad.
		const void* GetDataPtr() const;
		/// @brief Get mutable data pointer to internal buffer.
		/// @remarks It is usually used in UserSave.
		void* GetDataPtr();
		/// @brief Get the length of internal buffer.
		size_t GetDataSize() const;
	private:
		std::vector<uint8_t> m_RawData;
	};
	
	/// @brief Settings manager and config file reader writer.
	class CoreManager {
	public:
		/**
		 * @brief Build core manager.
		 * @param[in] cfg_file_path The path to config file.
		 * @param[in] version_identifier The identifier of version. Higher is newer. Lower config will try doing migration.
		 * @param[in] settings An initializer list containing pointers to all managed settings.
		*/
		CoreManager(
			const yycc_u8string_view& cfg_file_path,
			uint64_t version_identifier,
			std::initializer_list<AbstractSetting*> settings);
		~CoreManager() {}
		YYCC_DEL_CLS_COPY_MOVE(CoreManager);

		// Core functions
	public:
		/// @brief Load settings from file.
		/// @details Before loading, all settings will be reset to default value first.
		/// @return What happend when loading config. This function always success.
		ConfigLoadResult Load();
		/// @brief Save settings to file.
		/// @return True if success, otherwise false.
		bool Save();
		/// @brief Reset all settings to default value.
		void Reset();

	private:
		yycc_u8string m_CfgFilePath;
		uint64_t m_VersionIdentifier;
		std::map<yycc_u8string, AbstractSetting*> m_Settings;
	};

#pragma region Setting Presets

	/**
	 * @brief Arithmetic (integral, floating point, bool) and enum type setting
	 * @tparam _Ty The internal stored type belongs to arithmetic type.
	*/
	template<typename _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty> || std::is_enum_v<_Ty>, int> = 0>
	class NumberSetting : public AbstractSetting {
	public:
		/**
		 * @brief Construct arithmetic type setting.
		 * @param[in] name The name of this setting.
		 * @param[in] default_value The default value of this setting.
		 * @param[in] constraint The constraint applied to this setting.
		 * @exception std::invalid_argument Name of setting is empty.
		*/
		NumberSetting(
			const yycc_u8string_view& name, _Ty default_value,
			Constraints::Constraint<_Ty> constraint = Constraints::Constraint<_Ty> {}) :
			AbstractSetting(name), m_Data(default_value), m_DefaultData(default_value), m_Constraint(constraint) {}
		virtual ~NumberSetting() {}
		YYCC_DEL_CLS_COPY_MOVE(NumberSetting);

		/// @brief Get stored data in setting.
		_Ty Get() const { return m_Data; }
		/**
		 * @brief Set data to setting.
		 * @param[in] new_data The new data.
		 * @return True if success, otherwise false (given value is invalid)
		*/
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
		Constraints::Constraint<_Ty> m_Constraint;
	};

	/// @brief String type setting
	class StringSetting : public AbstractSetting {
	public:
		/**
		 * @brief Construct string setting
		 * @param[in] name The name of this setting.
		 * @param[in] default_value The default value of this setting.
		 * @param[in] constraint The constraint applied to this setting.
		 * @exception std::invalid_argument Name of setting is empty.
		*/
		StringSetting(
			const yycc_u8string_view& name, const yycc_u8string_view& default_value,
			Constraints::Constraint<yycc_u8string> constraint = Constraints::Constraint<yycc_u8string> {}) :
			AbstractSetting(name), m_Data(), m_DefaultData(), m_Constraint(constraint) {
			m_Data = default_value;
			m_DefaultData = default_value;
		}
		virtual ~StringSetting() {}
		YYCC_DEL_CLS_COPY_MOVE(StringSetting);

		/// @brief Get reference to stored string.
		const yycc_u8string& Get() const { return m_Data; }
		/**
		 * @brief Set string data to setting.
		 * @param[in] new_data The new string data.
		 * @return True if success, otherwise false (given value is invalid)
		*/
		bool Set(const yycc_u8string_view& new_data) {
			// check data validation
			yycc_u8string new_data_cache(new_data);
			if (m_Constraint.IsValid() && !m_Constraint.m_CheckFct(new_data_cache))
				return false;
			// assign data
			m_Data = std::move(new_data_cache);
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
		Constraints::Constraint<yycc_u8string> m_Constraint;
	};

#pragma endregion


}
