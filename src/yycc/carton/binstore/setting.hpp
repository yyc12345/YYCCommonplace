/**
 * @file setting.hpp
 * @brief Setting management for the binstore module.
 */
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

    /// @brief Represents a single setting with a name.
    class Setting {
    public:
        /**
         * @brief Construct a new Setting object.
         * @param[in] name The name of the setting.
         */
        Setting(const std::u8string_view& name);
        ~Setting();
        YYCC_DEFAULT_COPY_MOVE(Setting)

    public:
        /**
         * @brief Get the name of the setting.
         * @return A u8string_view of the setting name.
         */
        std::u8string_view get_name() const;

    private:
        std::u8string name;
    };

    /// @brief Represents a registered setting with both a token and setting information.
    class RegisteredSetting {
    public:
        /**
         * @private
         * @brief Construct a new Registered Setting object.
         * @param[in] token The unique token for the setting.
         * @param[in] setting The setting information.
         */
        RegisteredSetting(NS_YYCC_BINSTORE_TYPES::Token token, Setting&& setting);
        ~RegisteredSetting();
        YYCC_DEFAULT_COPY_MOVE(RegisteredSetting)

    public:
        /**
         * @brief Get the token associated with this registered setting.
         * @return The token.
         */
        NS_YYCC_BINSTORE_TYPES::Token get_token() const;

        /**
         * @brief Get the setting instance associated with this registered setting.
         * @return A const reference to the setting.
         */
        const Setting& get_setting() const;

    private:
        NS_YYCC_BINSTORE_TYPES::Token token; ///< The unique token for the setting.
        Setting setting;                     ///< The setting instance.
    };

    /// @brief Collection of settings with name-based lookup and token-based access.
    class SettingCollection {
    public:
        /// @brief Construct a new Setting Collection object.
        SettingCollection();
        ~SettingCollection();
        YYCC_DEFAULT_COPY_MOVE(SettingCollection)

    public:
        /**
         * @brief Add a setting to the collection.
         * @param[in] setting The setting to add.
         * @return The token assigned to the setting.
         */
        NS_YYCC_BINSTORE_TYPES::Token add_setting(Setting&& setting);
        /**
         * @brief Find a setting by its name.
         * @param[in] name The name of the setting to find.
         * @return An optional token if the setting was found, std::nullopt otherwise.
         */
        std::optional<NS_YYCC_BINSTORE_TYPES::Token> find_name(const std::u8string_view& name) const;
        /**
         * @brief Check if a setting with the given token exists.
         * @param[in] token The token to check.
         * @return True if the setting exists, false otherwise.
         */
        bool has_setting(NS_YYCC_BINSTORE_TYPES::Token token) const;
        /**
         * @brief Get a setting by its token.
         * @param[in] token The token of the setting to get.
         * @return A const reference to the setting.
         * @exception May throw if the token is invalid.
         */
        const Setting& get_setting(NS_YYCC_BINSTORE_TYPES::Token token) const;
        /**
         * @brief Get all registered settings.
         * @return A const reference to the vector of registered settings.
         */
        const std::vector<RegisteredSetting>& all_settings() const;
        /**
         * @brief Get the number of settings in the collection.
         * @return The number of settings.
         */
        size_t length() const;
        /**
         * @brief Check if the collection is empty.
         * @return True if empty, false otherwise.
         */
        bool empty() const;

    private:
        std::map<std::u8string, NS_YYCC_BINSTORE_TYPES::Token> names;
        std::vector<RegisteredSetting> settings;
    };

} // namespace yycc::carton::binstore::setting

#undef NS_YYCC_BINSTORE_TYPES
