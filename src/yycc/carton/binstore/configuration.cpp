#include "configuration.hpp"

#define BINSTORE ::yycc::carton::binstore
#define TYPES ::yycc::carton::binstore::types

namespace yycc::carton::binstore::configuration {

    Configuration::Configuration(TYPES::VersionIdentifier version, BINSTORE::setting::SettingCollection&& settings) :
        version(version), settings(std::move(settings)) {}

    Configuration::~Configuration() {}

    TYPES::VersionIdentifier Configuration::get_version() const {
        return this->version;
    }

    const BINSTORE::setting::SettingCollection& Configuration::get_settings() const {
        return this->settings;
    }

} // namespace yycc::carton::binstore::configuration
