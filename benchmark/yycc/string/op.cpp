#include <benchmark/benchmark.h>
#include <yycc.hpp>
#include <yycc/string/op.hpp>

#define OP ::yycc::string::op
using namespace std::literals::string_view_literals;

namespace yyccbench::string::op {

    static void BM_StringStrip(benchmark::State& state) {
        std::u8string_view strl = u8" \thello\r\n"sv, words = u8" \t\r\n"sv;
        for (auto _ : state) {
            auto rv = OP::strip(strl, words);
        }
    }
    BENCHMARK(BM_StringStrip)->Name("StringStrip");

    static void BM_StringTrim(benchmark::State& state) {
        std::u8string_view strl = u8" \thello\r\n"sv, words = u8" \t\r\n"sv;
        for (auto _ : state) {
            auto rv = OP::trim(strl, words);
        }
    }
    BENCHMARK(BM_StringTrim)->Name("StringTrim");

}
