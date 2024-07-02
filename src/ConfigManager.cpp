#include "ConfigManager.hpp"

#include "EncodingHelper.hpp"
#include "IOHelper.hpp"

namespace YYCC::ConfigManager {

#pragma region Core Manager

	CoreManager::CoreManager(
		const yycc_char8_t* cfg_file_path,
		uint64_t version_identifier,
		std::initializer_list<AbstractSetting*> settings) :
		m_CfgFilePath(), m_VersionIdentifier(version_identifier), m_Settings() {
		// assign cfg path
		if (cfg_file_path != nullptr)
			m_CfgFilePath = cfg_file_path;
		// assign settings
		for (auto* setting : settings) {
			m_Settings.try_emplace(setting->GetName(), setting);
		}
	}

	bool CoreManager::Load() {
		// reset all settings first
		Reset();

		// get file handle
		auto fs = this->GetFileHandle(YYCC_U8("rb"));
		if (fs.get() == nullptr) {
			// if we fail to get, it means that we do not have corresponding cfg file.
			// all settings should be reset to default value.
			return true;
		}

		// fetch version info
		uint64_t version_info;
		if (std::fread(&version_info, sizeof(version_info), 1u, fs.get()) != sizeof(version_info))
			return false;
		// check version
		// if read version is greater than we expected,
		// it means that this cfg file is created by the program higer than this.
		// we should not read anything from it.
		// however, for compaitibility reason, we allow read old cfg data.
		if (version_info > m_VersionIdentifier)
			return true;

		// fetch setting item from file
		yycc_u8string name_cache;
		while (true) {
			// try fetch setting name
			// fetch name length
			size_t name_length;
			if (std::fread(&name_length, sizeof(name_length), 1u, fs.get()) != sizeof(name_length)) {
				// we also check whether reach EOF at there.
				if (std::feof(fs.get())) break;
				else return false;
			}
			// fetch name body
			name_cache.resize(name_length);
			if (std::fread(name_cache.data(), name_length, 1u, fs.get()) != name_length)
				return false;

			// get setting data length
			size_t data_length;
			if (std::fread(&data_length, sizeof(data_length), 1u, fs.get()) != sizeof(data_length))
				return false;

			// get matched setting first
			const auto& found = m_Settings.find(name_cache);
			if (found != m_Settings.end()) {
				// found. read data for it
				found->second->ResizeData(data_length);
				if (std::fread(found->second->GetDataPtr(), data_length, 1u, fs.get()) != data_length)
					return false;
				// call user defined load function
				// if fail to parse, reset to default value
				if (!found->second->UserLoad())
					found->second->UserReset();
			} else {
				// fail to find. skip this unknown setting
				if (fseek(fs.get(), static_cast<long>(data_length), SEEK_CUR) != 0)
					return false;
			}
		}

		return true;
	}

	bool CoreManager::Save() {
		// get file handle
		auto fs = this->GetFileHandle(YYCC_U8("wb"));
		// if we fail to get, return false.
		if (fs == nullptr) return false;

		// write config data
		uint64_t version_info = m_VersionIdentifier;
		if (std::fwrite(&version_info, sizeof(version_info), 1u, fs.get()) != sizeof(version_info))
			return false;

		// iterate all data for writing
		for (const auto& pair : m_Settings) {
			// do user defined save
			// if failed, skip this setting
			if (!pair.second->UserSave())
				continue;

			// write setting name
			// write name length
			size_t name_length = pair.first.size();
			if (std::fwrite(&name_length, sizeof(name_length), 1u, fs.get()) != sizeof(name_length))
				return false;
			// write name body
			if (std::fwrite(pair.first.c_str(), name_length, 1u, fs.get()) != name_length)
				return false;

			// write setting daat
			// write data length
			size_t data_length = pair.second->GetDataSize();
			if (std::fwrite(&data_length, sizeof(data_length), 1u, fs.get()) != sizeof(data_length))
				return false;
			// write data body
			if (std::fwrite(pair.second->GetDataPtr(), data_length, 1u, fs.get()) != data_length)
				return false;
		}

		// all settings done, return true
		return true;
	}

	void CoreManager::Reset() {
		for (const auto& pair : m_Settings) {
			pair.second->UserReset();
		}
	}

	CoreManager::FileHandleGuard_t CoreManager::GetFileHandle(const yycc_char8_t* mode) const {
		return CoreManager::FileHandleGuard_t(
			IOHelper::UTF8FOpen(this->m_CfgFilePath.c_str(), mode),
			[](FILE* fs) -> void {
				if (fs != nullptr) std::fclose(fs);
			}
		);
	}

#pragma endregion

}
