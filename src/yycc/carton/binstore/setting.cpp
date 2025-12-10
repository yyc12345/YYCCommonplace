#include "setting.hpp"
#include "../../patch/format.hpp"
#include <stdexcept>

#define TYPES ::yycc::carton::binstore::types
#define FORMAT ::yycc::patch::format

namespace yycc::carton::binstore::setting {

#pragma region Setting Class

    Setting::Setting(const std::u8string_view& name) : name(name) {
        if (name.empty()) {
            throw std::logic_error("the name of setting should not be empty");
        }
    }

    Setting::~Setting() {}

    std::u8string_view Setting::get_name() const {
        return this->name;
    }

#pragma endregion

#pragma region RegisteredSetting Class

    RegisteredSetting::RegisteredSetting(TYPES::Token token, Setting&& setting) : token(token), setting(std::move(setting)) {}

    RegisteredSetting::~RegisteredSetting() {}

    TYPES::Token RegisteredSetting::get_token() const {
        return this->token;
    }

    const Setting& RegisteredSetting::get_setting() const {
        return this->setting;
    }

#pragma endregion

#pragma region SettingCollection Class

    SettingCollection::SettingCollection() : names(), settings() {}

    SettingCollection::~SettingCollection() {}

    TYPES::Token SettingCollection::add_setting(Setting&& setting) {
        auto token = this->settings.size();

        std::u8string name(setting.get_name());
        auto [_, ok] = this->names.try_emplace(name, token);
        if (!ok) {
            throw std::logic_error(FORMAT::format("duplicated setting name {}", name));
        }

        this->settings.emplace_back(RegisteredSetting(token, std::move(setting)));
        return token;
    }

    std::optional<TYPES::Token> SettingCollection::find_name(const std::u8string_view& name) const {
        auto finder = this->names.find(std::u8string(name));
        if (finder == this->names.end()) return std::nullopt;
        else return finder->second;
    }

    bool SettingCollection::has_setting(TYPES::Token token) const {
        return token < this->settings.size();
    }

    const Setting& SettingCollection::get_setting(TYPES::Token token) const {
        return this->settings.at(token).get_setting();
    }

    const std::vector<RegisteredSetting>& SettingCollection::all_settings() const {
        return this->settings;
    }

    size_t SettingCollection::length() const {
        return this->settings.size();
    }

    bool SettingCollection::empty() const {
        return this->settings.empty();
    }

#pragma endregion

} // namespace yycc::carton::binstore::setting