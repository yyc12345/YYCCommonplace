#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include "configuration.hpp"
#include "serializer.hpp"
#include <map>
#include <optional>
#include <utility>
#include <filesystem>
#include <istream>
#include <ostream>

#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types
#define NS_YYCC_BINSTORE_CFG ::yycc::carton::binstore::configuration
#define NS_YYCC_BINSTORE_SERDES ::yycc::carton::binstore::serializer

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

    /// @brief The operation result state when getting setting's value.
    enum class GetValueState {
        Ok,      ///< Successfully load value from raw part. We return it.
        Default, ///< Given setting is not presented. We create it from its default value and return.
        Reset,   ///< Can not read from raw part. We reset it in its default value and return.
    };

    /// @brief The operation result state when setting setting's value.
    enum class SetValueState {
        Ok,    ///< Successfully set value.
        Reset, ///< Given value is not meet the requirement of this setting and its value was reset into default value.
    };

    class Storage {
    private:
        /**
         * @brief All stored values of setting in raw format.
         * @details Key is the token to already registered settings.
         * Valus is its stored value in raw data format.
         */
        std::map<NS_YYCC_BINSTORE_TYPES::Token, NS_YYCC_BINSTORE_TYPES::ByteArray> raws;
        NS_YYCC_BINSTORE_CFG::Configuration configuration; ///< The configuration associated with this storage.

    public:
        Storage();
        ~Storage();

    public:
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load_from_file(const std::filesystem::path& fpath, LoadStrategy strategy);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load_from_io(std::istream& s, LoadStrategy strategy);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save_into_file(const std::filesystem::path& fpath);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save_into_io(std::ostream& s);
        /**
         * @brief Reset all values of setting to their default value.
         * @details Please note that all stored value will be wiped before assign them with default value.65
         */
        void reset();

    private:
        bool has_value(NS_YYCC_BINSTORE_TYPES::Token token) const;
        const NS_YYCC_BINSTORE_TYPES::ByteArray& get_raw_value(NS_YYCC_BINSTORE_TYPES::Token token) const;
        void set_raw_value(NS_YYCC_BINSTORE_TYPES::Token token, NS_YYCC_BINSTORE_TYPES::ByteArray&& ba);

    public:
        template<NS_YYCC_BINSTORE_SERDES::SerDes T>
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<std::pair<GetValueState, NS_YYCC_BINSTORE_SERDES::SerDesValueType<T>>> get_value(
            NS_YYCC_BINSTORE_TYPES::Token token, const T& serdes = T{}) {
            // If we have value, we fetch it first
            if (this->has_value(token)) {
                // Get raw value.
                const auto& ba = this->get_raw_value(token);
                // Try to deserialize it.
                auto value = serdes.deserialize(raw_value);
                // If the result is okey, return it.
                if (value.has_value()) {
                    return std::make_pair(GetValueState::Ok, std::move(value.value()));
                }
                // Otherwise we need reset it into default value.
            }

            // If we do not have this setting,
            // or we need reset it into default value,
            // we need execute following code.
            // First decide the return state.
            GetValueState state = this->has_value(token) ? GetValueState::Reset : GetValueState::Default;
            {
                // Fetch the default raw data.
                auto ba = serdes.reset();
                // Set it for this setting.
                this->set_raw_value(token, std::move(ba));
            }
            // The get it and deserialize it.
            const auto& ba = this->get_raw_value(token);
            auto value = serdes.deserialize(ba);
            // Default must can be deserialized.
            // If not, throw exception.
            if (value.has_value()) {
                return std::make_pair(state, std::move(value));
            } else {
                throw std::logic_error("default value can not be deserialized");
            }
        }
        template<NS_YYCC_BINSTORE_SERDES::SerDes T>
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<SetValueState> set_value(NS_YYCC_BINSTORE_TYPES::Token token,
                                                                        const NS_YYCC_BINSTORE_SERDES::SerDesValueType<T>& value,
                                                                        const T& serdes = T{}) {
            // First we try assign it.
            {
                // Convert it into raw format.
                auto ba = serdes.serialize(value);
                // If we can not serialize it, we need reset it into default value.
                // Otherwise assign it and return.
                if (ba.has_value()) {
                    this->set_raw_value(token, )
                }
            }
        }
    };

} // namespace yycc::carton::binstore::storage

#undef NS_YYCC_BINSTORE_SERDES
#undef NS_YYCC_BINSTORE_CFG
#undef NS_YYCC_BINSTORE_TYPES
