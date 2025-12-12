#include "storage.hpp"
#include "../../num/safe_cast.hpp"
#include <fstream>
#include <concepts>
#include <type_traits>

#define TYPES ::yycc::carton::binstore::types
#define CFG ::yycc::carton::binstore::configuration
#define SERDES ::yycc::carton::binstore::serializer
#define SAFECAST ::yycc::num::safe_cast

namespace yycc::carton::binstore::storage {

#pragma region Read and Write Helper Functions

    static bool is_eof(std::istream& s) {
        // Peek next value and check bit flag.
        s.peek();
        return s.eof();
    }

    static bool read_buffer(std::istream& s, void* buffer, size_t length) {
        // Cast length
        auto rv_length = SAFECAST::try_to<std::streamsize>(length);
        if (!rv_length.has_value()) return false;
        auto cast_length = rv_length.value();

        // Read data.
        s.read(static_cast<std::istream::char_type*>(buffer), cast_length);
        // Check read count.
        if (s.gcount() != cast_length) return false;
        // Return IO status.
        return s.good();
    }

    template<typename T>
        requires std::is_pod_v<T>
    static bool read_pod(std::istream& s, T& val) {
        return read_buffer(s, &val, sizeof(T));
    }

    static bool read_byte_array(std::istream& s, TYPES::ByteArray& ba) {
        size_t length = 0;
        if (!read_pod(s, length)) return false;

        // Resize byte array.
        // There is an exception may be thrown that resized length is too large.
        // We need capture it and return error.
        try {
            ba.resize_data(length);
        } catch (const std::exception&) {
            return false;
        }
        // Read data into byte array.
        read_buffer(s, ba.get_data_ptr(), length);
        // Okey
        return true;
    }

    static bool read_u8string(std::istream& s, std::u8string& strl) {
        size_t length = 0;
        if (!read_pod(s, length)) return false;

        // Same reason for try-catch like ByteArray.
        try {
            strl.resize(length);
        } catch (const std::exception&) {
            return false;
        }
        // Read data into byte array.
        read_buffer(s, strl.data(), length);
        // Okey
        return true;
    }

    static bool write_buffer(std::ostream& s, const void* buffer, size_t length) {
        // Cast length
        auto rv_length = SAFECAST::try_to<std::streamsize>(length);
        if (!rv_length.has_value()) return false;
        auto cast_length = rv_length.value();

        // Write data.
        s.write(static_cast<const std::istream::char_type*>(buffer), cast_length);
        // There is no function to tell how many data was written,
        // so directly return.
        return s.good();
    }

    template<typename T>
        requires std::is_pod_v<T>
    static bool write_pod(std::ostream& s, const T& val) {
        return write_buffer(s, &val, sizeof(T));
    }

    static bool write_byte_array(std::ostream& s, const TYPES::ByteArray& ba) {
        // Write length header.
        auto length = ba.get_data_size();
        if (!write_pod(s, length)) return false;
        // Write body
        return write_buffer(s, ba.get_data_ptr(), length);
    }

    static bool write_u8string(std::ostream& s, const std::u8string_view& sv) {
        // Write length header.
        auto length = sv.length();
        if (!write_pod(s, length)) return false;
        // Write body
        return write_buffer(s, sv.data(), length);
    }

#pragma endregion

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
        // Before loading, we need clear all stored raw data first.
        this->raws.clear();

        // Read identifier
        TYPES::VersionIdentifier version;
        if (!read_pod(s, version)) return std::unexpected(TYPES::BinstoreError::Io);

        // Check identifier with strategy
        {
            bool ok_for_read = false;
            auto expected_version = this->cfg.get_version();
            switch (strategy) {
                case LoadStrategy::OnlyCurrent:
                    ok_for_read = (version == expected_version);
                    break;
                case LoadStrategy::MigrateOld:
                    ok_for_read = (version <= expected_version);
                    break;
                case LoadStrategy::AcceptAll:
                    ok_for_read = true;
                    break;
            }
            if (!ok_for_read) return std::unexpected(TYPES::BinstoreError::BadVersion);
        }

        // Read settings one by one.
        const auto& settings = this->cfg.get_settings();
        while (!is_eof(s)) {
            // Read setting name
            std::u8string setting_name;
            if (!read_u8string(s, setting_name)) return std::unexpected(TYPES::BinstoreError::Io);

            // Read setting body
            TYPES::ByteArray ba;
            if (!read_byte_array(s, ba)) return std::unexpected(TYPES::BinstoreError::Io);

            // Check whether there is such setting and its token.
            auto token_finder = settings.find_name(setting_name);
            // If no such name, skip this setting.
            if (!token_finder.has_value()) continue;
            auto token = token_finder.value();

            // If there is duplicated entry, report error
            if (this->raws.contains(token)) std::unexpected(TYPES::BinstoreError::DuplicatedAssign);
            // Otherwise insert it
            this->raws.emplace(token, std::move(ba));
        }

        // Okey
        return {};
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
        // Write version identifier
        auto version = this->cfg.get_version();
        if (!write_pod(s, version)) return std::unexpected(TYPES::BinstoreError::Io);

        // Write settings one by one
        const auto& settings = this->cfg.get_settings();
        for (const auto& [setting_token, setting_value] : this->raws) {
            // Fetch setting name first
            auto setting_name = settings.get_setting(setting_token).get_name();

            // Write name
            if (!write_u8string(s, setting_name)) return std::unexpected(TYPES::BinstoreError::Io);
            // Write setting body
            if (!write_byte_array(s, setting_value)) return std::unexpected(TYPES::BinstoreError::Io);
        }

        // Okey
        return {};
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
