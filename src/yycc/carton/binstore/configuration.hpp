#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include "setting.hpp"

#define NS_YYCC_BINSTORE ::yycc::carton::binstore
#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types

namespace yycc::carton::binstore::configuration {

    class Configuration {
    public:
        Configuration(NS_YYCC_BINSTORE_TYPES::VersionIdentifier version, NS_YYCC_BINSTORE::setting::SettingCollection&& settings);
        ~Configuration();
        YYCC_DEFAULT_COPY_MOVE(Configuration)

    public:
        NS_YYCC_BINSTORE_TYPES::VersionIdentifier get_version() const;
        const NS_YYCC_BINSTORE::setting::SettingCollection& get_settings() const;

    private:
        NS_YYCC_BINSTORE_TYPES::VersionIdentifier version;     ///< The version of current configuration struct.
        NS_YYCC_BINSTORE::setting::SettingCollection settings; ///< All registered settings.
    };

} // namespace yycc::carton::binstore::configuration

#undef NS_YYCC_BINSTORE_TYPES
#undef NS_YYCC_BINSTORE
