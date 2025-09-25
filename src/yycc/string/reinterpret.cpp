#include "reinterpret.hpp"

namespace yycc::string::reinterpret {

    char8_t as_utf8(const char& src) {
        return static_cast<char8_t>(src);
    }
    const char8_t* as_utf8(const char* src) {
        return reinterpret_cast<const char8_t*>(src);
    }
    char8_t* as_utf8(char* src) {
        return reinterpret_cast<char8_t*>(src);
    }
    std::u8string as_utf8(const std::string_view& src) {
        return std::u8string(reinterpret_cast<const char8_t*>(src.data()), src.size());
    }
    std::u8string_view as_utf8_view(const std::string_view& src) {
        return std::u8string_view(reinterpret_cast<const char8_t*>(src.data()), src.size());
    }

    char as_ordinary(char8_t src) {
        return static_cast<char>(src);
    }
    const char* as_ordinary(const char8_t* src) {
        return reinterpret_cast<const char*>(src);
    }
    char* as_ordinary(char8_t* src) {
        return reinterpret_cast<char*>(src);
    }
    std::string as_ordinary(const std::u8string_view& src) {
        return std::string(reinterpret_cast<const char*>(src.data()), src.size());
    }
    std::string_view as_ordinary_view(const std::u8string_view& src) {
        return std::string_view(reinterpret_cast<const char*>(src.data()), src.size());
    }

} // namespace yycc::string::reinterpret
