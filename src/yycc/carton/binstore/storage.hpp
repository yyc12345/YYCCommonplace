/**
 * @file storage.hpp
 * @brief Storage management for the binstore module.
 */
#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include "configuration.hpp"
#include "serdes.hpp"
#include <map>
#include <optional>
#include <utility>
#include <filesystem>
#include <istream>
#include <ostream>
#include <stdexcept>

#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types
#define NS_YYCC_BINSTORE_CFG ::yycc::carton::binstore::configuration
#define NS_YYCC_BINSTORE_SERDES ::yycc::carton::binstore::serdes

namespace yycc::carton::binstore::storage {

    /// @brief The strategy when loading from storage.
    enum class LoadStrategy {
        /**
         * @brief Only accept matched version.
         * @details
         * Any loading of other versions will explicitly cause error return.
         * This is convenient for developer who want control migration by themselves.
         * They can specify this strategy and try to load data with different version configurations 
         * from older to newer one by one.
         */
        OnlyCurrent,
        /**
         * @brief Try to migrate old version.
         * @details
         * Accept mateched and any older versions.
         * Any newer versions will explicitly cause error return.
         * This strategy is good for developer who are lazy to treat this manually.
         */
        MigrateOld,
        /**
         * @brief Accept all version.
         * @details
         * This strategy is not suggested.
         * This strategy only suit for quick demo.
         */
        AcceptAll,
    };

    /// @brief Main storage class for managing binary settings storage.
    class Storage {
    private:
        /**
         * @brief All stored values of setting in raw format.
         * @details Key is the token to already registered settings.
         * Valus is its stored value in raw data format.
         */
        std::map<NS_YYCC_BINSTORE_TYPES::Token, NS_YYCC_BINSTORE_TYPES::ByteArray> raws;
        NS_YYCC_BINSTORE_CFG::Configuration cfg; ///< The configuration associated with this storage.

    public:
        /**
         * @brief Construct a new Storage object.
         * @param[in] cfg The configuration to associate with this storage.
         */
        Storage(NS_YYCC_BINSTORE_CFG::Configuration&& cfg);
        ~Storage();
        YYCC_DEFAULT_COPY_MOVE(Storage)

    public:
        /**
         * @brief Load settings from a file.
         * @param[in] fpath Path to the file to load from.
         * @param[in] strategy The load strategy to use. Defaults to MigrateOld.
         * @return A BinstoreResult indicating success or failure.
         */
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load_from_file(const std::filesystem::path& fpath,
                                                                    LoadStrategy strategy = LoadStrategy::MigrateOld);

        /**
         * @brief Load settings from an input stream.
         * @param[in] s The input stream to load from.
         * @param[in] strategy The load strategy to use. Defaults to MigrateOld.
         * @return A BinstoreResult indicating success or failure.
         */
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load(std::istream& s, LoadStrategy strategy = LoadStrategy::MigrateOld);

        /**
         * @brief Save settings to a file.
         * @param[in] fpath Path to the file to save to.
         * @return A BinstoreResult indicating success or failure.
         */
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save_into_file(const std::filesystem::path& fpath);

        /**
         * @brief Save settings to an output stream.
         * @param[in] s The output stream to save to.
         * @return A BinstoreResult indicating success or failure.
         */
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save(std::ostream& s);

        /**
         * @brief Clear all raw data saved in internal cache.
         * @details This will cause every setting was set in default value when user fetching them.
         */
        void clear();

    private:
        /**
         * @brief Check whether given token is a valid token to registered settings.
         * @param[in] token Token for checking.
         * @return True if it is a registered setting, otherwise false.
         */
        bool has_setting(NS_YYCC_BINSTORE_TYPES::Token token) const;
        /**
         * @brief Check whether given token is stored in raw value dictionary.
         * @param[in] token Token for checking.
         * @return True if it is stored, otherwise false.
         * @exception std::logic_error Given token is not point to a registered setting.
         */
        bool is_setting_stored(NS_YYCC_BINSTORE_TYPES::Token token) const;
        /**
         * @brief Get raw value of given setting.
         * @param[in] token Token for fetching.
         * @return The const reference to given setting's raw data.
         * @exception std::logic_error Given token is not point to a registered setting.
         * @exception std::logic_error Given token pointed to setting is not stored yet.
         */
        const NS_YYCC_BINSTORE_TYPES::ByteArray& get_raw_value(NS_YYCC_BINSTORE_TYPES::Token token) const;
        /**
         * @brief Set raw value for given setting.
         * @param[in] token Token for setting.
         * @param[in] ba The right-value raw value for setting.
         * @exception std::logic_error Given token is not point to a registered setting.
         */
        void set_raw_value(NS_YYCC_BINSTORE_TYPES::Token token, NS_YYCC_BINSTORE_TYPES::ByteArray&& ba);

    public:
        /**
         * @brief Reset given setting into default value.
         * @tparam T The SerDes applied to this setting.
         * @param[in] token Token to setting for resetting.
         * @param[in] serdes Optional SerDes passed if it can not be constructed from default ctor.
         */
        template<NS_YYCC_BINSTORE_SERDES::SerDes T>
        void reset_value(NS_YYCC_BINSTORE_TYPES::Token token, const T& serdes = T{}) {
            // Check whether has this setting.
            if (!has_setting(token)) throw std::logic_error("given setting token is invalid");
            // Reset it.
            auto ba = serdes.reset();
            this->set_raw_value(token, std::move(ba));
        }
        /**
         * @brief Get given setting's value.
         * @tparam T The SerDes applied to this setting.
         * @param[in] token Token to setting for fetching value.
         * @param[in] serdes Optional SerDes passed if it can not be constructed from default ctor.
         * @return Fetched value. If there is no such setting before or fail to deserialize underlying data,
         * Default value will be forcely set before the return of this function.
         */
        template<NS_YYCC_BINSTORE_SERDES::SerDes T>
        NS_YYCC_BINSTORE_SERDES::SerDesValueType<T> get_value(NS_YYCC_BINSTORE_TYPES::Token token, const T& serdes = T{}) {
            // Check whether has this setting.
            if (!has_setting(token)) throw std::logic_error("given setting token is invalid");

            // If we have stored raw value, we fetch it first.
            if (this->is_setting_stored(token)) {
                // Get raw value.
                const auto& ba = this->get_raw_value(token);
                // Try to deserialize it.
                auto value = serdes.deserialize(ba);
                // If the result is okey, return it.
                if (value.has_value()) {
                    return value.value();
                }
                // Otherwise we need reset it into default value.
            }

            // If we do not have this setting, or we need reset it into default value
            // due to failed deserialization, we need execute following code.
            // Reset its value first
            this->reset_value(token, serdes);
            // The re-fetch its raw value and deserialize it.
            const auto& ba = this->get_raw_value(token);
            auto value = serdes.deserialize(ba);
            // Default value must can be deserialized.
            // If not, throw exception.
            if (value.has_value()) {
                return value.value();
            } else {
                throw std::logic_error("default value must can be deserialized");
            }
        }
        /**
         * @brief Set given setting's value.
         * @tparam T The SerDes applied to this setting.
         * @param[in] token Token to setting for setting value.
         * @param[in] value The value to set.
         * @param[in] serdes Optional SerDes passed if it can not be constructed from default ctor.
         * @return True if the setting was set to your given value,
         * otherwise false, the value was set as default value because given value can not be serialized.
         */
        template<NS_YYCC_BINSTORE_SERDES::SerDes T>
        bool set_value(NS_YYCC_BINSTORE_TYPES::Token token,
                       const NS_YYCC_BINSTORE_SERDES::SerDesValueType<T>& value,
                       const T& serdes = T{}) {
            // Check whether has this setting.
            if (!has_setting(token)) throw std::logic_error("given setting token is invalid");

            // We try to serialize given value first.
            auto rv_ser = serdes.serialize(value);
            // If we can serialize it, we directly use it,'
            // otherwise we need fetch it from default value.
            auto success_ser = rv_ser.has_value();
            auto ba = success_ser ? std::move(rv_ser.value()) : serdes.reset();
            // Assign it to setting's raw value.
            this->set_raw_value(token, std::move(ba));

            // Return result
            return success_ser;
        }
    };

} // namespace yycc::carton::binstore::storage

#undef NS_YYCC_BINSTORE_SERDES
#undef NS_YYCC_BINSTORE_CFG
#undef NS_YYCC_BINSTORE_TYPES
