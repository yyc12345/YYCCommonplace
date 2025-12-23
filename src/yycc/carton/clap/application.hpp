#pragma once
#include "summary.hpp"
#include "option.hpp"
#include "variable.hpp"
#include "../../macro/class_copy_move.hpp"

#define NS_YYCC_CLAP ::yycc::carton::clap

namespace yycc::carton::clap::application {

    /// @brief Represents a complete command line application with its summary, options, and environment variables.
    class Application {
    public:
        /**
         * @brief Constructs a new Application object with the given summary, options, and variables.
         * @param[in] summary The summary information for the application
         * @param[in] options The collection of command line options
         * @param[in] variables The collection of command line variables
         */
        Application(NS_YYCC_CLAP::summary::Summary&& summary,
                    NS_YYCC_CLAP::option::OptionCollection&& options,
                    NS_YYCC_CLAP::variable::VariableCollection&& variables);
        ~Application();
        YYCC_DEFAULT_COPY_MOVE(Application)

    public:
        /**
         * @brief Gets the summary information for this application.
         * @return A constant reference to the application's summary
         */
        const NS_YYCC_CLAP::summary::Summary& get_summary() const;
        /**
         * @brief Gets the collection of command line options for this application.
         * @return A constant reference to the application's options
         */
        const NS_YYCC_CLAP::option::OptionCollection& get_options() const;
        /**
         * @brief Gets the collection of environment variables for this application.
         * @return A constant reference to the application's variables
         */
        const NS_YYCC_CLAP::variable::VariableCollection& get_variables() const;

    private:
        NS_YYCC_CLAP::summary::Summary summary;               ///< The summary information for the application
        NS_YYCC_CLAP::option::OptionCollection options;       ///< The collection of command line options
        NS_YYCC_CLAP::variable::VariableCollection variables; ///< The collection of environment variables
    };

} // namespace yycc::carton::clap::application

#undef NS_YYCC_CLAP