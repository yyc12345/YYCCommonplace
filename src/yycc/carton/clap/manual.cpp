#include "manual.hpp"
#include "../termcolor.hpp"
#include "../../patch/stream.hpp"
#include "../../string/op.hpp"

#define CLAP ::yycc::carton::clap
#define TABULATE ::yycc::carton::tabulate
#define TERMCOLOR ::yycc::carton::termcolor
#define OP ::yycc::carton::op

using namespace ::yycc::patch::stream;

namespace yycc::carton::clap::manual::Manual {

#pragma region Manual Translation

    ManualTr::ManualTr() :
        author_and_version(u8"Invented by {0}. Version {1}."), usage_title(u8"Usage:"), usage_body(u8"{0} <options> ..."),
        avail_opt(u8"Available options:"), avail_var(u8"Available environment variables:") {}

    ManualTr::~ManualTr() {}

#pragma endregion

#pragma region Manual

    using Application = CLAP::application::Application;
    using Tabulate = TABULATE::Tabulate;

    Manual::Manual(const Application &app, ManualTr &&trctx) : trctx(std::move(trctx)), app(app), opt_printer(3), var_printer(2) {
        this->setup_table();
        this->fill_opt_table();
        this->fill_var_table();
    }

    Manual::~Manual() {}

    void Manual::setup_table() {
        this->opt_printer.show_header(false);
        this->opt_printer.show_bar(false);
        this->opt_printer.set_prefix(u8"    ");

        this->var_printer.show_header(false);
        this->var_printer.show_bar(false);
        this->var_printer.set_prefix(u8"    ");
    }

    void Manual::fill_opt_table() {
        const auto &options = app.get_options();
        for (const auto &reg_opt : options.all_options()) {
            const auto &opt = reg_opt.get_option();
        }
    }

    void Manual::fill_var_table() {}

    void Manual::print_version(std::ostream &dst) const {
        const auto &summary = this->app.get_summary();

        dst << std::format
        dst << summary.get_description() << std::endl;
        dst << std::endl;
    }

    void Manual::print_help(std::ostream &dst) const {}

#pragma endregion

} // namespace yycc::carton::clap::manual::Manual
