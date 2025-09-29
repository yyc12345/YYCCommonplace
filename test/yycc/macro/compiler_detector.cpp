#include <gtest/gtest.h>
#include <yycc.hpp>
#include <yycc/macro/compiler_detector.hpp>

#define COMPILER ::yycc::macro::compiler

namespace yycctest::macro::compiler {

    TEST(MacroCompiler, Main) {
        auto rv = COMPILER::get_compiler();
#if defined(YYCC_CC_MSVC)
        EXPECT_EQ(rv, COMPILER::CompilerKind::Msvc);
#elif defined(YYCC_CC_GCC)
        EXPECT_EQ(rv, COMPILER::CompilerKind::Gcc);
#else
        EXPECT_EQ(rv, COMPILER::CompilerKind::Clang);
#endif
    }

} // namespace yycctest::macro::compiler
