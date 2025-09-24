#include "summary.hpp"

namespace yycc::carton::clap::summary {

    Summary::Summary(const std::u8string_view &name,
                     const std::u8string_view &author,
                     const std::u8string_view &version,
                     const std::u8string_view &description) : name(name), author(author), version(version), description(description) {}

    Summary::~Summary() {}

    std::u8string_view Summary::get_name() const {
        return this->name;
    }

    std::u8string_view Summary::get_author() const {
        return this->author;
    }

    std::u8string_view Summary::get_version() const {
        return this->version;
    }

    std::u8string_view Summary::get_description() const {
        return this->description;
    }

} // namespace yycc::carton::clap::summary
