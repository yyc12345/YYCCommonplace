#pragma once
#include <ostream>
#include <string_view>

/**
 * @brief This namespace add UTF8 support for \c std::ostream.
 * @details
 * The operator overloads written in this namespace will give \c std::ostream ability to write UTF8 string and its char.
 * For using this feature, please directly use <TT>using namespace yycc::patch::stream;</TT> to import this namespace.
 */
namespace yycc::patch::stream {

    // TODO: replace all old way (C-style output) with this new way if possible.

    std::ostream& operator<<(std::ostream& os, const std::u8string_view& u8str);
    std::ostream& operator<<(std::ostream& os, const char8_t* u8str);
    std::ostream& operator<<(std::ostream& os, char8_t u8chr);

}
