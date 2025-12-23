#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include "setting.hpp"

#define NS_YYCC_BINSTORE ::yycc::carton::binstore
#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types

namespace yycc::carton::binstore::configuration {

    /// @brief Configuration class that holds version and settings for the binstore module.
    class Configuration {
    public:
        /**
         * @brief Construct a new Configuration object.
         * @param[in] version The version identifier for this configuration.
         * @param[in] settings The settings collection to associate with this configuration.
         */
        Configuration(NS_YYCC_BINSTORE_TYPES::VersionIdentifier version, NS_YYCC_BINSTORE::setting::SettingCollection&& settings);
        ~Configuration();
        YYCC_DEFAULT_COPY_MOVE(Configuration)

    public:
        /**
         * @brief Get the version identifier of this configuration.
         * @return The version identifier.
         */
        NS_YYCC_BINSTORE_TYPES::VersionIdentifier get_version() const;
        /**
         * @brief Get the settings collection associated with this configuration.
         * @return A const reference to the settings collection.
         */
        const NS_YYCC_BINSTORE::setting::SettingCollection& get_settings() const;

    private:
        NS_YYCC_BINSTORE_TYPES::VersionIdentifier version;     ///< The version of current configuration struct.
        NS_YYCC_BINSTORE::setting::SettingCollection settings; ///< All registered settings.
    };

} // namespace yycc::carton::binstore::configuration

#undef NS_YYCC_BINSTORE_TYPES
#undef NS_YYCC_BINSTORE
