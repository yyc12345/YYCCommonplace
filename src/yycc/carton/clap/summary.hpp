#pragma once
#include "../../macro/class_copy_move.hpp"
#include <string>
#include <string_view>

namespace yycc::carton::clap::summary {
    
    class Summary {
    public:
        Summary(const std::u8string_view& name,
                const std::u8string_view& author,
                const std::u8string_view& version,
                const std::u8string_view& description);
        ~Summary();
        YYCC_DEFAULT_COPY_MOVE(Summary)

    public:
        std::u8string_view get_name() const;
        std::u8string_view get_author() const;
        std::u8string_view get_version() const;
        std::u8string_view get_description() const;

    private:
        std::u8string name, author, version, description;
    };

}
