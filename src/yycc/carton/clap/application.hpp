#pragma once
#include "summary.hpp"
#include "option.hpp"
#include "variable.hpp"
#include "../../macro/class_copy_move.hpp"

#define NS_YYCC_CLAP ::yycc::carton::clap

namespace yycc::carton::clap::application {
    
    class Application {
    public:
        Application(NS_YYCC_CLAP::summary::Summary&& summary,
                    NS_YYCC_CLAP::option::OptionCollection&& options,
                    NS_YYCC_CLAP::variable::VariableCollection&& variables);
        ~Application();
        YYCC_DEFAULT_COPY_MOVE(Application);

    public:
        const NS_YYCC_CLAP::summary::Summary& get_summary() const;
        const NS_YYCC_CLAP::option::OptionCollection& get_options() const;
        const NS_YYCC_CLAP::variable::VariableCollection& get_variables() const;

    private:
        NS_YYCC_CLAP::summary::Summary summary;
        NS_YYCC_CLAP::option::OptionCollection options;
        NS_YYCC_CLAP::variable::VariableCollection variables;
    };

}

#undef NS_YYCC_CLAP
