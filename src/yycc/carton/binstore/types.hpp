#pragma once
#include "../../macro/class_copy_move.hpp"
#include <expected>
#include <vector>
#include <cstdint>

namespace yycc::carton::binstore::types {

    /// @brief All possible error kind occurs in this module.
    enum class BinstoreError {
        NoSuchSetting,    ///< Given token is invalid for setting.
        DuplicatedAssign, ///< Duplicated setting entry in data.
        BadVersion,       ///< The version provided in given file or IO is rejected by version strategy.
        Io,               ///< C++ IO error.
    };

    /// @brief The result type used in this module.
    template<typename T>
    using BinstoreResult = std::expected<T, BinstoreError>;

    /**
     * @brief An unique token type.
     * @details
     * When outside code registering an setting in collection
     * there must be a token returned by manager.
     * When outside code want to visit this registered item again,
     * they should provide this token returned when registering.
     * 
     * Its value actually is the index of its stored vector.
     * So this type is an alias to vector size type.
     */
    using Token = size_t;

    /**
     * @brief The identifier reprsenting version in binstore.
     * @details Higher number in identifier means that the newer version.
     * In most cases, new version can accept old version's configurations.
     * Once the struct of configurations was changed, this identifier should be bumped up.
     */
    using VersionIdentifier = uint64_t;

    /// @private
    /// @brief The raw data of setting.
    class ByteArray {
    public:
        ByteArray();
        ~ByteArray();
        YYCC_DEFAULT_COPY_MOVE(ByteArray)

    public:
        /**
         * @brief Get the length of internal buffer.
         * @remarks This is usually used when reading data.
         * @return The length of internal buffer.
         */
        size_t get_data_size() const;
        /**
         * @brief Resize internal buffer to given size.
         * @remarks This is usually used when writing data.
         * @param[in] new_size The new size of internal buffer.
         */
        void resize_data(size_t new_size);
        /**
         * @brief Get data pointer to internal buffer for reading.
         * @remarks This is usually used when reading data.
         * @param[in] offset The offset in byte added to underlying pointer.
         * @return The data pointer to internal buffer.
         */
        const void* get_data_ptr(size_t offset = 0) const;
        /**
         * @brief Get data pointer to internal buffer for writing.
         * @remarks This is usually used when writing data.
         * @param[in] offset The offset in byte added to underlying pointer.
         * @return The data pointer to internal buffer.
         */
        void* get_data_ptr(size_t offset = 0);

    private:
        std::vector<uint8_t> datas; ///< The internal buffer.
    };

} // namespace yycc::carton::binstore::types
