#include "stream.hpp"
#include "../string/reinterpret.hpp"

#define REINTERPRET ::yycc::string::reinterpret

namespace yycc::patch::stream {

    std::ostream& operator<<(std::ostream& os, const std::u8string_view& u8str) {
        os << REINTERPRET::as_ordinary_view(u8str);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const char8_t* u8str) {
        os << REINTERPRET::as_ordinary(u8str);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, char8_t u8chr) {
        os << static_cast<char>(u8chr);
        return os;
    }

}
