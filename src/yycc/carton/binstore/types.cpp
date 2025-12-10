#include "types.hpp"

namespace yycc::carton::binstore::types {

#pragma region ByteArray Class

    ByteArray::ByteArray() : datas() {}

    ByteArray::~ByteArray() {}

    size_t ByteArray::GetDataSize() const {
        return this->datas.size();
    }

    void ByteArray::ResizeData(size_t new_size) {
        this->datas.resize(new_size);
    }

    const void* ByteArray::GetDataPtr(size_t offset) const {
        return this->datas.data() + offset;
    }

    void* ByteArray::GetDataPtr(size_t offset) {
        return this->datas.data() + offset;
    }

#pragma endregion

} // namespace yycc::carton::binstore::types
