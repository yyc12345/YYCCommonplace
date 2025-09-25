#pragma once
#include "application.hpp"
#include "../../macro/class_copy_move.hpp"
#include "../tabulate.hpp"
#include <iostream>

#define NS_YYCC_CLAP ::yycc::carton::clap
#define NS_YYCC_TABULATE ::yycc::carton::tabulate

namespace yycc::carton::clap::manual {

    struct ManualTr {
    public:
        ManualTr();
        ~ManualTr();
        YYCC_DEFAULT_COPY_MOVE(ManualTr);

    public:
        std::u8string author_and_version;
        std::u8string usage_title, usage_body;
        std::u8string avail_opt, avail_var;
    };

    class Manual {
    public:
        Manual(const NS_YYCC_CLAP::application::Application& app, ManualTr&& trctx = ManualTr());
        ~Manual();
        YYCC_DEFAULT_COPY_MOVE(Manual);

    private:
        void setup_table();
        void fill_opt_table();
        void fill_var_table();

    public:
        void print_version(std::ostream& dst = std::cout) const;
        void print_help(std::ostream& dst = std::cout) const;

    private:
        ManualTr trctx;
        const NS_YYCC_CLAP::application::Application app;
        NS_YYCC_TABULATE::Tabulate opt_printer;
        NS_YYCC_TABULATE::Tabulate var_printer;
    };

} // namespace yycc::carton::clap::manual

#undef NS_YYCC_TABULATE
#undef NS_YYCC_CLAP
