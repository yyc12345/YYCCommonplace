#include "manual.hpp"
#include "../termcolor.hpp"
#include "../../patch/stream.hpp"
#include "../../patch/format.hpp"

#define CLAP ::yycc::carton::clap
#define TABULATE ::yycc::carton::tabulate
#define TERMCOLOR ::yycc::carton::termcolor
#define FORMAT ::yycc::patch::format

using namespace ::yycc::patch::stream;

namespace yycc::carton::clap::manual {

#pragma region Manual Translation

    ManualTr::ManualTr() :
        author_and_version(u8"Invented by {0}. Version {1}."), usage_title(u8"Usage:"), usage_body(u8"{0} <options> ..."),
        avail_opt(u8"Available options:"), avail_var(u8"Available environment variables:") {}

    ManualTr::~ManualTr() {}

#pragma endregion

#pragma region Manual

    using Application = CLAP::application::Application;
    using Tabulate = TABULATE::Tabulate;

    static constexpr char8_t INDENT[] = u8"    ";

    Manual::Manual(const Application &app, ManualTr &&trctx) : trctx(std::move(trctx)), app(app), opt_printer(3), var_printer(2) {
        this->setup_table();
        this->fill_opt_table();
        this->fill_var_table();
    }

    Manual::~Manual() {}

    void Manual::setup_table() {
        this->opt_printer.show_header(false);
        this->opt_printer.show_bar(false);
        this->opt_printer.set_prefix(INDENT);

        this->var_printer.show_header(false);
        this->var_printer.show_bar(false);
        this->var_printer.set_prefix(INDENT);
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

        TERMCOLOR::cprintln(summary.get_name(), TERMCOLOR::Color::Yellow, TERMCOLOR::Color::Default, TERMCOLOR::Attribute::Default, dst);
        dst << summary.get_name() << std::endl;
        dst << FORMAT::format(trctx.author_and_version, summary.get_author(), summary.get_version()) << std::endl;
        dst << summary.get_description() << std::endl;
        dst << std::endl;
    }

    void Manual::print_help(std::ostream &dst) const {
        this->print_version();

        TERMCOLOR::cprintln(trctx.usage_title, TERMCOLOR::Color::Yellow, TERMCOLOR::Color::Default, TERMCOLOR::Attribute::Default, dst);
        dst << INDENT << FORMAT::format(trctx.usage_body, app.get_summary().get_bin_name()) << std::endl;

        const auto &variables = app.get_variables();
        if (!variables.empty()) {
            TERMCOLOR::cprintln(trctx.avail_var, TERMCOLOR::Color::Yellow, TERMCOLOR::Color::Default, TERMCOLOR::Attribute::Default, dst);
            this->var_printer.print(dst);
            dst << std::endl;
        }

        const auto &options = app.get_options();
        if (!options.empty()) {
            TERMCOLOR::cprintln(trctx.avail_opt, TERMCOLOR::Color::Yellow, TERMCOLOR::Color::Default, TERMCOLOR::Attribute::Default, dst);
            this->opt_printer.print(dst);
            dst << std::endl;
        }
    }

#pragma endregion

} // namespace yycc::carton::clap::manual
