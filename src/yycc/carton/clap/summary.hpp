#pragma once
#include "../../macro/class_copy_move.hpp"
#include <string>
#include <string_view>

namespace yycc::carton::clap::summary {

    /// @brief Represents summary information for a command line application.
    class Summary {
    public:
        /**
         * @brief Constructs a new Summary object with the specified parameters.
         * @param[in] name The name of the application
         * @param[in] author The author of the application
         * @param[in] version The version of the application
         * @param[in] description A description of the application's purpose
         */
        Summary(const std::u8string_view& name,
                const std::u8string_view& author,
                const std::u8string_view& version,
                const std::u8string_view& description);
        ~Summary();
        YYCC_DEFAULT_COPY_MOVE(Summary)

    public:
        /**
         * @brief Gets the name of the application.
         * @return The application's name as a string view
         */
        std::u8string_view get_name() const;
        /**
         * @brief Gets the author of the application.
         * @return The application's author as a string view
         */
        std::u8string_view get_author() const;
        /**
         * @brief Gets the version of the application.
         * @return The application's version as a string view
         */
        std::u8string_view get_version() const;
        /**
         * @brief Gets the description of the application.
         * @return The application's description as a string view
         */
        std::u8string_view get_description() const;

    private:
        std::u8string name, author, version, description;
    };

} // namespace yycc::carton::clap::summary
