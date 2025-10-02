#include "manual.hpp"
#include "../termcolor.hpp"
#include "../../patch/stream.hpp"
#include "../../patch/format.hpp"
#include "../../string/op.hpp"
#include "../../rust/env.hpp"
#include <ranges>

#define CLAP ::yycc::carton::clap
#define TABULATE ::yycc::carton::tabulate
#define TERMCOLOR ::yycc::carton::termcolor
#define OP ::yycc::string::op
#define FORMAT ::yycc::patch::format
#define ENV ::yycc::rust::env

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

            auto desc_by_line = OP::lazy_split(opt.get_description(), u8"\n");
            for (const auto [index, item] : std::views::enumerate(desc_by_line)) {
                if (index == 0) {
                    auto full_name = TERMCOLOR::colored(opt.to_showcase_name(),
                                                        TERMCOLOR::Color::LightYellow,
                                                        TERMCOLOR::Color::Default,
                                                        TERMCOLOR::Attribute::Default);
                    auto value_hint = TERMCOLOR::colored(opt.to_showcase_value(),
                                                         TERMCOLOR::Color::LightGreen,
                                                         TERMCOLOR::Color::Default,
                                                         TERMCOLOR::Attribute::Default);
                    this->opt_printer.add_row({full_name, value_hint, item});
                } else {
                    this->opt_printer.add_row({u8"", u8"", item});
                }
            }
        }
    }

    void Manual::fill_var_table() {
        const auto &variables = app.get_variables();
        for (const auto &reg_var : variables.all_variables()) {
            const auto &var = reg_var.get_variable();

            auto desc_by_line = OP::lazy_split(var.get_description(), u8"\n");
            for (const auto [index, item] : std::views::enumerate(desc_by_line)) {
                if (index == 0) {
                    auto name = TERMCOLOR::colored(var.get_name(),
                                                   TERMCOLOR::Color::LightYellow,
                                                   TERMCOLOR::Color::Default,
                                                   TERMCOLOR::Attribute::Default);
                    this->var_printer.add_row({name, item});
                } else {
                    this->var_printer.add_row({u8"", item});
                }
            }
        }
    }

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

        // only print usage if we can fetch the name of executable
        auto executable = ENV::current_exe();
        if (executable.has_value()) {
            TERMCOLOR::cprintln(trctx.usage_title, TERMCOLOR::Color::Yellow, TERMCOLOR::Color::Default, TERMCOLOR::Attribute::Default, dst);
            dst << INDENT << FORMAT::format(trctx.usage_body, executable.value()) << std::endl;
        }

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
