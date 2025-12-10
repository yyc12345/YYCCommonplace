#pragma once
#include "../../macro/class_copy_move.hpp"
#include "types.hpp"
#include <optional>
#include <string>
#include <limits>
#include <concepts>
#include <type_traits>

#define NS_YYCC_BINSTORE_TYPES ::yycc::carton::binstore::types

namespace yycc::carton::binstore::serializer {

    // TODO: Support numeric list and string list SerDes.

    /**
     * @brief The concept for checking whether given type is a valid binstore serializer and deserializer (SerDes).
     * @details
     * A valid SerDes must have a type name or alias called "ValueType"
     * indicating the corresponding setting type which read and write with underlying ByteArray.
     * Please note that the type of this value must can be placed into \c std::optional.
     * 
     * SerDes must have following 3 functions:
     * \li Function named "serialize" will accept an const reference of setting data,
     * and convert it into underlying ByteArray format in return value.
     * If any error occurs, it will return \c std::nullopt instead.
     * \li Function named "deserialize" will do the opposite operation with "serialize".
     * It convert ByteArray into setting desired type, or \c std::nullopt if any error occurs.
     * \li Function named "reset" will built an ByteArray which holding default value of this option.
     * This is usually used when fail to deserialize or there is no such setting.
     */
    template<typename T>
    concept SerDes = requires(const T& t, const NS_YYCC_BINSTORE_TYPES::ByteArray& cba) {
        // Check whether there is T::ValueType type
        typename T::ValueType;
        // Check whether there are required member functions and they have correct signatures.
        { t.serialize(std::declval<const typename T::ValueType&>()) } -> std::same_as<std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray>>;
        { t.deserialize(cba) } -> std::same_as<std::optional<typename T::ValueType>>;
        { t.reset() } -> std::same_as<NS_YYCC_BINSTORE_TYPES::ByteArray>;
    };

    /// @brief A convenient alias to the value type of SerDes.
    template<SerDes T>
    using SerDesValueType = T::ValueType;

    template<std::integral T,
             auto TMin = std::numeric_limits<T>::min(),
             auto TMax = std::numeric_limits<T>::max(),
             auto TDefault = static_cast<T>(0)>
    struct IntegralSerDes {
        YYCC_DEFAULT_COPY_MOVE(IntegralSerDes)

        static_assert(TMin <= TMax);
        static_assert(TDefault >= TMin && TDefault <= TMax);

        using ValueType = T;
        static constexpr size_t VALUE_SIZE = sizeof(ValueType);

        std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray> serialize(const ValueType& value) const {
            if (value > TMax || value < TMin) return std::nullopt;

            NS_YYCC_BINSTORE_TYPES::ByteArray ba;
            ba.ResizeData(VALUE_SIZE);
            std::memcpy(ba.GetDataPtr(), &value, VALUE_SIZE);
            return ba;
        }

        std::optional<ValueType> deserialize(const NS_YYCC_BINSTORE_TYPES::ByteArray& ba) const {
            if (ba.GetDataSize() != VALUE_SIZE) return std::nullopt;
            ValueType value;
            std::memcpy(&value, ba.GetDataPtr(), VALUE_SIZE);

            if (value > TMax || value < TMin) return std::nullopt;
            return value;
        }

        NS_YYCC_BINSTORE_TYPES::ByteArray reset() const { return this->serialize(TDefault).value(); }
    };

    template<std::floating_point T,
             auto TMin = std::numeric_limits<T>::lowest(),
             auto TMax = std::numeric_limits<T>::max(),
             auto TDefault = static_cast<T>(0)>
    struct FloatingPointSerDes {
        YYCC_DEFAULT_COPY_MOVE(FloatingPointSerDes)

        static_assert(std::isfinite(TMin));
        static_assert(std::isfinite(TMax));
        static_assert(TMin <= TMax);
        static_assert(TDefault >= TMin && TDefault <= TMax);

        using ValueType = T;
        static constexpr size_t VALUE_SIZE = sizeof(ValueType);

        std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray> serialize(const ValueType& value) const {
            if (value > TMax || value < TMin) return std::nullopt;

            NS_YYCC_BINSTORE_TYPES::ByteArray ba;
            ba.ResizeData(VALUE_SIZE);
            std::memcpy(ba.GetDataPtr(), &value, VALUE_SIZE);
            return ba;
        }

        std::optional<ValueType> deserialize(const NS_YYCC_BINSTORE_TYPES::ByteArray& ba) const {
            if (ba.GetDataSize() != VALUE_SIZE) return std::nullopt;

            ValueType value;
            std::memcpy(&value, ba.GetDataPtr(), VALUE_SIZE);
            if (value > TMax || value < TMin) return std::nullopt;
            return value;
        }

        NS_YYCC_BINSTORE_TYPES::ByteArray reset() const { return this->serialize(TDefault).value(); }
    };

    template<bool TDefault = false>
    struct BoolSerDes {
        YYCC_DEFAULT_COPY_MOVE(BoolSerDes)

        using ValueType = bool;
        static constexpr size_t VALUE_SIZE = sizeof(ValueType);

        std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray> serialize(const ValueType& value) const {
            NS_YYCC_BINSTORE_TYPES::ByteArray ba;
            ba.ResizeData(VALUE_SIZE);
            std::memcpy(ba.GetDataPtr(), &value, VALUE_SIZE);
            return ba;
        }

        std::optional<ValueType> deserialize(const NS_YYCC_BINSTORE_TYPES::ByteArray& ba) const {
            if (ba.GetDataSize() != VALUE_SIZE) return std::nullopt;

            ValueType value;
            std::memcpy(&value, ba.GetDataPtr(), VALUE_SIZE);
            return value;
        }

        NS_YYCC_BINSTORE_TYPES::ByteArray reset() const { return this->serialize(TDefault).value(); }
    };

    struct StringSerDes {
        YYCC_DEFAULT_COPY_MOVE(StringSerDes)

        using ValueType = std::u8string;
        using HeaderType = size_t;
        static constexpr size_t HEADER_SIZE = sizeof(HeaderType);

        std::optional<NS_YYCC_BINSTORE_TYPES::ByteArray> serialize(const ValueType& value) const {
            // Get the size of string
            HeaderType length = value.size();

            // Prepare byte array and allocate size.
            NS_YYCC_BINSTORE_TYPES::ByteArray ba;
            ba.ResizeData(HEADER_SIZE + length);

            // Copy length first
            std::memcpy(ba.GetDataPtr(), &length, HEADER_SIZE);
            // Copy string data
            std::memcpy(ba.GetDataPtr(HEADER_SIZE), value.data(), length);

            // Okey
            return ba;
        }

        std::optional<ValueType> deserialize(const NS_YYCC_BINSTORE_TYPES::ByteArray& ba) const {
            // Get byte array size
            size_t ba_size = ba.GetDataSize();
            // Check whether it has header
            if (ba_size < HEADER_SIZE) return std::nullopt;

            // Get header
            HeaderType length;
            std::memcpy(&length, ba.GetDataPtr(), HEADER_SIZE);

            // Check whether full size is header + length.
            if (ba_size != HEADER_SIZE + length) return std::nullopt;
            // Prepare result
            std::u8string value(length, u8'\0');
            // Read into result
            std::memcpy(value.data(), ba.GetDataPtr(HEADER_SIZE), length);

            // Okey
            return value;
        }

        NS_YYCC_BINSTORE_TYPES::ByteArray reset() const { return this->serialize(u8"").value(); }
    };

} // namespace yycc::carton::binstore::serializer
