#include "types.hpp"

namespace yycc::carton::binstore::types {

#pragma region ByteArray Class

    ByteArray::ByteArray() : datas() {}

    ByteArray::~ByteArray() {}

    size_t ByteArray::get_data_size() const {
        return this->datas.size();
    }

    void ByteArray::resize_data(size_t new_size) {
        this->datas.resize(new_size);
    }

    const void* ByteArray::get_data_ptr(size_t offset) const {
        return this->datas.data() + offset;
    }

    void* ByteArray::get_data_ptr(size_t offset) {
        return this->datas.data() + offset;
    }

#pragma endregion

} // namespace yycc::carton::binstore::types
