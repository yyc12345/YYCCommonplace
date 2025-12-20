#pragma once
#include <string_view>
#include <type_traits>
#include <concepts>
#include <memory>
#include <stdexcept>

namespace yycc::carton::brigadier::constraint {

    class IValidator {
    public:
        IValidator();
        virtual ~IValidator();

    public:
        virtual bool validate(const std::u8string_view& sv) const = 0;
    };

    template<typename T>
    concept Converter = requires(const T& t, const std::u8string_view& sv) {
        // Check whether there is T::ReturnType type
        typename T::ReturnType;
        // Check whether there is "convert" member function and it has correct signature.
        { t.convert(sv) } -> std::same_as<typename T::ReturnType>;
    };

    //template<Converter T>
    //concept Constraint = requires(const T& t) {
    //    { t.get_validator() } -> std::same_as<std::unique_ptr<IValidator>>;
    //    { t.get_converter() } -> std::same_as<typename T::ReturnType>;
    //};

    template<typename TValidator, Converter TConverter>
        requires std::is_base_of_v<IValidator, TValidator>
    struct Constraint {
        std::unique_ptr<IValidator> validator() const { throw std::logic_error("not implemented function"); }
        TConverter converter() const { throw std::logic_error("not implemented function"); }
    };

} // namespace yycc::carton::brigadier::constraint
