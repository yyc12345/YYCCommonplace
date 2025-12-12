#include "storage.hpp"
#include <fstream>

#define TYPES ::yycc::carton::binstore::types
#define CFG ::yycc::carton::binstore::configuration
#define SERDES ::yycc::carton::binstore::serializer

namespace yycc::carton::binstore::storage {

#pragma region Storage Class

    Storage::Storage(CFG::Configuration&& cfg) : cfg(std::move(cfg)), raws() {}

    Storage::~Storage() {}

    TYPES::BinstoreResult<void> Storage::load_from_file(const std::filesystem::path& fpath, LoadStrategy strategy) {
        std::ifstream fs(fpath, std::ios::binary);
        if (fs.is_open()) {
            auto rv = this->load(fs, strategy);
            fs.close();
            return rv;
        } else {
            return std::unexpected(TYPES::BinstoreError::Io);
        }
    }

    TYPES::BinstoreResult<void> Storage::load(std::istream& s, LoadStrategy strategy) {
        return TYPES::BinstoreResult<void>();
    }

    TYPES::BinstoreResult<void> Storage::save_into_file(const std::filesystem::path& fpath) {
        std::ofstream fs(fpath, std::ios::binary);
        if (fs.is_open()) {
            auto rv = this->save(fs);
            fs.close();
            return rv;
        } else {
            return std::unexpected(TYPES::BinstoreError::Io);
        }
    }

    TYPES::BinstoreResult<void> Storage::save(std::ostream& s) {
        return TYPES::BinstoreResult<void>();
    }

    bool Storage::has_setting(TYPES::Token token) const {
        return this->cfg.get_settings().has_setting(token);
    }

    bool Storage::is_setting_stored(TYPES::Token token) const {
        if (!this->has_setting(token)) throw std::logic_error("given setting token is invalid");
        return this->raws.contains(token);
    }

    const TYPES::ByteArray& Storage::get_raw_value(TYPES::Token token) const {
        if (!this->has_setting(token)) throw std::logic_error("given setting token is invalid");
        auto finder = this->raws.find(token);
        if (finder != this->raws.end()) {
            return finder->second;
        } else {
            throw std::logic_error("given setting has not been stored yet");
        }
    }

    void Storage::set_raw_value(TYPES::Token token, TYPES::ByteArray&& ba) {
        if (!this->has_setting(token)) throw std::logic_error("given setting token is invalid");
        this->raws.insert_or_assign(token, std::move(ba));
    }

#pragma endregion

} // namespace yycc::carton::binstore::storage
