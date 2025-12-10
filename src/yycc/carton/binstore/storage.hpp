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
#define NS_YYCC_BINSTORE_SERIALIZER ::yycc::carton::binstore::serializer

namespace yycc::carton::binstore::storage {

    /// @brief The strategy when loading from storage.
    enum class LoadStrategy {
        OnlyCurrent,      ///< Only the file with exactly matched version identifier can be loaded. Any other version will cause load error.
        LegacyAndCurrent, ///< Accept all old version and current version. Any other version will cause load error.
        AcceptAll,        ///< Accept all versions.
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
         * @brief All stored settings in raw format.
         * @details Key is the token to already registered settings.
         * Valus is its stored value in raw data format.
         */
        std::map<NS_YYCC_BINSTORE_TYPES::Token, NS_YYCC_BINSTORE_TYPES::ByteArray> raws;

    public:
        Storage();
        ~Storage();

    public:
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load_from_file(const std::filesystem::path& fpath, LoadStrategy strategy);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> load_from_io(const std::istream& s, LoadStrategy strategy);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save_into_file(const std::filesystem::path& fpath);
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<void> save_into_io(const std::ostream& s);
        void reset();

    private:
        bool has_value(NS_YYCC_BINSTORE_TYPES::Token token) const;
        std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray> get_raw_value(NS_YYCC_BINSTORE_TYPES::Token token) const;
        void set_raw_value(NS_YYCC_BINSTORE_TYPES::Token token, NS_YYCC_BINSTORE_TYPES::ByteArray&& ba);

    public:
        template<NS_YYCC_BINSTORE_SERIALIZER::SerDes T>
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<std::pair<GetValueState, NS_YYCC_BINSTORE_SERIALIZER::SerDesValueType<T>>> get_value(
            NS_YYCC_BINSTORE_TYPES::Token token, const T& serdes = T()) {
            // Fetch from raw value.
            auto rv_raw_value = this->get_raw_value(token);
            if (!rv_raw_value.has_value()) return std::unexpected(rv_raw_value.error());
            auto& [state, raw_value] = rv_raw_value.value();

            // Pass into deserializer.
            auto rv_des = serdes.deserialize(raw_value);
            if (rv_des.has_value()) return rv_des.value();

            // If we can not set it, we force reset it and try again.
            this->set_raw_value()
        }
        template<NS_YYCC_BINSTORE_SERIALIZER::SerDes T>
        NS_YYCC_BINSTORE_TYPES::BinstoreResult<SetValueState> set_value(NS_YYCC_BINSTORE_TYPES::Token token,
                                                                        const NS_YYCC_BINSTORE_SERIALIZER::SerDesValueType<T>& value,
                                                                        const T& serdes = T()) {

        }
    };

} // namespace yycc::carton::binstore::storage
