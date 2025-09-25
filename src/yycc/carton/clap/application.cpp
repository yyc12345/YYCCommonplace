#include "application.hpp"

#define CLAP ::yycc::carton::clap

namespace yycc::carton::clap::application {

    using Summary = CLAP::summary::Summary;
    using OptionCollection = CLAP::option::OptionCollection;
    using VariableCollection = CLAP::variable::VariableCollection;

    Application::Application(Summary &&summary, OptionCollection &&options, VariableCollection &&variables) :
        summary(std::move(summary)), options(std::move(options)), variables(std::move(variables)) {}

    Application::~Application() {}

    const Summary &Application::get_summary() const {
        return this->summary;
    }

    const OptionCollection &Application::get_options() const {
        return this->options;
    }

    const VariableCollection &Application::get_variables() const {
        return this->variables;
    }

} // namespace yycc::carton::clap::application
