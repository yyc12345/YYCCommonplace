#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <optional>

#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types

namespace yycc::carton::binstore::setting {

    class Setting {
    public:
        Setting(const std::u8string_view& name);
        ~Setting();
        YYCC_DEFAULT_COPY_MOVE(Setting)

    public:
        std::u8string_view get_name() const;

    private:
        std::u8string name;
    };

    class RegisteredSetting {
    public:
        RegisteredSetting(NS_YYCC_BINSTORE_TYPES::Token token, Setting&& setting);
        ~RegisteredSetting();
        YYCC_DEFAULT_COPY_MOVE(RegisteredSetting)

    public:
        NS_YYCC_BINSTORE_TYPES::Token get_token() const;
        const Setting& get_setting() const;

    private:
        NS_YYCC_BINSTORE_TYPES::Token token;
        Setting setting;
    };

    class SettingCollection {
    public:
        SettingCollection();
        ~SettingCollection();
        YYCC_DEFAULT_COPY_MOVE(SettingCollection)

    public:
        NS_YYCC_BINSTORE_TYPES::Token add_setting(Setting&& setting);
        std::optional<NS_YYCC_BINSTORE_TYPES::Token> find_name(const std::u8string_view& name) const;
        bool has_setting(NS_YYCC_BINSTORE_TYPES::Token token) const;
        const Setting& get_setting(NS_YYCC_BINSTORE_TYPES::Token token) const;
        const std::vector<RegisteredSetting>& all_settings() const;
        size_t length() const;
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_BINSTORE_TYPES::Token> names;
        std::vector<RegisteredSetting> settings;
    };

}

#undef NS_YYCC_BINSTORE_TYPES
