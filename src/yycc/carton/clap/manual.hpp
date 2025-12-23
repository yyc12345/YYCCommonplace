#pragma once
#include "application.hpp"
#include "../../macro/class_copy_move.hpp"
#include "../tabulate.hpp"
#include <iostream>

#define NS_YYCC_CLAP ::yycc::carton::clap
#define NS_YYCC_TABULATE ::yycc::carton::tabulate

namespace yycc::carton::clap::manual {

    /// @brief Structure containing translation context for manual generation.
    struct ManualTr {
    public:
        /// @brief Constructs a new ManualTr object with default values.
        ManualTr();
        ~ManualTr();
        YYCC_DEFAULT_COPY_MOVE(ManualTr)

    public:
        std::u8string author_and_version;      ///< Translated string for author and version
        std::u8string usage_title, usage_body; ///< Translated strings for usage title and body
        std::u8string avail_opt, avail_var;    ///< Translated strings for available options and variables
    };

    /// @brief Class responsible for generating help and version information for command line applications.
    class Manual {
    public:
        /**
         * @brief Constructs a new Manual object for the given application.
         * @param[in] app The application to generate manual for
         * @param[in] trctx Translation context for manual generation (default if not provided)
         */
        Manual(const NS_YYCC_CLAP::application::Application& app, ManualTr&& trctx = ManualTr());
        ~Manual();
        YYCC_DEFAULT_COPY_MOVE(Manual)

    private:
        void setup_table();    ///< Sets up the tables for displaying options and variables
        void fill_opt_table(); ///< Fills the options table with available options
        void fill_var_table(); ///< Fills the variables table with available variables

    public:
        /**
         * @brief Prints the version information of the application.
         * @param[in] dst The output stream to print to (defaults to std::cout)
         */
        void print_version(std::ostream& dst = std::cout) const;
        /**
         * @brief Prints the help information of the application.
         * @param[in] dst The output stream to print to (defaults to std::cout)
         */
        void print_help(std::ostream& dst = std::cout) const;

    private:
        ManualTr trctx;                             ///< Translation context for manual generation
        NS_YYCC_CLAP::application::Application app; ///< The application to generate manual for
        NS_YYCC_TABULATE::Tabulate opt_printer;     ///< Tabulate object for printing options
        NS_YYCC_TABULATE::Tabulate var_printer;     ///< Tabulate object for printing variables
    };

} // namespace yycc::carton::clap::manual

#undef NS_YYCC_TABULATE
#undef NS_YYCC_CLAP